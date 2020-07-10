import argparse
from copy import deepcopy
import json
import networkx as nx
import os
import pprint
import random
import re
import serial
import struct
import sys
import time

from intelhex import IntelHex

CURRENT_VERSION = 11
FLASH_BASE = 0x0

FLASHED_VERSION_NAME = 'lastFlashedNum.txt'
FLASHED_SYMBOLS_NAME = 'flashed.symbols'

# -- Utility functions --

def search_symbols(filename, pattern, cb):
    with open(filename, 'r') as f:
        for l in f:
            if re.search(pattern, l):
                cb(l)


def get_symbols_in_section(filename, section_re):
    symbols = []
    def cb(l):
        nonlocal symbols
        symbols.append(l.split()[-1])
    search_symbols(filename, section_re, cb)
    return symbols


def get_symbol_address(filename, symbol_re):
    addr = None
    def cb(l):
        nonlocal addr
        addr = int(l.split()[0], base=16)
    search_symbols(filename, symbol_re, cb)
    return addr


def get_symbol_size(filename, symbol_re):
    size = 0
    def cb(l):
        nonlocal size
        size = int(l.split()[-2], base=16)
    search_symbols(filename, symbol_re, cb)
    return size


def get_symbol_name(filename, addr_re):
    name = None
    def cb(l):
        nonlocal name
        name = l.split()[-1]
    search_symbols(filename, format(addr_re, 'x'), cb)
    return name


def serialize_mem_init(update_data):

    ''' (all 4 byte ints/ptrs)

    mem_init_size
    { # each mem init
        <base> <offset> <val>
    }
    ...

    '''

    mem_init_bytes = struct.pack('I', 1 * 4 + (3 * 4 * len(update_data['mem_init'])))
    for m in update_data['mem_init']:
        mem_init_bytes += struct.pack('III', *m)

    return mem_init_bytes
    

def serialize_hw_init(update_data):

    ''' (all 4 byte ints/ptrs)

    hw_init_size
    { # each hw init
        size
        <fn_ptr/args...>
    }
    ...

    '''
    
    hw_bytes = bytes()
    for hw in update_data['hw_init']:
        hw_bytes += struct.pack('I' + 'I'*len(hw), 1 * 4 + (len(hw) * 4), *hw)

    total_hw_size = len(hw_bytes) + 4
    return struct.pack('I', total_hw_size) + hw_bytes


def serialize_transfers(update_data):

    ''' (all 4 byte ints/ptrs)

    transfers_size
    { # each transfer
        <origin> <dest> <size>
    }
    ...

    '''

    transfers_payload = struct.pack('I', 4 + len(update_data['state_transfer']) * 3 * 4)

    for t in update_data['state_transfer']:
        transfers_payload += struct.pack('III', t, update_data['state_transfer'][t][0], update_data['state_transfer'][t][1])

    return transfers_payload


def serialize_predicates(update_data):

    ''' (all 4 byte ints/ptrs)

    predicates_size
    { # each predicate
        size
        event
        updated_event
        n_inactive_ops
        n_reset_ops
        n_constraints
        n_state_init
        hw_transfer_size 
        <inactive_ops>
        <reset_ops>
        { # each constraint
            size
            symbol
            n_ranges
            { # each range
                <range lower> <range upper>
            }
        }
        { # each state init val
            <addr> <offset> <size>
        }
        { # each hw transfer
            size
            <fn_ptr/args...>
        }   
    }
    ...

    '''

    predicate_payload = bytes()

    for p in update_data['predicate_transfer']:
        predicate, init_transfers, hw_transfer_calls = p

        predicate_header = struct.pack('IIII',
                predicate['event_name'], predicate['new_event_name'], len(predicate['inactive_ops']), len(predicate['reset_ops']))
        predicate_size = 4 * 4
    
        # fill in var length sections

        predicate_bytes = bytes()

        for io in predicate['inactive_ops']:
            predicate_bytes += struct.pack('I', io)
            predicate_size += 1 * 4

        for ro in predicate['reset_ops']:
            predicate_bytes += struct.pack('I', ro)
            predicate_size += 1 * 4

        for c in predicate['constraints']:
            constraint_bytes = struct.pack('II', c['symbol'], len(c['range']))
            constraint_size = 2 * 4

            for r in c['range']:
                constraint_bytes += struct.pack('ii', r[0], r[1])
                constraint_size += 2 * 4

            # prepend total constraint size
            constraint_size += 1 * 4
            predicate_bytes += struct.pack('I', constraint_size) + constraint_bytes
            predicate_size += constraint_size

        for it in init_transfers:
            predicate_bytes += struct.pack('III', it[0], it[1], it[2])
            predicate_size += 3 * 4

        hw_transfer_size = 0
        for hw_call in hw_transfer_calls:
            predicate_bytes += struct.pack('I' + 'I'*len(hw_call), 1 * 4 + (len(hw_call) * 4), *hw_call)
            hw_transfer_size += 1 * 4 + (len(hw_call) * 4)
        predicate_size += hw_transfer_size

        predicate_header += struct.pack('III',
            len(predicate['constraints']),
            len(init_transfers),
            hw_transfer_size
        )
        predicate_size += 4 * 4
        print('PREDICATE SIZE', predicate_size)
        predicate_header = struct.pack('I', predicate_size) + predicate_header

        predicate_payload += predicate_header + predicate_bytes

    return struct.pack('I', 4 + len(predicate_payload)) + predicate_payload


