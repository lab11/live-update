import argparse
from copy import deepcopy
from elftools.elf.elffile import ELFFile
import json
import networkx as nx
import pprint
import pygraphviz
import pyjson5
import re


def constraints_to_fn(c_list):

    def always_true(state):
        return True

    if not c_list:
        return always_true

    reg_symbol_re = re.compile('reg_.*<.* (.*)>$')
    derived_symbol_re = re.compile('derived_.*\{.*,(.*)\}$')
    range_re = re.compile('\{.*\[(\d+), (\d+)\].*\}')

    # TODO handle more constraints that aren't just one int
    constraint_ranges = []  # [symbol, lower, upper]
    for c in c_list:
        m = reg_symbol_re.match(c['symbol'])
        if m:
            sym_name = m.group(1)
        else:
            m = derived_symbol_re.match(c['symbol'])
            if m:
                sym_name = m.group(1)
            else:
                return None

        m = range_re.match(c['range'])
        lower = int(m.group(1))
        upper = int(m.group(2))

        constraint_ranges.append([sym_name, lower, upper])

    def check_constraint(state):
        mem = state[0]
        for c in constraint_ranges:
            if c[0] not in mem:
                return False

            if mem[c[0]] < c[1] or mem[c[0]] > c[2]:
                return False

        return True

    return check_constraint


def eval_symbolic_expression(exp, state):
    #print()
    #print()
    #print('evaluating', exp, 'state is', state)
    if exp['type'] == 'data' and exp['subtype'] == 'derived':
        #print(' result', state[exp['region']])
        return state[exp['region']] 

    elif exp['type'] == 'binary':
        operator = exp['operator']

        if type(exp['lhs']) == dict: # assume it's another expression
            lhs = eval_symbolic_expression(exp['lhs'], state)
        else: # assume it's a constant
            lhs = exp['lhs'].replace('U', '')

        if type(exp['rhs']) == dict: # assume it's another expression
            rhs = eval_symbolic_expression(exp['rhs'], state)
        else: # assume it's a constant
            rhs = exp['rhs'].replace('U', '')

        #print(' result of evaluating', str(lhs) + operator + str(rhs), 'is', eval(str(lhs) + operator + str(rhs)))
        output = eval(str(lhs) + operator + str(rhs))
        if output is True:
            #print('  returning 1')
            return 1
        elif output is False:
            #print('  returning 0')
            return 0
        else:
            #print('  returning', output)
            return output

    else:
        print('don\'t support that type of symbolic expression yet, enjoy the exit')
        exit(1)


