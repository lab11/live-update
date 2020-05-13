import argparse
import serial
import os
import random
import re
import struct
import sys
import time
import json

from intelhex import IntelHex

CURRENT_VERSION = 0x8
FLASH_BASE = 0x200000

FLASHED_VERSION_NAME = 'lastFlashedNum.txt'
FLASHED_SYMBOLS_NAME = 'flashed.symbols'

# -- Utility functions --

def get_app_name(folder):
    return os.path.basename(os.path.abspath(folder))


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


def valid_partition(app_folder, manifest):
    flashed_version_filename = os.path.join(app_folder, '_build', FLASHED_VERSION_NAME)

    if not os.path.exists(flashed_version_filename):
        print('Flashed version number not found at', flashed_version_filename)
        return False

    with open(flashed_version_filename, 'r') as f:
        flashed_version_number = f.readlines()[0]

    flashed_partition = (int(flashed_version_number.split('+')[1]) + 1) % 2

    if flashed_partition == manifest['update_partition']:
        print('Current version partition ({} for version {}) conflicts with flashed version partition ({} for version {}), re-run `make` to resolve to different partition.'.format(manifest['update_partition'], manifest['update_version'], flashed_partition, flashed_version_number))
        return False

    return True


# TODO make better
def parse_ast(f):

    ast = [l.strip() for l in f.readlines()]

    # find all the timer declarations
    timers = {}
    mode = 'init' # init, timer, expiry, stop
    current_timer = None

    for l in ast:
        m = re.search('-VarDecl.*used\s(.*)\s\'struct k_timer', l)
        if m:
            if m.group(1) not in timers:
                timers[m.group(1)] = {}
            continue

        if mode == 'init':
            m = re.search('.*Function.*\'k_timer_init\'', l)
            if m:
                mode = 'timer'
                continue

        if mode == 'timer':
            m = re.search('\'struct k_timer\':.*\s\'(.*)\'\s\'struct k_timer', l)
            if m:
                current_timer = m.group(1)
                mode = 'expiry'
                continue

        if mode == 'expiry':
            m = re.search('void \(struct k_timer \*\).*\s\'(.*)\'\s.void \(struct k_timer \*\)', l)
            if m:
                timers[current_timer]['expire_callback'] = m.group(1)
                mode = 'stop'
                continue

            if 'k_timer_expiry_t' in l and 'NullToPointer' in l:
                timers[current_timer]['expire_callback'] = None
                mode = 'stop'
                continue

        if mode == 'stop':
            m = re.search('void \(struct k_timer \*\).*\s\'(.*)\'\s.void \(struct k_timer \*\)', l)
            if m:
                timers[current_timer]['stop_callback'] = m.group(1)
                mode = 'init'
                continue

            if 'k_timer_stop_t' in l and 'NullToPointer' in l:
                timers[current_timer]['stop_callback'] = None
                mode = 'init'
                continue

    return {
        'timers': timers
    }
    

def generate_update_header(manifest, app_folder, update_folder):

    header = {}

    flashed_symbols = os.path.join(app_folder, '_build', FLASHED_SYMBOLS_NAME)
    update_symbols = os.path.join(update_folder, manifest['update_symbols'])

    main_ptr_addr = get_symbol_address(flashed_symbols, 'main_ptr')
    if not main_ptr_addr:
        print('Could not locate main_ptr_addr, exiting...')
        exit(1)
    header['main_ptr_addr'] = main_ptr_addr

    update_flag_addr = get_symbol_address(flashed_symbols, 'update_flag')
    if not update_flag_addr:
        print('Could not locate update_flag_addr, exiting...')
        exit(1)
    header['update_flag_addr'] = update_flag_addr

    with open(os.path.join(update_folder, manifest['update_ast']), 'r') as f:
        ast_metadata = parse_ast(f)
        header['init_size'] = 3 * len(ast_metadata['timers']) * 4

    '''
    print('\nTimer callbacks:')
    for t in ast_metadata['timers']:
        t_dict = ast_metadata['timers'][t]
        print("    {} ({}):".format(t, hex(get_symbol_address(update_symbols, t))))
        print("      expiry: {}".format(
            hex(get_symbol_address(update_symbols, t_dict['expire_callback'])) if t_dict['expire_callback'] else '0x0'
        ))
        print("      stop: {}".format(
            hex(get_symbol_address(update_symbols, t_dict['stop_callback'])) if t_dict['stop_callback'] else '0x0'
        ))
    '''

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

    return header


