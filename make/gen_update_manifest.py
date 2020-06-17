import argparse
import serial
import os
import random
import re
import struct
import sys
import time
import json


UPDATE_SYMBOL_FILE = 'update.symbols'
UPDATE_DATA_FILE = 'update.data'
UPDATE_ELF_FILE = 'update_ns.elf'
UPDATE_HEX_FILE = 'update.hex'
UPDATE_ANALYSIS_FILE = 'analysis.json'
UPDATE_GRAPH_FILE = 'update.graph'


def gen_failed_manifest(msg):
    return {
        'valid': False,
        'reason': msg
    }


def get_version_number(filename):
    with open(filename, 'r') as f:
        return int(f.readlines()[0])


if __name__ == '__main__':

    parser = argparse.ArgumentParser(description='Generate update manifest for current build')
    parser.add_argument('dir', help='Update package directory')
    parser.add_argument('current_ver_file', help='File with current version')
    args = parser.parse_args()

    current_version_number = get_version_number(args.current_ver_file)        
    current_partition = (current_version_number + 1) % 2

    if not os.path.exists(os.path.join(args.dir, UPDATE_SYMBOL_FILE)):
        manifest = gen_failed_manifest('Could not file update payload file ' + os.path.join(args.dir, UPDATE_SYMBOL_FILE))
    elif not os.path.exists(os.path.join(args.dir, UPDATE_ELF_FILE)):
        manifest = gen_failed_manifest('Could not file update payload file ' + os.path.join(args.dir, UPDATE_ELF_FILE))
    elif not os.path.exists(os.path.join(args.dir, UPDATE_DATA_FILE)):
        manifest = gen_failed_manifest('Could not file update payload file ' + os.path.join(args.dir, UPDATE_DATA_FILE))
    elif not os.path.exists(os.path.join(args.dir, UPDATE_ANALYSIS_FILE)):
        manifest = gen_failed_manifest('Could not file update payload file ' + os.path.join(args.dir, UPDATE_ANALYSIS_FILE))
    elif not os.path.exists(os.path.join(args.dir, UPDATE_GRAPH_FILE)):
        manifest = gen_failed_manifest('Could not file update graph file ' + os.path.join(args.dir, UPDATE_GRAPH_FILE))
    else:
        manifest = {
            'valid': True,
            'update_version': current_version_number,
            'update_partition': current_partition,
            'update_symbols': UPDATE_SYMBOL_FILE,
            'update_data': UPDATE_DATA_FILE,
            'update_elf': UPDATE_ELF_FILE,
            'update_hex': UPDATE_HEX_FILE,
            'update_analysis': UPDATE_ANALYSIS_FILE,
            'update_graph': UPDATE_GRAPH_FILE,
        }

    print(json.dumps(manifest, sort_keys=False, indent=4))
            