def apply_action(e, state):

    result = deepcopy(state)
    mem, timers, gpio = result
    reset_timers = []

    if 'call' in e:
        if "tfm_gpio_enable_output" in e['call']:
            r = re.compile('tfm_gpio_enable_output\((.*)\)$')
            m = r.match(e['call'])
            if m:
                pin = int(m.group(1))
                gpio['output_enabled'] |= (1 << pin)

        elif "tfm_gpio_disable" in e['call']:
            r = re.compile('tfm_gpio_disable\((.*)\)$')
            m = r.match(e['call'])
            if m:
                pin = int(m.group(1))
                gpio['output_enabled'] &= ~(1 << pin)

        elif "tfm_gpio_interrupt_enable" in e['call']:
            r = re.compile('tfm_gpio_interrupt_enable\((.*),(.*)\)$')
            m = r.match(e['call'])
            if m:
                pin = int(m.group(1))
                gpio['interrupt_enabled'] |= (1 << pin)

                cfg = m.group(2)
                for i in e['program_state']['store']['items']:
                    if i['cluster'] in cfg: # same var name
                        callback = i['items'][2]['value']
                        callback_match = re.compile('&code\{(.*)\}').match(callback)
                        if callback_match:
                            gpio['interrupts'][pin] = callback_match.group(1)

        elif "tfm_gpio_interrupt_disable" in e['call']:
            r = re.compile('tfm_gpio_interrupt_disable\((.*)\)$')
            m = r.match(e['call'])
            if m:
                pin = int(m.group(1))
                gpio['interrupt_enabled'] &= ~(1 << pin)
                del gpio['interrupts'][pin]

        elif "k_timer_init" in e['call']:
            r = re.compile('k_timer_init\((.*), (.*), (.*)\)$')
            m = r.match(e['call'])
            if m:
                timers['timers'][m.group(1)] = {
                    'expiry_fn': m.group(2),
                    'stop_fn': m.group(3)
                }

        elif "k_timer_start" in e['call']:
            r = re.compile('k_timer_start\((.*), (.*), (.*)\)$')
            m = r.match(e['call'])
            if m:
                timers['timers'][m.group(1)]['duration_ms'] = \
                    eval(m.group(2).replace('U', ''))
                timers['timers'][m.group(1)]['period_ms'] = \
                    eval(m.group(3).replace('U', ''))
                timers['active'].append(m.group(1))
                reset_timers.append(m.group(1))

        elif "tfm_gpio_clear" in e['call']:
            r = re.compile('tfm_gpio_clear\((.*)\)$')
            m = r.match(e['call'])
            if m:
                pin = int(m.group(1))
                gpio['set'] &= ~(1 << pin)

        elif "tfm_gpio_set" in e['call']:
            r = re.compile('tfm_gpio_set\((.*)\)$')
            m = r.match(e['call'])
            if m:
                pin = int(m.group(1))
                gpio['set'] |= (1 << pin)

        elif "tfm_gpio_write_all" in e['call']:
            r = re.compile('tfm_gpio_write_all\((.*)\)$')
            m = r.match(e['call'])
            if m:
                val = int(m.group(1))
                gpio['set'] = val

        else:
            print('DIDN\'T IMPLEMENT apply_action for call e:', e)
            exit(1)

    elif e['event'] == 'bind':
        if 'bindloc_region' not in e:
            print('Assumed that bindloc_region was there, it\'s not, enjoy your exit')
            exit(1)

        dest_name = e['bindloc_region']
        
        if 'bindval_nonloc_concrete_int' in e:
            mem[dest_name] = int(e['bindval_nonloc_concrete_int'].split()[0])
        elif 'bindval_symbolic_expression' in e:
            mem[dest_name] = eval_symbolic_expression(e['bindval_symbolic_expression'], mem)
        else:
            print('Assumed that some known bindval format was there, it\'s not, enjoy your exit')
            exit(1)

    else:
        print('DIDN\'T IMPLEMENT apply_action for some e:', e)
        exit(1)

    return result, reset_timers


def apply_event_path(event_id, specific_events, state):

    end_result = (state, [])
    for e in specific_events:
        if e['currentID'] == event_id:
            new_state, reset_timers = apply_action(e, state)
            end_result = (new_state, reset_timers)

            for next_e in specific_events:
                if next_e['previousID'] == event_id:
                    constraint_fn = constraints_to_fn(next_e['program_state']['constraints'])
                    if constraint_fn(new_state):
                        future_result, future_reset_timers = apply_event_path(next_e['currentID'], specific_events, new_state) 
                        return (future_result, future_reset_timers + reset_timers)

    return end_result


def apply_event(name, events, state):
    if name not in events:
        # print('applying empty event', name, 'because it\'s not listed')
        return (state, [])

    mem, timers, gpio = state
    reset_timers = []
    for t in timers['timers']:
        if timers['timers'][t]['expiry_fn'] == name:
            reset_timers.append(t)

    for e in events[name]:
        if e['previousID'] == "0":
            constraint_fn = constraints_to_fn(e['program_state']['constraints'])
            if constraint_fn(state):
                new_state, new_reset_timers = apply_event_path(e['currentID'], events[name], state)
                return (new_state, new_reset_timers + reset_timers)

    return (None, None)


