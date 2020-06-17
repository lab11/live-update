import argparse
import os
import sys

parser = argparse.ArgumentParser(description='Bump build version number')
parser.add_argument('version_file', help='Path to version file')

args = parser.parse_args()

current_version = 0
if os.path.exists(args.version_file):
    with open(args.version_file, 'r') as f:
        current_version = int(f.readlines()[0])

new_version = current_version + 1

with open(args.version_file, 'w') as f:
    f.write(str(new_version)) 

