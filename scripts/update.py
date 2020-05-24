import argparse
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

CURRENT_VERSION = 10
FLASH_BASE = 0x200000

FLASHED_VERSION_NAME = 'lastFlashedNum.txt'
FLASHED_SYMBOLS_NAME = 'flashed.symbols'

# -- Utility functions --

def get_app_name(folder):
    return os.path.basename(os.path.abspath(folder))


def search_symbols(filename, pattern, cb):
    with open(filename, 'r') as f:
        for l in f:
            if re.search(pattern, l):
                cb(l)


def get_symbols_in_section(filename, section_re):
    symbols = []
    def cb(l):
        nonlocal symbols
        symbols.append(l.split()[-1])
    search_symbols(filename, section_re, cb)
    return symbols


def get_symbol_address(filename, symbol_re):
    addr = None
    def cb(l):
        nonlocal addr
        addr = int(l.split()[0], base=16)
    search_symbols(filename, symbol_re, cb)
    return addr


def get_symbol_size(filename, symbol_re):
    size = 0
    def cb(l):
        nonlocal size
        size = int(l.split()[-2], base=16)
    search_symbols(filename, symbol_re, cb)
    return size


def valid_partition(app_folder, manifest):
    flashed_version_filename = os.path.join(app_folder, '_build', FLASHED_VERSION_NAME)

    if not os.path.exists(flashed_version_filename):
        print('Flashed version number not found at', flashed_version_filename)
        return False

    with open(flashed_version_filename, 'r') as f:
        flashed_version_number = f.readlines()[0]

    flashed_partition = (int(flashed_version_number.split('+')[1]) + 1) % 2

    if flashed_partition == manifest['update_partition']:
        print('Current version partition ({} for version {}) conflicts with flashed version partition ({} for version {}), re-run `make` to resolve to different partition.'.format(manifest['update_partition'], manifest['update_version'], flashed_partition, flashed_version_number))
        return False

    return True


