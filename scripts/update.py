import argparse
import serial
import os
import random
import re
import struct
import sys
import time

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

    symbols = dump_symbols(args.app_folder)

    main_ptr_addr = get_symbol_address(symbols, 'main_ptr')
    if not main_ptr_addr:
        print('Could not locate main_ptr_addr, exiting...')
        exit(1)

    update_flag_addr = get_symbol_address(symbols, 'update_flag')
    if not update_flag_addr:
        print('Could not locate update_flag_addr, exiting...')
        exit(1)
    
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

    update_text_start_addr = get_symbol_address(symbols, 'appflash_text_update_start')
    if not update_text_start_addr:
        print('Could not locate update_text_start_addr, exiting...')
        exit(1)

    update_text_rom_start_addr = get_symbol_address(symbols, 'appflash_text_update_rom_start')
    if not update_text_rom_start_addr:
        print('Could not locate update_text_rom_start_addr, exiting...')
        exit(1)

    update_text_size_addr = get_symbol_address(symbols, 'appflash_text_update_size')
    if not update_text_size_addr:
        print('Could not locate update_text_size_addr, exiting...')
        exit(1)

    appflash_rodata_start = get_symbol_address(symbols, 'appflash_rodata_start')
    if not appflash_rodata_start:
        print('Could not locate appflash_rodata_start, exiting...')
        exit(1)

    appflash_rodata_size = get_symbol_address(symbols, 'appflash_rodata_size')
    if not appflash_rodata_size:
        print('Could not locate appflash_rodata_size, exiting...')
        exit(1)

    appflash_rodata_rom_start = get_symbol_address(symbols, 'appflash_rodata_rom_start')
    if not appflash_rodata_rom_start:
        print('Could not locate appflash_rodata_rom_start, exiting...')
        exit(1)

    update_rodata_start_addr = get_symbol_address(symbols, 'appflash_rodata_update_start')
    if not update_rodata_start_addr:
        print('Could not locate update_rodata, exiting...')
        exit(1)

    update_rodata_rom_start_addr = get_symbol_address(symbols, 'appflash_rodata_update_rom_start')
    if not update_rodata_rom_start_addr:
        print('Could not locate update_rodata_rom_start_addr, exiting...')
        exit(1)

    update_rodata_size_addr = get_symbol_address(symbols, 'appflash_rodata_update_size')
    if not update_rodata_size_addr:
        print('Could not locate update_rodata_size_addr, exiting...')
        exit(1)

    # look at symbols for appflash_text_rom_start, appflash_rodata_rom_start and sizes for both/
    FLASH_BASE = 0x200000
    ih = IntelHex(args.app_folder + os.sep + BUILD_DIR + os.sep + get_app_name(args.app_folder) + '_merged.hex')

    app_text = bytes(ih[FLASH_BASE + appflash_text_rom_start: FLASH_BASE + appflash_text_rom_start + appflash_text_size].tobinarray())
    app_rodata = bytes(ih[FLASH_BASE + appflash_rodata_rom_start: FLASH_BASE + appflash_rodata_rom_start + appflash_rodata_size].tobinarray())

    CURRENT_VERSION = 0x5
    '''
    Format for transfer: {
        'version' : <char>
        'main_ptr_addr' : <u32>
        'main_ptr' : <u32>
        'update_flag' : <u32>
        'text_start' : <u32>
        'text_size' : <u32>
        // 'update_text_start_addr' : <u32>
        // 'update_text_rom_start_addr' : <u32>
        // 'update_text_size_addr' : <u32>
        'rodata_start' : <u32>
        'rodata_size' : <u32>
        // 'update_rodata_start_addr' : <u32>
        // 'update_rodata_rom_start_addr' : <u32>
        // 'update_rodata_size_addr' : <u32>
        'text' : <...>
        'rodata' : <...>
    }
    '''

    header = struct.pack('IIIIIIII',
        CURRENT_VERSION,
        main_ptr_addr,
        appflash_text_start | 0x1, # main ptr (w/ thumb)
        update_flag_addr,
        appflash_text_start,
        appflash_text_size,
        #update_text_start_addr,
        #update_text_rom_start_addr,
        #update_text_size_addr,
        appflash_rodata_start,
        appflash_rodata_size,
        #update_rodata_start_addr,
        #update_rodata_rom_start_addr,
        #update_rodata_size_addr, 
    )

    with serial.Serial(args.dev, args.baud, timeout=2) as ser:
        while True:
            data = ser.read(1024).decode('utf-8')
            if not data:
                break
            print(data)

        #print('sending {} byte payload:\n\tversion: {}\n\tmain_ptr_addr: {}\n\tmain_ptr: {}\n\tupdate_flag: {}\n\tflash_text_start: {}\n\tflash_text_size: {}\n\tflash_rodata_start: {}\n\tflash_rodata_size: {}\n\tapp_text: {} bytes\n\tapp_rodata: {} bytes\n'.format(len(header) + len(app_text) + len(app_rodata), CURRENT_VERSION, hex(main_ptr_addr), hex(appflash_text_start | 0x1), hex(update_flag_addr), hex(appflash_text_start), hex(appflash_text_size), hex(appflash_rodata_start), hex(appflash_rodata_size), len(app_text), len(app_rodata)))

        ser.write(header)
        time.sleep(0.5)
        ser.write(app_text)
        time.sleep(0.5)
        ser.write(app_rodata)
        time.sleep(0.5)

        while True:
            data = ser.read(1024).decode('utf-8')
            if not data:
                break
            print(data)

    print('done')

