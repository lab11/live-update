import argparse
import serial
import os
import random
import struct
import sys
import time


if __name__ == '__main__':

    parser = argparse.ArgumentParser(description='Primary control script for the live update process.')
    # parser.add_argument('-a', '--abc', help='no', default='question?')
    parser.add_argument('dev', help='Serial port')
    parser.add_argument('--baud', help='Baud rate', type=int, default=115200)
    parser.add_argument('--timeout', help='Timeout', type=int, default=None)
    args = parser.parse_args()

    with serial.Serial(args.dev, args.baud, timeout=args.timeout) as ser:
        while True:
            tx = random.choice('abcdefghijklmnopqrstuvwxy')
            print('tx:', tx)
            ser.write(tx.encode('utf-8'))
            rx = ser.read()
            print('rx:', rx)

            time.sleep(0.25)
    