def generate_update_header(manifest, app_folder, update_folder, predicates, transfers):

    header = {}

    flashed_symbols = os.path.join(app_folder, '_build', FLASHED_SYMBOLS_NAME)
    update_symbols = os.path.join(update_folder, manifest['update_symbols'])

    main_ptr_addr = get_symbol_address(flashed_symbols, 'rodata.*main_ptr')
    if not main_ptr_addr:
        print('Could not locate main_ptr_addr, exiting...')
        exit(1)
    header['main_ptr_addr'] = main_ptr_addr

    update_flag_addr = get_symbol_address(flashed_symbols, 'update_flag')
    if not update_flag_addr:
        print('Could not locate update_flag_addr, exiting...')
        exit(1)
    header['update_flag_addr'] = update_flag_addr

    header['main_addr'] = get_symbol_address(update_symbols, '\smain$')

    header['appflash_text_start'] = get_symbol_address(update_symbols, 'appflash_text_start')
    if not header['appflash_text_start']:
        print('Could not locate appflash_text_start, exiting...')
        exit(1)

    header['appflash_text_size'] = get_symbol_address(update_symbols, 'appflash_text_size')
    if not header['appflash_text_size']:
        print('Could not locate appflash_text_size, exiting...')
        exit(1)

    header['appflash_text_rom_start'] = get_symbol_address(update_symbols, 'appflash_text_rom_start')
    if not header['appflash_text_rom_start']:
        print('Could not locate appflash_text_rom_start, exiting...')
        exit(1)

    header['appflash_rodata_start'] = get_symbol_address(update_symbols, 'appflash_rodata_start')
    header['appflash_rodata_size'] = get_symbol_address(update_symbols, 'appflash_rodata_size')
    header['appflash_rodata_rom_start'] = get_symbol_address(update_symbols, 'appflash_rodata_rom_start')

    if not header['appflash_rodata_start'] and \
       not header['appflash_rodata_size'] and \
       not header['appflash_rodata_rom_start']: # there's just no rodata in update, that's ok
        header['appflash_rodata_start'] = 0x0
        header['appflash_rodata_size'] = 0
        header['appflash_rodata_rom_start'] = 0x0
    elif not header['appflash_rodata_size'] or \
         not header['appflash_rodata_start'] or \
         not header['appflash_rodata_rom_start']:
        print('Could not locate appflash_rodata_size or appflash_rodata_start or appflash_rodata_rom_start, exiting...')
        exit(1)

    header['appram_bss_start'] = get_symbol_address(update_symbols, 'appram_bss_start')
    header['appram_bss_size'] = get_symbol_address(update_symbols, 'appram_bss_size')
    header['appram_bss_start_addr'] = get_symbol_address(update_symbols, 'appram_bss_update_start')
    header['appram_bss_size_addr'] = get_symbol_address(update_symbols, 'appram_bss_update_size')

    if not header['appram_bss_start'] and not header['appram_bss_size']:
        header['appram_bss_start'] = 0x0
        header['appram_bss_size'] = 0
        header['appram_bss_start_addr'] = 0x0
        header['appram_bss_size_addr'] = 0x0
    elif not header['appram_bss_start'] or not header['appram_bss_size']:
        print('Could not locate appram_bss_start or appram_bss_size but did locate the other, exiting...')
        exit(1)

    # event_handler_addr, gpio_interrupt_enabled, gpio_out_enabled, and gpio_out_set have fixed
    # size of 4 bytes each per predicate, so calculate that here
    predicates_size = (4 * 4) * len(predicates)

    # every predicate gets a size when serialized
    predicates_size += 4 * len(predicates)
    for p in predicates:
        predicates_size += 4 # num memory checks
        for m in p['memory_checks']:
            _, size, _ = m
            predicates_size += (2 * 4) + size

        predicates_size += 4 # num active timers
        predicates_size += (3 * 4) * len(p['active_timers'])

        predicates_size += 4 # num interrupt cbs
        predicates_size += (2 * 4) * len(p['gpio_interrupt_callbacks'])

    header['predicates_size'] = predicates_size
    header['n_predicates'] = len(predicates)

    # event_handler_addr, gpio_interrupt_enabled, gpio_out_enabled, and gpio_out_set have
    # fixed size of 4 bytes each per transfer, so calculate that here
    transfers_size = (4 * 4) * len(transfers)

    # every transfer gets a size when serialized
    transfers_size += 4 * len(transfers)
    for t in transfers:
        transfers_size += 4 # number memory items
        transfers_size += (3 * 4) * len(t['memory'])

        transfers_size += 4 # number init items
        for i in t['init_memory']:
            _, size, _ = i
            transfers_size += (2 * 4) + size
            
        transfers_size += 4 # number timer items
        transfers_size += (3 * 4) * len(t['timers'])

        transfers_size += 4 # number active timer items
        transfers_size += (3 * 4) * len(t['active_timers'])

        transfers_size += 4 # number gpio interrupt cbs
        transfers_size += (2 * 4) * len(t['gpio_interrupt_callbacks'])

    header['transfers_size'] = transfers_size

    return header


def generate_predicate(manifest, update_folder, flashed_symbols, point):

    predicate = {
        'memory_checks': [], # (address, size, value bytes) triples
        'active_timers': [], # (timer base address, duration, period) triples
        'gpio_interrupt_enabled': 0, # bit vector for gpio interrupts
        'gpio_interrupt_callbacks': [], # (pin, address) triples
        'gpio_out_enabled': 0, # bit vector for gpio output enabled
        'gpio_out_set': 0, # bit vector for gpio output set value
        'event_handler_addr': 0, # address of handler for predicate event
    }

    expected_mem, expected_timers, expected_gpio = point['src_state_f']
    # fix duplicates in active timers
    expected_timers['active'] = list(set(expected_timers['active']))

    predicate['event_handler_addr'] = get_symbol_address(flashed_symbols, point['e_event_f']['event'] + '$')

    for var in expected_mem:
        addr = get_symbol_address(flashed_symbols, var+'$')
        size = get_symbol_size(flashed_symbols, var+'$')
        value = expected_mem[var].to_bytes(size, byteorder='little')

        predicate['memory_checks'].append((addr, size, value))

    for active_timer in expected_timers['active']:

        duration = expected_timers['timers'][active_timer]['duration_ms']
        period = expected_timers['timers'][active_timer]['period_ms']

        # strip the leading & if necessary to get address
        if active_timer[0] == '&':
            active_timer = active_timer[1:]
        addr = get_symbol_address(flashed_symbols, active_timer+'$')

        predicate['active_timers'].append((addr, duration, period))

    predicate['gpio_interrupt_enabled'] = expected_gpio['interrupt_enabled']
    predicate['gpio_out_enabled'] = expected_gpio['output_enabled']
    predicate['gpio_out_set'] = expected_gpio['set']

    for pin, cb in expected_gpio['interrupts'].items():
        addr = get_symbol_address(flashed_symbols, cb+'$')
        predicate['gpio_interrupt_callbacks'].append((pin, addr))

    return predicate


