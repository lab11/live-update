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


BUILD_DIR = '_build'
SYMBOL_FILE = '/tmp/update.symbols'


def get_app_name(folder):
    return os.path.basename(os.path.abspath(folder))


def dump_symbols(folder):
    # hardcoded for now
    filename = SYMBOL_FILE

    os.system('arm-none-eabi-objdump -t ' + folder + os.sep + BUILD_DIR + os.sep + get_app_name(folder) + '_ns.elf > ' + filename)
    return filename


def get_symbol_address(filename, symbol_re):
    with open(filename, 'r') as f:
        for l in f:
            if re.search(symbol_re, l):
                return int(l.split()[0], base=16)
    return None


if __name__ == '__main__':

    parser = argparse.ArgumentParser(description='Primary control script for the live update process.')
    # parser.add_argument('-a', '--abc', help='no', default='question?')
    parser.add_argument('app_folder', help='Application folder')
    parser.add_argument('dev', help='Serial port')
    parser.add_argument('--baud', help='Baud rate', type=int, default=115200)
    parser.add_argument('--timeout', help='Timeout', type=int, default=None)
    args = parser.parse_args()

    # check made and flashed versions, warn to remake if necessary
    with open(args.app_folder + os.sep + '.lastVerNum.txt', 'r') as make_version_f:
        with open(args.app_folder + os.sep + BUILD_DIR + os.sep + 'lastFlashedNum.txt', 'r') as flash_version_f:
            make_partition = int(make_version_f.readline()[-1]) % 2
            flash_partition = int(flash_version_f.readline()[-1]) % 2
    
            if make_partition == flash_partition:
                print('Both flashed and currently compiled versions are linked to a conflicting partition, re-`make` app and rerun script')
                exit(1)

    with open(BUILD_DIR + os.sep + 'update.txt', 'r') as f:
        data = json.load(f)

        main_ptr_addr = int(data['main_ptr_addr'], base=16)
        update_flag_addr = int(data['update_flag_addr'], base=16)

    symbols = dump_symbols(args.app_folder)

    main_addr = get_symbol_address(symbols, '\smain$')

    appflash_text_start = get_symbol_address(symbols, 'appflash_text_start')
    if not appflash_text_start:
        print('Could not locate appflash_text_start, exiting...')
        exit(1)

    appflash_text_size = get_symbol_address(symbols, 'appflash_text_size')
    if not appflash_text_size:
        print('Could not locate appflash_text_size, exiting...')
        exit(1)

    appflash_text_rom_start = get_symbol_address(symbols, 'appflash_text_rom_start')
    if not appflash_text_start:
        print('Could not locate appflash_text_rom_start, exiting...')
        exit(1)

    appflash_rodata_start = get_symbol_address(symbols, 'appflash_rodata_start')
    appflash_rodata_size = get_symbol_address(symbols, 'appflash_rodata_size')
    appflash_rodata_rom_start = get_symbol_address(symbols, 'appflash_rodata_rom_start')

    if not appflash_rodata_start and not appflash_rodata_size and not appflash_rodata_rom_start: # just no rodata in update
        appflash_rodata_start = 0x0
        appflash_rodata_size = 0
        appflash_rodata_rom_start = 0x0
    elif not appflash_rodata_size or not appflash_rodata_start or not appflash_rodata_rom_start:
        print('Could not locate appflash_rodata_size or appflash_rodata_start or appflash_rodata_rom_start, exiting...')
        exit(1)

    appram_bss_start = get_symbol_address(symbols, 'appram_bss_start')
    appram_bss_size = get_symbol_address(symbols, 'appram_bss_size')
    appram_bss_start_addr = get_symbol_address(symbols, 'appram_bss_update_start')
    appram_bss_size_addr = get_symbol_address(symbols, 'appram_bss_update_size')

    if not appram_bss_start and not appram_bss_size:
        appram_bss_start = 0x0
        appram_bss_size = 0
    elif not appram_bss_start or not appram_bss_size:
        print('Could not locate appram_bss_start or appram_bss_size, exiting...')
        exit(1)

    # look at symbols for appflash_text_rom_start, appflash_rodata_rom_start and sizes for both/
    FLASH_BASE = 0x200000
    ih = IntelHex(args.app_folder + os.sep + BUILD_DIR + os.sep + get_app_name(args.app_folder) + '_merged.hex')

    app_text = bytes(ih[FLASH_BASE + appflash_text_rom_start: FLASH_BASE + appflash_text_rom_start + appflash_text_size].tobinarray())
    app_rodata = bytes(ih[FLASH_BASE + appflash_rodata_rom_start: FLASH_BASE + appflash_rodata_rom_start + appflash_rodata_size].tobinarray())

    CURRENT_VERSION = 0x6

    '''
    Format for transfer: {
        'version' : <char>
        'main_ptr_addr' : <u32>
        'main_ptr' : <u32>
        'update_flag' : <u32>
        'text_start' : <u32>
        'text_size' : <u32>
        'rodata_start' : <u32>
        'rodata_size' : <u32>
        'bss_start': <u32>
        'bss_size': <u32>
        'bss_start_addr': <u32>
        'bss_size_addr': <u32>
        'text' : <...>
        'rodata' : <...>
    }
    '''

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
        appram_bss_size_addr: {}
    }}
    """.format(
        CURRENT_VERSION,
        hex(main_ptr_addr),
        hex(main_addr | 0x1), # main ptr (w/ thumb)
        hex(update_flag_addr),
        hex(appflash_text_start),
        hex(appflash_text_size),
        hex(appflash_rodata_start),
        hex(appflash_rodata_size),
        hex(appram_bss_start),
        hex(appram_bss_size),
        hex(appram_bss_start_addr),
        hex(appram_bss_size_addr)
    )
    print(header_str)

    header = struct.pack('IIIIIIIIIIII',
        CURRENT_VERSION,
        main_ptr_addr,
        main_addr | 0x1, # main ptr (w/ thumb)
        update_flag_addr,
        appflash_text_start,
        appflash_text_size,
        appflash_rodata_start,
        appflash_rodata_size,
        appram_bss_start,
        appram_bss_size,
        appram_bss_start_addr,
        appram_bss_size_addr
    )

    with serial.Serial(args.dev, args.baud, timeout=2) as ser:

        print("sending header...")
        ser.write(header)
        time.sleep(1)
        print("sending app_text...")
        ser.write(app_text)
        time.sleep(1)
        print("sending rodata...")
        ser.write(app_rodata)
        time.sleep(1)

        #print("reading...")
        #data = ser.read(1024)
        #print(data.decode('utf-8'))

    print("updating last flashed version number...")
    os.system('cp ' + args.app_folder + os.sep + '.lastVerNum.txt ' + args.app_folder + os.sep + BUILD_DIR + os.sep + 'lastFlashedNum.txt')
    print('done')