def generate_update_payload(update_folder, flashed_symbols, update_symbols, update_data, force):

    header = {}

    main_ptr_addr = get_symbol_address(flashed_symbols, 'rodata.*main_ptr')
    if not main_ptr_addr:
        print('Could not locate main_ptr_addr, exiting...')
        main_ptr_addr = 0
        #exit(1)
    header['main_ptr_addr'] = main_ptr_addr

    update_flag_addr = get_symbol_address(flashed_symbols, 'update_flag')
    if not update_flag_addr:
        print('Could not locate update_flag_addr, exiting...')
        exit(1)
    header['update_flag_addr'] = update_flag_addr

    header['main_addr'] = get_symbol_address(update_symbols, '\smain$')

    header['appflash_text_start'] = get_symbol_address(update_symbols, 'appflash_text_start')
    if not header['appflash_text_start']:
        print('Could not locate appflash_text_start, exiting...')
        exit(1)

    header['appflash_text_size'] = get_symbol_address(update_symbols, 'appflash_text_size')
    if not header['appflash_text_size']:
        print('Could not locate appflash_text_size, exiting...')
        exit(1)

    header['appflash_text_rom_start'] = get_symbol_address(update_symbols, 'appflash_text_rom_start')
    if not header['appflash_text_rom_start']:
        print('Could not locate appflash_text_rom_start, exiting...')
        exit(1)

    header['appflash_rodata_start'] = get_symbol_address(update_symbols, 'appflash_rodata_start')
    header['appflash_rodata_size'] = get_symbol_address(update_symbols, 'appflash_rodata_size')
    header['appflash_rodata_rom_start'] = get_symbol_address(update_symbols, 'appflash_rodata_rom_start')

    if not header['appflash_rodata_start'] and \
       not header['appflash_rodata_size'] and \
       not header['appflash_rodata_rom_start']: # there's just no rodata in update, that's ok
        header['appflash_rodata_start'] = 0x0
        header['appflash_rodata_size'] = 0
        header['appflash_rodata_rom_start'] = 0x0
    elif not header['appflash_rodata_size'] or \
         not header['appflash_rodata_start'] or \
         not header['appflash_rodata_rom_start']:
        print('Could not locate appflash_rodata_size or appflash_rodata_start or appflash_rodata_rom_start, exiting...')
        exit(1)

    header['appram_bss_start'] = get_symbol_address(update_symbols, 'appram_bss_start')
    header['appram_bss_size'] = get_symbol_address(update_symbols, 'appram_bss_size')
    header['appram_bss_start_addr'] = get_symbol_address(update_symbols, 'appram_bss_update_start')
    header['appram_bss_size_addr'] = get_symbol_address(update_symbols, 'appram_bss_update_size')

    if not header['appram_bss_start'] and not header['appram_bss_size']:
        header['appram_bss_start'] = 0x0
        header['appram_bss_size'] = 0
        header['appram_bss_start_addr'] = 0x0
        header['appram_bss_size_addr'] = 0x0
    elif not header['appram_bss_start'] or not header['appram_bss_size']:
        print('Could not locate appram_bss_start or appram_bss_size but did locate the other, exiting...')
        exit(1)

    payloads = {}

    # look at symbols for appflash_text_rom_start, appflash_rodata_rom_start and sizes for both
    hex_filename = os.path.join(update_folder, 'update.hex')
    ih = IntelHex(hex_filename)

    # extract .text segment
    payloads['text'] = bytes(
        ih[FLASH_BASE + header['appflash_text_rom_start']: FLASH_BASE + header['appflash_text_rom_start'] + header['appflash_text_size']].tobinarray())

    # extract .rodata segment
    payloads['rodata'] = bytes(
        ih[FLASH_BASE + header['appflash_rodata_rom_start']: FLASH_BASE + header['appflash_rodata_rom_start'] + header['appflash_rodata_size']].tobinarray())

    flashed_ram = get_symbol_address(flashed_symbols, '_appram_mpu_ro_region_start')
    update_ram = get_symbol_address(update_symbols, '_appram_mpu_ro_region_start')

    if flashed_ram == update_ram and not force:
        print('Error: flashed image and update image both using the same RAM section: {} (flashed) {} (update)'.format(hex(flashed_ram), hex(update_ram)))
        exit(1)

    payloads['predicates'] = serialize_predicates(update_data)
    payloads['transfers'] = serialize_transfers(update_data)
    payloads['hw_init'] = serialize_hw_init(update_data)
    payloads['mem_init'] = serialize_mem_init(update_data)

    header['payload_size'] = sum([len(v) for k,v in payloads.items()])
    return header, payloads


