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

    parser = argparse.ArgumentParser(description='Save update var information')
    # parser.add_argument('-a', '--abc', help='no', default='question?')
    parser.add_argument('app_folder', help='Application folder')
    args = parser.parse_args()

    symbols = dump_symbols(args.app_folder)

    main_ptr_addr = get_symbol_address(symbols, 'main_ptr')
    if not main_ptr_addr:
        print('save_update_info.py: Could not locate main_ptr_addr, exiting...')
        exit(1)

    update_flag_addr = get_symbol_address(symbols, 'update_flag')
    if not update_flag_addr:
        print('save_update_info.py: Could not locate update_flag_addr, exiting...')
        exit(1)

    with open(BUILD_DIR + os.sep + 'update.txt', 'w') as f:
        json.dump({
                'main_ptr_addr': hex(main_ptr_addr),
                'update_flag_addr': hex(update_flag_addr)
            }, f)

