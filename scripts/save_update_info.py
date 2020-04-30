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

def get_app_name(folder):
    return os.path.basename(os.path.abspath(folder))


def dump_symbols(elf, dest):
    os.system('arm-none-eabi-objdump -t ' + elf + ' > ' + dest)


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

    build_dir = os.path.join(args.app_folder, '_build')

    elf_filename = os.path.join(build_dir, get_app_name(args.app_folder) + '_ns.elf')
    symbols = os.path.join(build_dir, 'flashed.symbols')
    dump_symbols(elf_filename, symbols)

    main_ptr_addr = get_symbol_address(symbols, 'main_ptr')
    if not main_ptr_addr:
        print('save_update_info.py: Could not locate main_ptr_addr, exiting...')
        exit(1)

    update_flag_addr = get_symbol_address(symbols, 'update_flag')
    if not update_flag_addr:
        print('save_update_info.py: Could not locate update_flag_addr, exiting...')
        exit(1)

    with open(os.path.join(build_dir, 'update.txt'), 'w') as f:
        json.dump({
                'main_ptr_addr': hex(main_ptr_addr),
                'update_flag_addr': hex(update_flag_addr)
            }, f)