def generate_update_predicates(manifest, app_folder, update_folder, update_points):

    predicates = []

    flashed_symbols = os.path.join(app_folder, '_build', FLASHED_SYMBOLS_NAME)

    for p in update_points:
        predicates.append(generate_predicate(manifest, update_folder, flashed_symbols, p))

    return predicates


def generate_transfer(manifest, flashed_symbols, update_symbols, point):

    transfer = {
        'memory': [], # (address, dest, size) triples
        'init_memory': [], # (address, size, val bytes) triples
        'timers': [], # (base address, expire cb address, stop cb address) triples 
        'event_handler_addr': None, # new expire/interrupt cb for event, else None
        'active_timers': [], # base addresses for each active timer in updated version
        'gpio_interrupt_enabled': 0, # bit vector for gpio interrupts
        'gpio_interrupt_callbacks': [], # (pin, address) pairs
        'gpio_out_enabled': 0, # bit vector for gpio output enabled
        'gpio_out_set': 0, # bit vector for gpio output set value
    }

    expected_mem, expected_timers, expected_gpio = point['src_state_f']
    # fix duplicates in active timers
    expected_timers['active'] = list(set(expected_timers['active']))

    update_mem, update_timers, update_gpio = point['src_state_u']
    # fix duplicates in active timers
    update_timers['active'] = list(set(update_timers['active']))

    for var in update_mem:
        dest = get_symbol_address(update_symbols, var+'$')
        size = get_symbol_size(update_symbols, var+'$')

        if var in expected_mem:
            addr = get_symbol_address(flashed_symbols, var+'$')
            transfer['memory'].append((addr, dest, size))
        else:
            val = update_mem[var].to_bytes(size, byteorder='little') 
            transfer['init_memory'].append((dest, size, val))

    for timer in update_timers['timers']:

        active = True if timer in update_timers['active'] else False

        expire = None
        expiry_fn = update_timers['timers'][timer]['expiry_fn']
        if expiry_fn:
            expire = get_symbol_address(update_symbols, expiry_fn+'$')

        stop = None
        if update_timers['timers'][timer]['stop_fn']:
            stop = get_symbol_address(update_symbols, update_timers['timers'][timer]['stop_fn']+'$')

        # strip the leading & if necessary to get address
        if timer[0] == '&':
            timer = timer[1:]
        addr = get_symbol_address(update_symbols, timer+'$')

        transfer['timers'].append((addr, expire, stop))

        if active:
            duration = update_timers['timers'][timer]['duration_ms']
            period = update_timers['timers'][timer]['period_ms']
            transfer['active_timers'].append((addr, duration, period))

        # this timer expiry is the update trigger for this update point
        if expiry_fn == point['e_event_u']['event']:
            transfer['event_handler_addr'] = expire

    transfer['gpio_interrupt_enabled'] = update_gpio['interrupt_enabled']

    for pin, cb in update_gpio['interrupts'].items():
        addr = get_symbol_address(update_symbols, cb+'$')
        transfer['gpio_interrupt_callbacks'].append((pin, addr))

        if cb == point['e_event_u']['event']:
            transfer['event_handler_addr'] = addr

    transfer['gpio_out_enabled'] = update_gpio['output_enabled']
    transfer['gpio_out_set'] = update_gpio['set']

    return transfer


def generate_update_transfers(manifest, app_folder, update_folder, update_points):

    transfers = []

    flashed_symbols = os.path.join(app_folder, '_build', FLASHED_SYMBOLS_NAME)
    update_symbols = os.path.join(update_folder, manifest['update_symbols'])

    for p in update_points:
        transfers.append(generate_transfer(manifest, flashed_symbols, update_symbols, p))

    return transfers


