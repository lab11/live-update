#!/usr/bin/env python3

import argparse
import serial
import struct
import sys

dev = None

parser = argparse.ArgumentParser(description='Client utility for live updates')
parser.add_argument('-d', '--dev', dest=dev, action='store', help='path to tty device to use')
args = parser.parse_args()

dev = args.dev
if dev is None:
    dev = '/dev/tty.usbmodem141302'

ser = serial.Serial(dev, timeout = 2)

while True:
    res = ser.read(100)
    print(res.decode('utf-8'))
    sys.stdout.flush()

ser.close()