def serialize_header(header):

    # Sanity check print header
    header_str = """
    header = {{
        version: {},
        main_ptr_addr: {},
        main_addr: {},
        update_flag_addr: {},
        appflash_text_start: {},
        appflash_text_size: {},
        appflash_rodata_start: {},
        appflash_rodata_size: {},
        appram_bss_start: {},
        appram_bss_size: {},
        appram_bss_start_addr: {},
        appram_bss_size_addr: {},
        payload_size: {},
    }}
    """.format(
        CURRENT_VERSION,
        hex(header['main_ptr_addr']),
        hex(header['main_addr'] | 0x1),
        hex(header['update_flag_addr']),
        hex(header['appflash_text_start']),
        hex(header['appflash_text_size']),
        hex(header['appflash_rodata_start']),
        hex(header['appflash_rodata_size']),
        hex(header['appram_bss_start']),
        hex(header['appram_bss_size']),
        hex(header['appram_bss_start_addr']),
        hex(header['appram_bss_size_addr']),
        hex(header['payload_size']),
    )
    print(header_str)

    return struct.pack('I' * 13,
        CURRENT_VERSION,
        header['main_ptr_addr'],
        header['main_addr'] | 0x1, # make sure main ptr is thumb!
        header['update_flag_addr'],
        header['appflash_text_start'],
        header['appflash_text_size'],
        header['appflash_rodata_start'],
        header['appflash_rodata_size'],
        header['appram_bss_start'],
        header['appram_bss_size'],
        header['appram_bss_start_addr'],
        header['appram_bss_size_addr'],
        header['payload_size'],
    )

    return header


def resolve_symbol_addresses(update_data, flashed_symbols, update_symbols):
    
    for hw_call in update_data['hw_init']:
        for i, a in enumerate(hw_call):
            if type(a) == str:
                a = a[1:] if a[0] == '&' else a
                hw_call[i] = get_symbol_address(update_symbols, '\s{}$'.format(a))

    for predicate_pair in update_data['predicate_transfer']:
        predicate, state_inits, hw_inits = predicate_pair

        # convert predicate
        event_handler_name = predicate['event_name']
        predicate['event_name'] = get_symbol_address(flashed_symbols, '\s{}$'.format(event_handler_name))
        predicate['new_event_name'] = get_symbol_address(update_symbols, '\s{}$'.format(event_handler_name))

        op_addrs = []
        for ro in predicate['reset_ops']:
            op_addrs.append(get_symbol_address(flashed_symbols, '\s{}$'.format(ro)))
        predicate['reset_ops'] = op_addrs

        io_addrs = []
        for io in predicate['inactive_ops']:
            io_addrs.append(get_symbol_address(flashed_symbols, '\s{}$'.format(io)))
        predicate['inactive_ops'] = io_addrs

        translated_c = []
        for c in predicate['constraints']:
            c['size'] = get_symbol_size(flashed_symbols, '\s{}$'.format(c['symbol']))
            c['symbol'] = get_symbol_address(flashed_symbols, '\s{}$'.format(c['symbol']))
            translated_c.append(c)

        predicate['constraints'] = translated_c

        # convert each state init entry (symbol, offset, val)
        for init in state_inits:
            for i, e in enumerate(init):
                if type(e) == str:
                    init[i] = get_symbol_address(update_symbols, '\s{}$'.format(e))

        for hw_init in hw_inits:
            for i, e in enumerate(hw_init):
                if type(e) == str:
                    hw_init[i] = get_symbol_address(update_symbols, '\s{}$'.format(e))

    for mem_init in update_data['mem_init']:
        for i, e in enumerate(mem_init):
            if type(e) == str:
                mem_init[i] = get_symbol_address(update_symbols, '\s{}$'.format(e))

    for origin in list(update_data['state_transfer'].keys()):
        dest_size_pair = update_data['state_transfer'][origin]

        update_data['state_transfer'][get_symbol_address(flashed_symbols, '\s{}$'.format(origin))] = \
            (get_symbol_address(update_symbols, '\s{}$'.format(dest_size_pair[0])), dest_size_pair[1])
        del update_data['state_transfer'][origin]

    return update_data