def generate_update_payloads(header, manifest, app_folder, update_folder, predicates, transfers):

    payloads = {}

    # look at symbols for appflash_text_rom_start, appflash_rodata_rom_start and sizes for both
    hex_filename = os.path.join(update_folder, manifest['update_hex'])
    ih = IntelHex(hex_filename)

    # extract .text segment
    payloads['text'] = bytes(
        ih[FLASH_BASE + header['appflash_text_rom_start']: FLASH_BASE + header['appflash_text_rom_start'] + header['appflash_text_size']].tobinarray())

    # extract .rodata segment
    payloads['rodata'] = bytes(
        ih[FLASH_BASE + header['appflash_rodata_rom_start']: FLASH_BASE + header['appflash_rodata_rom_start'] + header['appflash_rodata_size']].tobinarray())

    # generate state transfer information
    flashed_symbols = os.path.join(app_folder, '_build', FLASHED_SYMBOLS_NAME)
    update_symbols = os.path.join(update_folder, manifest['update_symbols'])

    flashed_ram = get_symbol_address(flashed_symbols, '_appram_mpu_ro_region_start')
    update_ram = get_symbol_address(update_symbols, '_appram_mpu_ro_region_start')

    if flashed_ram == update_ram:
        print('Error: flashed image and update image both using the same RAM section: {} (flashed) {} (update)'.format(hex(flashed_ram), hex(update_ram)))
        exit(1)


    '''
    predicate = {
        overall size
        event_handler_addr
        len(memory_checks)
        len(active_timers)
        len(gpio_interrupt_cbs)
        'gpio_interrupt_enabled': 0, # bit vector for gpio interrupts
        'gpio_out_enabled': 0, # bit vector for gpio output enabled
        'gpio_out_set': 0, # bit vector for gpio output set value

        'memory_checks': [], # (address, size, value bytes) triples
        'active_timers': [], # (timer base address, duration, period) triples
        'gpio_interrupt_callbacks': [], # (pin, address) pairs
    }
    '''
    def serialize_predicate(p):

        p_bytes = struct.pack('IIII', p['event_handler_addr'], len(p['memory_checks']), len(p['active_timers']), len(p['gpio_interrupt_callbacks']))
        p_bytes += struct.pack('III', p['gpio_interrupt_enabled'], p['gpio_out_enabled'], p['gpio_out_set'])

        for m in p['memory_checks']:
            addr, size, val_bytes = m
            p_bytes += struct.pack('II', addr, size) + val_bytes

        for a in p['active_timers']:
            addr, duration, period = a
            p_bytes += struct.pack('III', addr, duration, period)

        for c in p['gpio_interrupt_callbacks']:
            pin, address = c
            p_bytes += struct.pack('II', int(pin), address)

        return struct.pack('I', len(p_bytes) + 4) + p_bytes

    payloads['predicates'] = b''
    for p in predicates:
        payloads['predicates'] += serialize_predicate(p)


    '''
    transfer = {
        overall size
        'event_handler_addr': None, # new event handler for event, else None
        len(memory)
        len(init_memory)
        len(timers)
        len(active_timers)
        len(gpio_interrupt_cbs)
        'gpio_interrupt_enabled': 0, # bit vector for gpio interrupts
        'gpio_out_enabled': 0, # bit vector for gpio output enabled
        'gpio_out_set': 0, # bit vector for gpio output set value
        'memory': [], # (address, dest, size) triples
        'init_memory': [], # (address, size, val bytes) triples
        'timers': [], # (base address, expire cb address, stop cb address) triples 
        'active_timers': [], # (base address, duration, period) triples for each active timer in updated version
        'gpio_interrupt_callbacks': [], # (pin, address) pairs
    }
    '''
    def serialize_transfer(t):

        t_bytes = struct.pack('IIIIII',
                t['event_handler_addr'] if t['event_handler_addr'] else 0,
                len(t['memory']), len(t['init_memory']), len(t['timers']), len(t['active_timers']), len(t['gpio_interrupt_callbacks']))
        t_bytes += struct.pack('III',
                t['gpio_interrupt_enabled'], t['gpio_out_enabled'], t['gpio_out_set'])
        
        for m in t['memory']:
            addr, dest, size = m
            t_bytes += struct.pack('III', addr, dest, size)

        for i in t['init_memory']:
            addr, size, val_bytes = i
            t_bytes += struct.pack('II', addr, size) + val_bytes

        for timer in t['timers']:
            base, expire, stop = timer
            t_bytes += struct.pack('III', base, expire if expire else 0, stop if stop else 0)

        for active_timer in t['active_timers']:
            addr, duration, period = active_timer
            t_bytes += struct.pack('III', addr, duration, period)

        for cb in t['gpio_interrupt_callbacks']:
            pin, addr = cb
            t_bytes += struct.pack('II', int(pin), addr)

        return struct.pack('I', len(t_bytes) + 4) + t_bytes


    payloads['transfers'] = b''
    for t in transfers:
        payloads['transfers'] += serialize_transfer(t)

    return payloads


