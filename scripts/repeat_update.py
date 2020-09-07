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

if __name__ == '__main__':

    parser = argparse.ArgumentParser(description='Repeat live update process.')
    parser.add_argument('app_folder', help='App folder')
    parser.add_argument('dev', help='Serial port')
    parser.add_argument('update_folder', help='Specify specific update package folder instead of deriving from app location', default=None)
    parser.add_argument('flashed_folder', help='Specify specific flashed package folder instead of deriving from app location', default=None)
    parser.add_argument('--dry_run', help='Does not attempt serial communication', action='store_true', default=False)
    parser.add_argument('--write_only', help='', action='store_true', default=False)
    parser.add_argument('--predicate_only', help='', action='store_true', default=False)
    parser.add_argument('--niter', help='Number of rounds', default=10)
    args = parser.parse_args()

    for i in range(int(args.niter)):
        print('---',i,'---')
        os.system('nrfjprog -s 960039321 -p')
        time.sleep(2)
        os.system('nrfjprog -s 682725125 -r')
        time.sleep(random.randint(2, 5) + random.random())

        s = 'python3 {} {} {} --update_folder {} --flashed_folder {} --no_update_flashed'.format(
            args.app_folder + '/../../scripts/update.py', # script
            args.app_folder,
            args.dev,
            args.update_folder,
            args.flashed_folder)
        if args.write_only:
            s += ' --write_only'
        if args.predicate_only:
            s += ' --predicate_only'
        if not args.dry_run:
            os.system(s)

        time.sleep(10)

    print('Cleaning up testing and exiting.')
    os.system('pkill -f JLink')

    os.system('nrfjprog -s 960039321 -p')
    time.sleep(2)
    os.system('nrfjprog -s 682725125 -r')
    time.sleep(random.randint(2, 5) + random.random())