def to_node(state):
    return json.dumps(state)


def to_state(node):
    return json.loads(node)


if __name__ == '__main__':

    parser = argparse.ArgumentParser(description='Static analysis script')
    parser.add_argument('app_symbols', help='Dumped symbol file for application')
    parser.add_argument('app_elf', help='Application ELF file')
    parser.add_argument('app_clang_json', help='Clang checker json output')
    parser.add_argument('out_graph_json', help='Output location (graph in JSONish format)')
    parser.add_argument('--dump_image', help='(optional) Dump an image of the graph somewhere')
    args = parser.parse_args()

    with open(args.app_symbols, 'r') as f:
        app_symbols = [x.strip().split() for x in f.readlines()[4:]]
        app_symbols = [l for l in app_symbols if len(l) > 0]

    with open(args.app_clang_json, 'r') as f:
        clang_data = pyjson5.load(f)

    system_events = {}
    for i in clang_data['analysis']:
        if i['caller'] not in system_events:
            system_events[i['caller']] = []

        system_events[i['caller']].append(i)

    # Initialize application global memory state
    init_mem = {}

    with open(args.app_elf, 'rb') as f:
        elffile = ELFFile(f)
        app_data = elffile.get_section_by_name('.app_data')

        if app_data:
            app_data = app_data.data()

            for data_item in [i for i in app_symbols if i[-3] == '.app_data' and int(i[-2], base=16) != 0]:
                symbol_size = int(data_item[-2], base=16)

                symbol_addr = int(data_item[0], base=16)
                if symbol_addr >= 0x2001c000: # it's hardcoded for now, sadly
                    base = 0x2001c000
                else:
                    base = 0x20018000
                symbol_offset = symbol_addr - base

                # only handle int values right now
                init_mem[data_item[-1]] = int.from_bytes(app_data[symbol_offset:symbol_offset+symbol_size], 'little')
    
    print(init_mem)
    
    for bss_item in [i for i in app_symbols if i[-3] == '.app_bss' and int(i[-2], base=16) != 0]:
        symbol_size = int(bss_item[-2], base=16)
        if symbol_size > 4:
            print('warning: app_bss state size > 4 bytes not supported right this minute, skipping {}...'.format(bss_item[-1]))
            continue
        init_mem[bss_item[-1]] = 0

    # Modify state, init timers, and init gpio based on `main`
    init_timers = {'timers': {}, 'active': []}
    init_gpio = {'interrupt_enabled': 0, 'interrupts': {}, 'output_enabled': 0, 'set': 0}

    start_state, reset_timers = apply_event('main', system_events, (init_mem, init_timers, init_gpio))
    #print(start_state)

    to_explore = []
    G = nx.DiGraph()

    G.add_node(to_node(start_state))
    to_explore.append(start_state)

    while len(to_explore) > 0:
        curr_state = to_explore.pop()

        possible_events = curr_state[2]['interrupts'].values()
        for active_timer in curr_state[1]['active']:
            possible_events.append(curr_state[1]['timers'][active_timer]['expiry_fn'])

        for e in possible_events:
            new_state, reset_timers = apply_event(e, system_events, curr_state)
            if to_node(new_state) not in G:
                G.add_node(to_node(new_state))
                to_explore.append(new_state)
            print('adding edge for event {} from {} to {}'.format(e, curr_state, new_state))

            U, V = to_node(curr_state), to_node(new_state)
            if not G.has_edge(U, V):
                G.add_edge(U, V, label=[{'event': e, 'reset': reset_timers}])
            else:
                G.get_edge_data(U, V)['label'].append({'event': e, 'reset': reset_timers})

    if args.dump_image:
        A = nx.nx_agraph.to_agraph(G)
        A.layout('dot')
        A.draw(args.dump_image)

    nx.write_gpickle(G, args.out_graph_json)