def serialize_header(header, payloads):

    # Sanity check print header
    header_str = """
    header = {{
        version: {},
        main_ptr_addr: {},
        main_addr: {},
        update_flag_addr: {},
        appflash_text_start: {},
        appflash_text_size: {},
        appflash_rodata_start: {},
        appflash_rodata_size: {},
        appram_bss_start: {},
        appram_bss_size: {},
        appram_bss_start_addr: {},
        appram_bss_size_addr: {},
        n_predicates: {},
        predicates_size: {},
        transfers_size: {}
    }}
    total size (expected): {}
    total size (actual): {}
    """.format(
        CURRENT_VERSION,
        hex(header['main_ptr_addr']),
        hex(header['main_addr'] | 0x1),
        hex(header['update_flag_addr']),
        hex(header['appflash_text_start']),
        hex(header['appflash_text_size']),
        hex(header['appflash_rodata_start']),
        hex(header['appflash_rodata_size']),
        hex(header['appram_bss_start']),
        hex(header['appram_bss_size']),
        hex(header['appram_bss_start_addr']),
        hex(header['appram_bss_size_addr']),
        hex(header['n_predicates']),
        hex(header['predicates_size']),
        hex(header['transfers_size']),
        4 * 15 + header['appflash_text_size'] + header['appflash_rodata_size'] + header['predicates_size'] + header['transfers_size'],
        4 * 15 + sum(len(payloads[x]) for x in payloads)
    )
    print(header_str)

    return struct.pack('I' * 15,
        CURRENT_VERSION,
        header['main_ptr_addr'],
        header['main_addr'] | 0x1, # make sure main ptr is thumb!
        header['update_flag_addr'],
        header['appflash_text_start'],
        header['appflash_text_size'],
        header['appflash_rodata_start'],
        header['appflash_rodata_size'],
        header['appram_bss_start'],
        header['appram_bss_size'],
        header['appram_bss_start_addr'],
        header['appram_bss_size_addr'],
        header['n_predicates'],
        header['predicates_size'],
        header['transfers_size'],
    )


