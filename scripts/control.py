#!/usr/bin/env python3

import argparse
import serial
import struct
import sys

VERSION = 0x03
PAYLOAD_MAX = 0x400

dev = None

parser = argparse.ArgumentParser(description='Client utility for live updates')
parser.add_argument('-d', '--dev', dest=dev, action='store', help='path to tty device to use')
parser.add_argument('--ping', action='store_true', help='ping the device; debug noop')
parser.add_argument('--update', action='store_true', help='update a task')
parser.add_argument('--update-addr', type=lambda x: int(x, 0), action='store', help='base address at which to place updated code')
parser.add_argument('--update-file', action='store', help='path to binary file containing update')
# parser.add_argument('-t', '--task', type=int, required='--resume' in sys.argv or '--suspend' in sys.argv, help='index of task to modify')
args = parser.parse_args()

dev = args.dev
if dev is None:
    dev = '/dev/tty.usbmodem141302'

payload = ''
if args.ping:
    #                      fmt    version  cmd  len    val
    payload = struct.pack('HHHH', VERSION, 0x0, 0x1*2, 0x1)
elif args.update:
    with open(args.update_file, 'rb') as f:
        update_binary = f.read()

    payload = struct.pack('HHHxxi', VERSION, 0x1, 0x4*1 + len(update_binary), args.update_addr)
    payload += update_binary
else:
    print('No valid command specified!')
    parser.print_help()
    sys.exit(1)

ser = serial.Serial(dev, timeout=1, baudrate=115200)

while True:
    ser.write(payload)
    x = input()

