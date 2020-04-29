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

CURRENT_VERSION = 0x7
FLASH_BASE = 0x200000

LAST_VERSION_NAME = '.lastVerNum.txt'
FLASHED_VERSION_NAME = 'lastFlashedNum.txt'

# -- Utility functions --

def get_app_name(folder):
    return os.path.basename(os.path.abspath(folder))


def dump_symbols(elf, dest):
    os.system('arm-none-eabi-objdump -t ' + elf + ' > ' + dest)


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


# check made and flashed versions, warn to remake if necessary
def validate_binary(app_dir, build_dir, force):

    make_version_filename = os.path.join(app_dir, LAST_VERSION_NAME)

    with open(make_version_filename, 'r') as make_version_f:

        flash_version_filename = os.path.join(build_dir, FLASHED_VERSION_NAME)
        with open(flash_version_filename, 'r') as flash_version_f:
            make_partition = int(make_version_f.readline()[-1]) % 2
            flash_partition = int(flash_version_f.readline()[-1]) % 2
    
            if make_partition == flash_partition and not force:
                print('Both flashed and current compiled versions are linked to a conflicting partition, re-`make` app')
                exit(1)


def generate_update_header(app_dir, build_dir, update_metadata_filename, update_symbols_filename, flashed_symbols_filename):

    header = {}

    with open(update_metadata_filename, 'r') as f:
        metadata = json.load(f)

        header['main_ptr_addr'] = int(metadata['main_ptr_addr'], base=16)
        header['update_flag_addr'] = int(metadata['update_flag_addr'], base=16)

    elf_filename = os.path.join(build_dir, get_app_name(app_dir) + '_ns.elf')
    dump_symbols(elf_filename, update_symbols_filename)
    update_symbols = update_symbols_filename
    flashed_symbols = flashed_symbols_filename

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
        appflash_rodata_start = 0x0
        appflash_rodata_size = 0
        appflash_rodata_rom_start = 0x0
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
        appram_bss_start = 0x0
        appram_bss_size = 0
    elif not header['appram_bss_start'] or not header['appram_bss_size']:
        print('Could not locate appram_bss_start or appram_bss_size but did locate the other, exiting...')
        exit(1)

    return header


def generate_update_payloads(app_dir, build_dir, update_symbols_filename, flashed_symbols_filename, header):

    payloads = {}

    # look at symbols for appflash_text_rom_start, appflash_rodata_rom_start and sizes for both
    hex_filename = os.path.join(build_dir, get_app_name(app_dir) + '_merged.hex')
    ih = IntelHex(hex_filename)

    # extract .text segment
    payloads['text'] = bytes(
        ih[FLASH_BASE + header['appflash_text_rom_start']: FLASH_BASE + header['appflash_text_rom_start'] + header['appflash_text_size']].tobinarray())

    # extract .rodata segment
    payloads['rodata'] = bytes(
        ih[FLASH_BASE + header['appflash_rodata_rom_start']: FLASH_BASE + header['appflash_rodata_rom_start'] + header['appflash_rodata_size']].tobinarray())

    # generate state transfer information
    elf_filename = os.path.join(build_dir, get_app_name(app_dir) + '_ns.elf')
    dump_symbols(elf_filename, update_symbols_filename)
    update_symbols = update_symbols_filename
    flashed_symbols = flashed_symbols_filename

    syms = get_symbols_in_section(update_symbols, '\.app_data')
    syms += get_symbols_in_section(update_symbols, '\.app_bss')

    syms = [s for s in syms if get_symbol_size(update_symbols, s + '$') != 0]

    transfer_triples = []
    for s in syms:
        size = get_symbol_size(update_symbols, s + '$') 

        update_addr = get_symbol_address(update_symbols, s + '$')
        flashed_addr = get_symbol_address(flashed_symbols, s + '$')

        print('    {} -> {} ({} bytes)'.format(hex(flashed_addr), hex(update_addr), size))
        transfer_triples += [flashed_addr, update_addr, size]

    triples_size = len(syms) * 3
    header['triples_bytes'] = triples_size * 4 # 3 values per memory region, each 4 bytes
    payloads['transfer'] = struct.pack('I' * triples_size, *transfer_triples)

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
        transfer_triples_size: {}
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
        4 * len(header) + header['appflash_text_size'] + header['appflash_rodata_size'] + header['triples_bytes'],
        4 * len(header) + len(payloads['text']) + len(payloads['rodata']) + len(payloads['transfer'])
    )
    print(header_str)

    return struct.pack('IIIIIIIIIIIII',
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
    )


if __name__ == '__main__':

    parser = argparse.ArgumentParser(description='Primary control script for the live update process.')
    parser.add_argument('app_folder', help='Application folder')
    parser.add_argument('dev', help='Serial port')
    parser.add_argument('--baud', help='Baud rate', type=int, default=115200)
    parser.add_argument('--timeout', help='Timeout', type=int, default=None)
    parser.add_argument('--force', help='Override slot warning', action='store_true', default=False)
    args = parser.parse_args()

    # important paths
    build_dir = os.path.join(args.app_folder, '_build')
    update_symbol_filename = os.path.join(build_dir, 'update.symbols')
    update_metadata_filename = os.path.join(build_dir, 'update.txt')
    flashed_symbol_filename = os.path.join(build_dir, 'flashed.symbols')

    validate_binary(args.app_folder, build_dir, args.force)

    update_header = generate_update_header(
        args.app_folder,
        build_dir,
        update_metadata_filename,
        update_symbol_filename,
        flashed_symbol_filename
    )
    
    update_payloads = generate_update_payloads(
        args.app_folder,
        build_dir,
        update_symbol_filename,
        flashed_symbol_filename,
        update_header
    )

    serialized_update_header = serialize_header(update_header, update_payloads)

    with serial.Serial(args.dev, args.baud, timeout=2) as ser:

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

    print("updating last flashed version number...")
    last_version_filename = os.path.join(args.app_folder, LAST_VERSION_NAME)
    last_flashed_filename = os.path.join(args.app_folder, build_dir, FLASHED_VERSION_NAME)
    os.system('cp ' + last_version_filename + ' ' + last_flashed_filename)

    print("updating last flashed symbol file...")
    os.system('cp ' + update_symbol_filename + ' ' + flashed_symbol_filename)

    print('done')