def is_updatable_event(G_u, src_state_u, dst_state_u, e_event_u, start_state_u, src_state_f, dst_state_f, e_event_f):

    event_f, event_u = e_event_f['event'], e_event_u['event']
    same_event = False

    # Check event is the same timer expiry
    for t in src_state_f[1]['timers']:
        if src_state_f[1]['timers'][t]['expiry_fn'] == event_f:
            for t_u in src_state_u[1]['timers']:
                if t_u == t and src_state_u[1]['timers'][t_u]['expiry_fn'] == event_u:
                    same_event = True
                    break

    # Check event is the same GPIO interrupt
    for i in src_state_f[2]['interrupts']:
        if event_f == src_state_f[2]['interrupts'][i] and \
            i in src_state_u[2]['interrupts'] and \
            event_u == src_state_u[2]['interrupts'][i]:
            same_event = True
            break

    if not same_event:
        return False

    # Memory: if known, same name needs same values, else needs init values of update
    src_mem_u = src_state_u[0]
    src_mem_f = src_state_f[0]

    for data in src_mem_u:
        if data in src_mem_f and src_mem_u[data] != src_mem_f[data]:
            return False # skip edge, data doesn't match between flashed and updated
        elif data not in src_mem_f and src_mem_u[data] != start_state_u[data]:
            return False # skip edge, data doesn't match updated initial state

    # Timers:
    
    src_timers_u = src_state_u[1]
    dst_timers_u = dst_state_u[1]

    src_timers_f = src_state_f[1]
    dst_timers_f = dst_state_f[1]

    # if a timer is active in the flashed version, and exists by name in the
    # update, look for a state where it's also active in the update
    for src_active_t_f in src_timers_f['active']:
        if src_active_t_f in src_timers_u['timers'] and src_active_t_f not in src_timers_u['active']:
            return False    

    # if a timer is active in the updated version, and exists by name in the
    # flash, check it's also active in the flash
    for src_active_t_u in src_timers_u['active']:
        if src_active_t_u in src_timers_f['timers'] and src_active_t_u not in src_timers_f['active']:
            return False    

    # can update if no timers are currently running
    timers_quiescent = len(src_timers_f) == 0 and len(dst_timers_f) == 0 and len(src_timers_u) == 0

    # can update if all active timers reset by event in both flashed and updated versions
    #    intersection of {update,flashed} {src,dst} active timers is 0
    timers_reset = True
    for src_active_t_f in src_timers_f['active']:
        if src_active_t_f in dst_timers_f['active'] and src_active_t_f not in e_event_f['reset']:
            timers_reset = False
            break

    for src_active_t_u in src_timers_u['active']:
        if src_active_t_u in dst_timers_u['active'] and src_active_t_u not in e_event_u['reset']:
            timers_reset = False
            break

    if not (timers_quiescent or timers_reset):
        return False

    # GPIO:
    src_gpio_u = src_state_u[2]
    src_gpio_f = src_state_f[2]

    # output pins need to be the same
    if src_gpio_f['set'] != src_gpio_u['set']:
        return False

    return True


def find_matching_update_event(G_u, src_state_f, dst_state_f, e_event_f):

    # Find update main, get initial state vals
    start_state_u = None
    for e in G_u.edges():
        dst_state_u = json.loads(e[1])
        e_events_u = G_u.get_edge_data(e[0], e[1])['label']

        for event in e_events_u:
            if event['event'] == 'main':
                start_state_u = dst_state_u
                break

        if start_state_u:
            break

    if not start_state_u:
        print('Couldn\'t find update initial state, exiting')
        exit(1)

    potential_events = [] 
    for e_u in G_u.edges():

        src_state_u, dst_state_u = json.loads(e_u[0]), json.loads(e_u[1])
        e_events_u = G_u.get_edge_data(e_u[0], e_u[1])['label']

        for e_event_u in e_events_u:
            if (is_updatable_event(G_u, \
                    src_state_u, dst_state_u, e_event_u, start_state_u, \
                    src_state_f, dst_state_f, e_event_f)):
                potential_events.append((src_state_u, dst_state_u, e_event_u))

    reachable_events = []
    for potential_e in potential_events:
        src_u = json.dumps(potential_e[0])
        unreachable_nodes = set(G_u.nodes).difference(nx.ancestors(G_u, src_u))

        if len(unreachable_nodes) == 1 and src_u in unreachable_nodes:
            reachable_events.append(potential_e)

    if len(reachable_events) == 0:
        return None
    elif len(reachable_events) > 1:
        print('Warning, found more than 1 potential/reachable update event, returning the first one...')
    return reachable_events[0] # return the only event

    '''
    print('EVENT_F:')
    pprint.pprint(e_event_f)
    print('SRC_STATE_F:')
    pprint.pprint(src_state_f)
    #print('DST_STATE_F:')
    #pprint.pprint(dst_state_f)
    print()
    print()

    for r in reachable_events:
        print(r[2]['event'])
        print('event:')
        pprint.pprint(r[2]) 
        print('src:')
        pprint.pprint(r[0]) 
        print()
    '''


