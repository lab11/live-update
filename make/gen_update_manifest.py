import argparse
import serial
import os
import random
import re
import struct
import sys
import time
import json


FLASHED_SYMBOL_FILE = 'flashed.symbols'
UPDATE_SYMBOL_FILE = 'update.symbols'
UPDATE_AST_FILE = 'update_ast.txt'
UPDATE_ELF_FILE = 'update_ns.elf'
UPDATE_HEX_FILE = 'update.hex'


def gen_failed_manifest(msg):
    return {
        'valid': False,
        'reason': msg
    }


def get_version_number(filename):
    with open(filename, 'r') as f:
        return f.readlines()[0]


if __name__ == '__main__':

    parser = argparse.ArgumentParser(description='Generate update manifest for current build')
    # parser.add_argument('-a', '--abc', help='no', default='question?')
    parser.add_argument('dir', help='Update package directory')
    parser.add_argument('flashed_ver_file', help='File with flashed version')
    parser.add_argument('current_ver_file', help='File with current version')
    args = parser.parse_args()

    if not os.path.exists(args.flashed_ver_file):
        manifest = gen_failed_manifest('Previously flashed version number not found at {}, a version must be flashed to the board first.'.format(args.flashed_ver_file))

    else:
        flashed_version_number = get_version_number(args.flashed_ver_file)
        current_version_number = get_version_number(args.current_ver_file)        

        # For version number format X.Y.Z+A determine partition on value of A,
        # offset by 1 because Make system is looking at previous version to
        # determine current partition
        flashed_partition = (int(flashed_version_number.split('+')[1]) + 1) % 2
        current_partition = (int(current_version_number.split('+')[1]) + 1) % 2

        if flashed_partition == current_partition:
            manifest = gen_failed_manifest('Current version partition ({} for version {}) conflicts with flashed version partition({} for version {}), re-run `make` to resolve to different partition.'.format(current_partition, current_version_number, flashed_partition, flashed_version_number))

        elif not os.path.exists(os.path.join(args.dir, FLASHED_SYMBOL_FILE)):
            manifest = gen_failed_manifest('Could not file update payload file ' + os.path.join(args.dir, FLASHED_SYMBOL_FILE))
        elif not os.path.exists(os.path.join(args.dir, UPDATE_SYMBOL_FILE)):
            manifest = gen_failed_manifest('Could not file update payload file ' + os.path.join(args.dir, UPDATE_SYMBOL_FILE))
        elif not os.path.exists(os.path.join(args.dir, UPDATE_AST_FILE)):
            manifest = gen_failed_manifest('Could not file update payload file ' + os.path.join(args.dir, UPDATE_AST_FILE))
        elif not os.path.exists(os.path.join(args.dir, UPDATE_ELF_FILE)):
            manifest = gen_failed_manifest('Could not file update payload file ' + os.path.join(args.dir, UPDATE_ELF_FILE))
        else:
            manifest = {
                'valid': True,
                'flashed_version': flashed_version_number,
                'flashed_partition': flashed_partition,
                'flashed_symbols': FLASHED_SYMBOL_FILE,
                'update_version': current_version_number,
                'update_partition': current_partition,
                'update_symbols': UPDATE_SYMBOL_FILE,
                'update_ast': UPDATE_AST_FILE,
                'update_elf': UPDATE_ELF_FILE,
                'update_hex': UPDATE_HEX_FILE
            }

    print(json.dumps(manifest, sort_keys=False, indent=4))
            