def generate_update_payloads(header, manifest, app_folder, update_folder):

    payloads = {}

    # look at symbols for appflash_text_rom_start, appflash_rodata_rom_start and sizes for both
    hex_filename = os.path.join(update_folder, manifest['update_hex'])
    ih = IntelHex(hex_filename)

    # extract .text segment
    payloads['text'] = bytes(
        ih[FLASH_BASE + header['appflash_text_rom_start']: FLASH_BASE + header['appflash_text_rom_start'] + header['appflash_text_size']].tobinarray())

    # extract .rodata segment
    payloads['rodata'] = bytes(
        ih[FLASH_BASE + header['appflash_rodata_rom_start']: FLASH_BASE + header['appflash_rodata_rom_start'] + header['appflash_rodata_size']].tobinarray())

    # generate state transfer information
    flashed_symbols = os.path.join(app_folder, '_build', FLASHED_SYMBOLS_NAME)
    update_symbols = os.path.join(update_folder, manifest['update_symbols'])

    flashed_ram = get_symbol_address(flashed_symbols, '_appram_mpu_ro_region_start')
    update_ram = get_symbol_address(update_symbols, '_appram_mpu_ro_region_start')

    if flashed_ram == update_ram:
        print('Error: flashed image and update image both using the same RAM section: {} (flashed) {} (update)'.format(hex(flashed_ram), hex(update_ram)))
        exit(1)

    syms = get_symbols_in_section(update_symbols, '\.app_data')
    syms += get_symbols_in_section(update_symbols, '\.app_bss')

    syms = [s for s in syms if get_symbol_size(update_symbols, s + '$') != 0]

    print('\nTransfer payload:')
    transfer_triples = []
    for s in syms:
        size = get_symbol_size(update_symbols, s + '$') 

        update_addr = get_symbol_address(update_symbols, s + '$')
        flashed_addr = get_symbol_address(flashed_symbols, s + '$')

        if not flashed_addr: # symbol not in old version, so no transfer generated
            continue

        print('    {} -> {} ({} bytes)'.format(hex(flashed_addr), hex(update_addr), size))
        transfer_triples += [flashed_addr, update_addr, size]

    triples_size = len(syms) * 3
    header['triples_bytes'] = triples_size * 4 # 3 values per memory region, each 4 bytes
    payloads['transfer'] = struct.pack('I' * triples_size, *transfer_triples)

    with open(os.path.join(update_folder, manifest['update_ast']), 'r') as f:
        ast_metadata = parse_ast(f)
    init_address_value_triples = []

    print('\nInit payload:')
    for t in ast_metadata['timers']:
        t_dict = ast_metadata['timers'][t]

        t_addr = get_symbol_address(update_symbols, t + '$')
        t_expiry = get_symbol_address(update_symbols, t_dict['expire_callback']) | 0x1 if t_dict['expire_callback'] else 0x0
        t_stop = get_symbol_address(update_symbols, t_dict['stop_callback']) | 0x1 if t_dict['stop_callback'] else 0x0

        init_address_value_triples += [t_addr, t_expiry, t_stop]

        print("    {} ({}):".format(t, hex(get_symbol_address(update_symbols, t + '$'))))
        print("      expiry: {}".format(
            hex(get_symbol_address(update_symbols, t_dict['expire_callback'])) if t_dict['expire_callback'] else '0x0'
        ))
        print("      stop: {}".format(
            hex(get_symbol_address(update_symbols, t_dict['stop_callback'])) if t_dict['stop_callback'] else '0x0'
        ))
        
    payloads['init'] = struct.pack('I' * len(ast_metadata['timers']) * 3, *init_address_value_triples)

    return payloads