def get_update_points(update_manifest, flashed_manifest, update_dir, flashed_dir):

    G_u = nx.read_gpickle(
        os.path.join(update_dir, update_manifest['update_graph']))
    G_f = nx.read_gpickle(
        os.path.join(flashed_dir, flashed_manifest['update_graph']))

    update_points = []
    for e in G_f.edges():
        src_state_f, dst_state_f = json.loads(e[0]), json.loads(e[1])
        e_events_f = G_f.get_edge_data(e[0], e[1])['label']

        for e_event_f in e_events_f:
            match = find_matching_update_event(G_u, src_state_f, dst_state_f, e_event_f)
            if match:
                src_state_u, dst_state_u, e_event_u = match
                update_points.append({
                    'src_state_f': src_state_f,
                    'dst_state_f': dst_state_f,
                    'src_state_u': src_state_u,
                    'dst_state_u': dst_state_u,
                    'e_event_f': e_event_f,
                    'e_event_u': e_event_u,
                })

    '''
    for p in update_points:
        print()
        pprint.pprint(p)
    '''

    return update_points


if __name__ == '__main__':

    parser = argparse.ArgumentParser(description='Primary control script for the live update process.')
    parser.add_argument('app_folder', help='App folder')
    parser.add_argument('dev', help='Serial port')
    parser.add_argument('--update_folder', help='Specify specific update package folder instead of deriving from app location', default=None)
    parser.add_argument('--flashed_folder', help='Specify specific flashed package folder instead of deriving from app location', default=None)
    parser.add_argument('--baud', help='Baud rate', type=int, default=115200)
    parser.add_argument('--timeout', help='Timeout', type=int, default=2)
    parser.add_argument('--force', help='Override slot warning', action='store_true', default=False)
    parser.add_argument('--dry_run', help='Does not attempt serial communication', action='store_true', default=False)
    args = parser.parse_args()

    if args.update_folder:
        update_folder = args.update_folder
    else:
        update_folder = os.path.join(args.app_folder, '_build', 'update')

    if args.flashed_folder:
        flashed_folder = args.flashed_folder
    else:
        flashed_folder = os.path.join(args.app_folder, '_build', 'flashed')

    with open(os.path.join(update_folder, 'manifest.json'), 'r') as f:
        manifest = json.load(f)

    with open(os.path.join(flashed_folder, 'manifest.json'), 'r') as f:
        flashed_manifest = json.load(f)

    if manifest['valid'] and flashed_manifest['valid'] and valid_partition(args.app_folder, manifest):

        update_points = get_update_points(manifest, flashed_manifest, update_folder, flashed_folder)

        update_predicates = generate_update_predicates(
            manifest,
            args.app_folder,
            update_folder,
            update_points
        )

        update_transfers = generate_update_transfers(
            manifest,
            args.app_folder,
            update_folder,
            update_points
        )

        '''
        for i in range(len(update_points)):
            print('=== PREDICATE ===')
            pprint.pprint(update_predicates[i])
            print()

            print('=== TRANSFER  ===')
            pprint.pprint(update_transfers[i])

            print()
            print()
        '''

        update_header = generate_update_header(
            manifest,
            args.app_folder,
            update_folder,
            update_predicates,
            update_transfers
        )
    
        update_payloads = generate_update_payloads(
            update_header,
            manifest,
            args.app_folder,
            update_folder,
            update_predicates,
            update_transfers
        )
    
        serialized_update_header = serialize_header(update_header, update_payloads)

        if not args.dry_run:
            with serial.Serial(args.dev, args.baud, timeout=args.timeout) as ser:

                print('sending header...')
                ser.write(serialized_update_header)

                for payload_name in ['text', 'rodata', 'predicates', 'transfers']:
                    time.sleep(1)
                    print('sending', payload_name, '...')
                    ser.write(update_payloads[payload_name])

            print("updating last flashed version number...")
            flashed_version_filename = os.path.join(args.app_folder, '_build', FLASHED_VERSION_NAME)
            os.system('echo {} > {}'.format(manifest['update_version'], flashed_version_filename))

            print("updating last flashed symbol file...")
            flashed_symbols_filename = os.path.join(args.app_folder, '_build', FLASHED_SYMBOLS_NAME)

            os.system('cp {} {}'.format(
                os.path.join(update_folder, manifest['update_symbols']),
                flashed_symbols_filename
            ))

            print("updating last flashed folder...")
            os.system('rm -rf {}'.format(
                flashed_folder
            ))
            os.system('cp -r {}'.format(
                update_folder
            ))

