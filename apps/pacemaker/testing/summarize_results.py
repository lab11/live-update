import argparse
from copy import deepcopy
import csv
from datetime import datetime
import json
import matplotlib.ticker as ticker
import matplotlib.pyplot as plt
import networkx as nx
import numpy as np
import os
import pprint
import random
import re
import serial
import struct
import sys
import time
from intelhex import IntelHex


def load_csv(f):
    lines = []
    with open(f, newline='') as csvfile:
        r = csv.reader(csvfile, delimiter=',')
        for row in r:
            if len(row) <= 0 or row[0][0] == '#':
                continue

            bad = False
            for s in [
                'SEGGER',
                'J-Link',
                'Process'
            ]:
                if row[0].startswith(s):
                    bad = True
                    break 
            if bad:
                continue

            lines.append(row)

    return lines


def gen_stats(data):

    return [len(data), np.min(data) / 1000, np.percentile(data, 25) / 1000, np.mean(data) / 1000, np.median(data) / 1000, np.percentile(data, 75) / 1000, np.max(data) / 1000]


def gen_figure(data):

    for k, v in data.items():
        data[k] = [i/1000 for i in v]

    filename = 'FIGOUT_' + datetime.now().strftime("%H:%M:%S") + '.png'
    #keys = list(reversed(['normal (All)', 'write only (All)', 'pred. only (All)', 'update (All)']))
    keys = list(reversed(['normal (All)', 'write only (All)', 'pred. only (All)', 'pred. only (After write)', 'update (All)', 'update (After write)']))

    medianprops = dict(linestyle='-', linewidth=1.5, color='black')
    fig1, ax1 = plt.subplots()
    bwplot_update = ax1.boxplot([data[k] for k in keys], vert=False, labels=keys,showfliers=True, medianprops=medianprops)
    #ax1.set_xticks([149500 + 100*i for i in range(1,10)])
    ax1.set_title('Pacemaker Response Time Distribution')
    ax1.set_label('Response Time (ms)')
    #ax1.set_xlim(148500, 151500)

    fig1.tight_layout()
    fig1.show()

    print('saving to', filename)
    fig1.savefig(filename, dpi=500)


if __name__ == '__main__':

    parser = argparse.ArgumentParser(description='Summary stats on result csvs.')
    parser.add_argument('--normal_csv', help='', default=None)
    parser.add_argument('--write_csv', help='', default=None)
    parser.add_argument('--predicate_csv', help='', default=None)
    parser.add_argument('--update_csv', help='', default=None)
    parser.add_argument('--hide_after_write', help='', action='store_true', default=False)
    args = parser.parse_args()

    results = {}
    fig_data = {}

    if args.normal_csv:
        normal_lines = load_csv(args.normal_csv)

        # trim lines before first start
        for i in range(len(normal_lines)):
            if normal_lines[i][0] == 'S':
                normal_lines = normal_lines[i:]
                break

        as_vp_response_times = []
        curr_idx = 0
        while (curr_idx < len(normal_lines)):
            if (normal_lines[curr_idx][0] == 'VP' and normal_lines[curr_idx-1][0] != 'AP'):
                as_vp_response_times.append(int(normal_lines[curr_idx][1]))
            curr_idx += 1

        results['normal (All)\t\t'] = gen_stats(as_vp_response_times)
        fig_data['normal (All)'] = as_vp_response_times

    if args.write_csv:
        write_lines = load_csv(args.write_csv)

        # trim lines before first start
        for i in range(len(write_lines)):
            if write_lines[i][0] == 'S':
                write_lines = write_lines[i:]
                break

        as_vp_response_times = []
        curr_idx = 0
        while (curr_idx < len(write_lines)):
            if (write_lines[curr_idx][0] == 'VP' and write_lines[curr_idx-1][0] != 'AP'):
                as_vp_response_times.append(int(write_lines[curr_idx][1]))
            curr_idx += 1

        results['write only (All)\t'] = gen_stats(as_vp_response_times)
        fig_data['write only (All)'] = as_vp_response_times

    if args.predicate_csv:
        predicate_lines = load_csv(args.predicate_csv)

        # trim lines before first start
        for i in range(len(predicate_lines)):
            if predicate_lines[i][0] == 'S':
                predicate_lines = predicate_lines[i:]
                break

        as_vp_response_times_all = []
        as_vp_response_times_after_write = []
        curr_idx = 0
        record_time = False 
        while (curr_idx < len(predicate_lines)):

            if (predicate_lines[curr_idx][0] == 'W'):
                record_time = True
            elif (predicate_lines[curr_idx][0] == 'S'):
                record_time = False 
            elif (predicate_lines[curr_idx][0] == 'VP' and predicate_lines[curr_idx-1][0] != 'AP'):
                as_vp_response_times_all.append(int(predicate_lines[curr_idx][1]))
                if record_time:
                    as_vp_response_times_after_write.append(int(predicate_lines[curr_idx][1]))

            curr_idx += 1

        if not args.hide_after_write:
            results['pred. only (After write)'] = gen_stats(as_vp_response_times_after_write)
        else:
            results['pred. only (All)\t'] = gen_stats(as_vp_response_times_all)
        fig_data['pred. only (All)'] = as_vp_response_times_all
        fig_data['pred. only (After write)'] = as_vp_response_times_after_write

    if args.update_csv:
        update_lines = load_csv(args.update_csv)

        # trim lines before first start
        for i in range(len(update_lines)):
            if update_lines[i][0] == 'S':
                update_lines = update_lines[i:]
                break

        as_vp_response_times_all = []
        as_vp_response_times_after_write = []
        curr_idx = 0
        record_time = False 
        while (curr_idx < len(update_lines)):

            if (update_lines[curr_idx][0] == 'W'):
                record_time = True
            elif (update_lines[curr_idx][0] == 'S'):
                record_time = False 
            elif (update_lines[curr_idx][0] == 'VP' and update_lines[curr_idx-1][0] != 'AP'):
                as_vp_response_times_all.append(int(update_lines[curr_idx][1]))
                if record_time:
                    as_vp_response_times_after_write.append(int(update_lines[curr_idx][1]))
            curr_idx += 1

        if not args.hide_after_write:
            results['update (After write)\t'] = gen_stats(as_vp_response_times_after_write)
        else:
            results['update (All)\t\t'] = gen_stats(as_vp_response_times_all)
        fig_data['update (All)'] = as_vp_response_times_all
        fig_data['update (After write)'] = as_vp_response_times_after_write

    gen_figure(fig_data)

    print()
    print()

    print('STATS (ms)\t\t\tN\tMin\tQ1\tMean\tMedian\tQ3\tMax')
    print('---------------------------------------------------------------------------------------')
    for r in results:
        stats = results[r]
        print('{}\t{}\t{:03.3f}\t{:03.3f}\t{:03.3f}\t{:03.3f}\t{:03.3f}\t{:03.3f}'.format(
            r,
            stats[0], stats[1], stats[2], stats[3], stats[4], stats[5], stats[6]
        ))

    print()
    
