import argparse
import serial
import os
import random
import re
import struct
import sys
import time
import json


if __name__ == '__main__':

    parser = argparse.ArgumentParser(description='Repeated update test script.')
    parser.add_argument('app_folder', help='Update folder 1 (first applied)')
    parser.add_argument('update_folder1', help='Update folder 1 (first applied)')
    parser.add_argument('update_folder2', help='Update folder 2 (second applied)')
    parser.add_argument('dev', help='Serial port')
    args = parser.parse_args()

    update_ctr = 0

    while True:
        # wait random amount of time before doing it again
        delay = random.randint(3, 15)
        print('waiting ' + str(delay) + ' seconds...', end='', flush=True)
        time.sleep(delay)
        print('done.')
        print()
        
        if update_ctr % 2 == 0:
            update_folder = args.update_folder1   
        else:
            update_folder = args.update_folder2

        # update
        base_folder = os.path.join(args.app_folder, '..', '..')
        command = 'python3 {} {} {} --update_folder {}'.format(
            os.path.join(base_folder, 'scripts', 'update.py'),
            args.app_folder,
            args.dev,
            update_folder
        )
            
        print('>> ' + command)
        os.system(command)

        update_ctr += 1