def serialize_header(header, payloads):

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
        transfer_triples_size: {},
        init_size: {}
    }}
    total size (expected): {}
    total size (actual): {}
    """.format(
        CURRENT_VERSION,
        hex(header['main_ptr_addr']),
        hex(header['main_addr'] | 0x1), # main ptr (w/ thumb)
        hex(header['update_flag_addr']),
        hex(header['appflash_text_start']),
        hex(header['appflash_text_size']),
        hex(header['appflash_rodata_start']),
        hex(header['appflash_rodata_size']),
        hex(header['appram_bss_start']),
        hex(header['appram_bss_size']),
        hex(header['appram_bss_start_addr']),
        hex(header['appram_bss_size_addr']),
        hex(header['triples_bytes']),
        hex(header['init_size']),
        4 * len(header) + header['appflash_text_size'] + header['appflash_rodata_size'] + header['triples_bytes'] + header['init_size'],
        4 * len(header) + sum(len(payloads[x]) for x in payloads)
    )
    print(header_str)

    return struct.pack('IIIIIIIIIIIIII',
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
        header['triples_bytes'],
        header['init_size']
    )


if __name__ == '__main__':

    parser = argparse.ArgumentParser(description='Primary control script for the live update process.')
    parser.add_argument('app_folder', help='App folder')
    parser.add_argument('dev', help='Serial port')
    parser.add_argument('--update_folder', help='Specify specific update package folder instead of deriving from app location', default=None)
    parser.add_argument('--baud', help='Baud rate', type=int, default=115200)
    parser.add_argument('--timeout', help='Timeout', type=int, default=2)
    parser.add_argument('--force', help='Override slot warning', action='store_true', default=False)
    parser.add_argument('--dry_run', help='Does not attempt serial communication', action='store_true', default=False)
    args = parser.parse_args()

    if args.update_folder:
        update_folder = args.update_folder
    else:
        update_folder = os.path.join(args.app_folder, '_build', 'update')

    with open(os.path.join(update_folder, 'manifest.json'), 'r') as f:
        manifest = json.load(f)

    if manifest['valid'] and valid_partition(args.app_folder, manifest):
        update_header = generate_update_header(
            manifest,
            args.app_folder,
            update_folder
        )
    
        update_payloads = generate_update_payloads(
            update_header,
            manifest,
            args.app_folder,
            update_folder
        )
    
        serialized_update_header = serialize_header(update_header, update_payloads)

        if not args.dry_run:
            with serial.Serial(args.dev, args.baud, timeout=args.timeout) as ser:

                print("sending header...")
                ser.write(serialized_update_header)

                time.sleep(1)
                print("sending app_text...")
                ser.write(update_payloads['text'])

                time.sleep(1)
                print("sending rodata...")
                ser.write(update_payloads['rodata'])

                time.sleep(1)
                print("sending transfer data...")
                ser.write(update_payloads['transfer'])

                time.sleep(1)
                print("sending init data...")
                ser.write(update_payloads['init'])

            print("updating last flashed version number...")
            flashed_version_filename = os.path.join(args.app_folder, '_build', FLASHED_VERSION_NAME)
            os.system('echo {} > {}'.format(manifest['update_version'], flashed_version_filename))

            print("updating last flashed symbol file...")
            flashed_symbols_filename = os.path.join(args.app_folder, '_build', FLASHED_SYMBOLS_NAME)

            os.system('cp {} {}'.format(
                os.path.join(update_folder, manifest['update_symbols']),
                flashed_symbols_filename
            ))