if __name__ == '__main__':

    parser = argparse.ArgumentParser(description='Primary control script for the live update process.')
    parser.add_argument('app_folder', help='App folder')
    parser.add_argument('dev', help='Serial port')
    parser.add_argument('--update_folder', help='Specify specific update package folder instead of deriving from app location', default=None)
    parser.add_argument('--flashed_folder', help='Specify specific flashed package folder instead of deriving from app location', default=None)
    parser.add_argument('--baud', help='Baud rate', type=int, default=115200)
    parser.add_argument('--timeout', help='Timeout', type=int, default=2)
    parser.add_argument('--force', help='Override slot warning', action='store_true', default=False)
    parser.add_argument('--dry_run', help='Does not attempt serial communication', action='store_true', default=False)
    parser.add_argument('--no_update_flashed', help='Does not overwrite currently flashed info', action='store_true', default=False)
    args = parser.parse_args()

    if args.update_folder:
        update_folder = args.update_folder
        load_transfer_from_update_folder = True
    else:
        update_folder = os.path.join(args.app_folder, '_build', 'update')
        load_transfer_from_update_folder = False 

    if args.flashed_folder:
        flashed_folder = args.flashed_folder
    else:
        flashed_folder = os.path.join(args.app_folder, '_build', 'flashed')

    with open(os.path.join(update_folder, 'manifest.json'), 'r') as f:
        manifest = json.load(f)

    with open(os.path.join(flashed_folder, 'manifest.json'), 'r') as f:
        flashed_manifest = json.load(f)

    if manifest['valid'] and flashed_manifest['valid'] and (manifest['update_partition'] != flashed_manifest['update_partition']  or args.force):

        TRANSFER_OUT = update_folder + '/update.transfer'
        os.system('python3 {} {} {} {} {} {} {}'.format(
            args.app_folder + '/../../scripts/gen_transfer.py', # script
            args.app_folder if not load_transfer_from_update_folder else update_folder, # transfer script folder
            flashed_folder + '/update.predicates', # original predicates
            flashed_folder + '/update.symbols', # original symbols
            update_folder + '/update.predicates', # updated predicates
            update_folder + '/update.symbols', # updated symbols
            TRANSFER_OUT, # output file
        ))

        with open(TRANSFER_OUT, 'r') as f:
            update_data = json.load(f)

        flashed_symbols = os.path.join(flashed_folder, 'update.symbols')
        update_symbols = os.path.join(update_folder, 'update.symbols')

        print()
        print('BEFORE RESOLVED')
        pprint.pprint(update_data)
        update_data = resolve_symbol_addresses(update_data, flashed_symbols, update_symbols)
        print()
        print('AFTER RESOLVED')
        pprint.pprint(update_data)

        header, payloads = generate_update_payload(
            update_folder,
            flashed_symbols,
            update_symbols,
            update_data,
            args.force
        )

        for p in payloads:
            print(p, len(payloads[p]))

        header = serialize_header(header)

        if not args.dry_run:
            with serial.Serial(args.dev, args.baud, timeout=args.timeout) as ser:

                total_size = len(header)
                print('sending header', len(header), '...')
                ser.write(header)

                for payload_name in ['text', 'rodata', 'predicates', 'transfers', 'hw_init', 'mem_init']:
                    time.sleep(1)
                    total_size += len(payloads[payload_name])
                    print('sending', payload_name, len(payloads[payload_name]), '...')
                    ser.write(payloads[payload_name])

                print('device should have received', total_size, 'bytes total')

            if not args.no_update_flashed:

                print("updating last flashed version number...")
                flashed_version_filename = os.path.join(args.app_folder, '_build', FLASHED_VERSION_NAME)
                os.system('echo {} > {}'.format(manifest['update_version'], flashed_version_filename))

                print("updating last flashed folder...")
                os.system('rm -rf {}'.format(
                    flashed_folder
                ))
                os.system('cp -r {} {}'.format(
                    update_folder,
                    flashed_folder
                ))

