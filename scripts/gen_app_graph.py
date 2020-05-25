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


def apply_action(all_events, e, state):

    if 'call_gpio' in e['event']:
        if "tfm_gpio_enable_output" in e['call']:
            new_state = deepcopy(state)
            mem, timers, gpio = new_state

            r = re.compile('tfm_gpio_enable_output\((.*)\)$')
            m = r.match(e['call'])
            if m:
                pin = int(m.group(1))
                gpio['output_enabled'] |= (1 << pin)

            return [(new_state, [])] 

        elif "tfm_gpio_disable" in e['call']:
            new_state = deepcopy(state)
            mem, timers, gpio = new_state

            r = re.compile('tfm_gpio_disable\((.*)\)$')
            m = r.match(e['call'])
            if m:
                pin = int(m.group(1))
                gpio['output_enabled'] &= ~(1 << pin)

            return [(new_state, [])]

        elif "tfm_gpio_interrupt_enable" in e['call']:
            new_state = deepcopy(state)
            mem, timers, gpio = new_state

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

            return [(new_state, [])]

        elif "tfm_gpio_interrupt_disable" in e['call']:
            new_state = deepcopy(state)
            mem, timers, gpio = new_state

            r = re.compile('tfm_gpio_interrupt_disable\((.*)\)$')
            m = r.match(e['call'])
            if m:
                pin = int(m.group(1))
                gpio['interrupt_enabled'] &= ~(1 << pin)
                del gpio['interrupts'][pin]

            return [(new_state, [])]

        elif "tfm_gpio_clear" in e['call']:
            new_state = deepcopy(state)
            mem, timers, gpio = new_state

            r = re.compile('tfm_gpio_clear\((.*)\)$')
            m = r.match(e['call'])
            if m:
                print('found GPIO clear on pin', int(m.group(1)))
                pin = int(m.group(1))
                print('set before:', gpio['set'])
                gpio['set'] &= ~(1 << pin)
                print('set afer:', gpio['set'])

            return [(new_state, [])]

        elif "tfm_gpio_set" in e['call']:
            new_state = deepcopy(state)
            mem, timers, gpio = new_state

            r = re.compile('tfm_gpio_set\((.*)\)$')
            m = r.match(e['call'])
            if m:
                print('found GPIO set on pin', int(m.group(1)))
                pin = int(m.group(1))
                print('set before:', gpio['set'])
                gpio['set'] |= (1 << pin)
                print('set after:', gpio['set'])

            return [(new_state, [])]

        elif "tfm_gpio_write_all" in e['call']:
            new_state = deepcopy(state)
            mem, timers, gpio = new_state

            r = re.compile('tfm_gpio_write_all\((.*)\)$')
            m = r.match(e['call'])
            if m:
                val = int(m.group(1))
                gpio['set'] = val

            return [(new_state, [])]

    elif 'call_timer' in e['event']:
        if "k_timer_init" in e['call']:
            new_state = deepcopy(state)
            mem, timers, gpio = new_state

            r = re.compile('k_timer_init\((.*), (.*), (.*)\)$')
            m = r.match(e['call'])
            if m:
                timers['timers'][m.group(1)] = {
                    'expiry_fn': None if '(void *)0' in m.group(2) else m.group(2),
                    'stop_fn': None if '(void *)0' in m.group(3) else m.group(3)
                }

            return [(new_state, [])]

        elif "k_timer_start" in e['call']:
            new_state = deepcopy(state)
            mem, timers, gpio = new_state
            reset_timers = []

            r = re.compile('k_timer_start\((.*), (.*), (.*)\)$')
            m = r.match(e['call'])
            if m:
                if 'stopping' not in timers or not timers['stopping']:
                    t_name = m.group(1)
                else:
                    t_name = timers['stopping']
                timers['stopping'] = None
                timers['timers'][t_name]['duration_ms'] = \
                    eval(m.group(2).replace('U', ''))
                timers['timers'][t_name]['period_ms'] = \
                    eval(m.group(3).replace('U', ''))
                if t_name not in timers['active']:
                    timers['active'].append(t_name)
                reset_timers.append(t_name)

            return [(new_state, reset_timers)]

        elif "k_timer_stop" in e['call']:
            new_state = deepcopy(state)
            mem, timers, gpio = new_state

            r = re.compile('k_timer_stop\((.*)\)$')
            m = r.match(e['call'])

            t_name = m.group(1)
            if m and t_name in timers['active']:
                timers['active'].remove(m.group(1))

            t = timers['timers'][t_name] 
            if t and t['stop_fn'] and t['stop_fn'] in all_events:
                timers['stopping'] = t_name

                results = []
                for s, t in apply_event(t['stop_fn'], 0, all_events, new_state):
                    results.append((s, t))
                return results
            else:
                return [(new_state, [])]

    # handle potential additional 'events' due to calls to other translation units
    elif 'call_other' in e['event']:
        new_state = deepcopy(state)
        mem, timers, gpio = new_state

        r = re.compile('(.*)\(.*\)$')
        m = r.match(e['call'])
        if m:
            call_name = m.group(1)
            if call_name in all_events:
                results = []
                for s, t in apply_event(call_name, 0, all_events, new_state):
                    results.append((s, t))
                return results
            else:
                return [(new_state, [])]
        else:
            return [(new_state, [])]

    elif 'bind' in e['event']:
        if 'bindloc_region' not in e:
            print('Assumed that bindloc_region was there, it\'s not, enjoy your exit')
            exit(1)

        new_state = deepcopy(state)
        mem, timers, gpio = new_state

        dest_name = e['bindloc_region']
        
        if 'bindval_nonloc_concrete_int' in e:
            mem[dest_name] = int(e['bindval_nonloc_concrete_int'].split()[0])
        elif 'bindval_symbolic_expression' in e:
            mem[dest_name] = eval_symbolic_expression(e['bindval_symbolic_expression'], mem)
        else:
            print('Assumed that some known bindval format was there, it\'s not, enjoy your exit')
            exit(1)

        return [(new_state, [])]

    else:
        print('DIDN\'T IMPLEMENT apply_action for some e:', e)
        exit(1)


'''
def apply_event_path(event_id, global_events, specific_events, state):

    end_results = []
    for e in specific_events:
        if e['currentID'] == event_id:
            for s, t in apply_action(global_events, e, state):
                end_results.append((s, t))  # in case no other paths to take

                for next_e in specific_events:
                    if next_e['previousID'] == event_id:
                        constraint_fn = constraints_to_fn(next_e['program_state']['constraints'])
                        if constraint_fn(s):
                            results = []
                            for s2, t2 in apply_event_path(next_e['currentID'], global_events, specific_events, s):
                                results.append((s2, t + t2))
                            return results
    return end_results
'''


def apply_event(name, previousID, events, state):
    print('apply_event:', name, 'prev_id:', previousID)

    # for the event caller `name`, given the current state `state` generated by
    # event `previousID`, apply any following events and return the resulting
    # states

    results = []
    for next_event in events:
        if int(next_event['previousID']) == previousID and name == next_event['caller']:
            constraint_fn = constraints_to_fn(next_event['program_state']['constraints'])
            if constraint_fn(state):
                print('  apply_action', next_event['call'] if 'call' in next_event else '', 'curr_id:', next_event['currentID'])
                for new_state, new_reset_timers in apply_action(events, next_event, state):
                    results += apply_event(name, int(next_event['currentID']), events, new_state)

    if len(results) == 0: # no valid next events, just return our state
        results.append((state, []))

    if previousID == 0:
        reset_timer = None
        for t_name, t in state[1]['timers'].items():
            if t['expiry_fn'] == name:
                reset_timer = t_name

        for r in results:
            r[1].append(reset_timer)

    return results


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
    parser.add_argument('--dump_dot', help='(optional) Dump a dot of the graph somewhere')
    args = parser.parse_args()

    with open(args.app_symbols, 'r') as f:
        app_symbols = [x.strip().split() for x in f.readlines()[4:]]
        app_symbols = [l for l in app_symbols if len(l) > 0]

    with open(args.app_clang_json, 'r') as f:
        clang_data = pyjson5.load(f)

    # preprocess analysis to get rid of live_symbols and apply those constraints to other events
    events = []
    current_constraints = None
    for e in clang_data['analysis']:
        if e['event'] == 'live_symbols':
            current_constraints = e['program_state']['constraints']
        else:
            e['program_state']['constraints'] = current_constraints 
            events.append(e)

    pprint.pprint(events)

    '''
    for i in clang_data['analysis']:

        if i['caller'] not in system_events:
            system_events[i['caller']] = []

        system_events[i['caller']].append(i)
    '''

    # Initialize application global memory state
    init_mem = {}

    with open(args.app_elf, 'rb') as f:
        elffile = ELFFile(f)
        app_data = elffile.get_section_by_name('.app_data')

        if app_data:
            app_data = app_data.data()

            for data_item in [i for i in app_symbols if i[-3] == '.app_data' and int(i[-2], base=16) != 0]:
                symbol_size = int(data_item[-2], base=16)
                if symbol_size > 4:
                    print('app_data (> 4 bytes): ignoring {}...'.format(data_item[-1]))
                    continue

                symbol_addr = int(data_item[0], base=16)
                if symbol_addr >= 0x2001c000:
                    base = 0x2001c000
                else:
                    base = 0x20018000
                symbol_offset = symbol_addr - base

                # only handle int values right now
                init_mem[data_item[-1]] = int.from_bytes(app_data[symbol_offset:symbol_offset+symbol_size], 'little')
    
    for bss_item in [i for i in app_symbols if i[-3] == '.app_bss' and int(i[-2], base=16) != 0]:
        symbol_size = int(bss_item[-2], base=16)
        if symbol_size > 4:
            print('bss_state (> 4 bytes): ignoring {}...'.format(bss_item[-1]))
            continue
        init_mem[bss_item[-1]] = 0

    # Modify state, init timers, and init gpio based on `main`
    init_timers = {'timers': {}, 'active': []}
    init_gpio = {'interrupt_enabled': 0, 'interrupts': {}, 'output_enabled': 0, 'set': 0}

    init_state = (init_mem, init_timers, init_gpio)

    G = nx.DiGraph()
    G.add_node(to_node(init_state))

    to_explore = []
    print()
    print('APPLY MAIN')
    for next_state, t in apply_event('main', 0, events, init_state):
        G.add_node(to_node(next_state))
        G.add_edge(to_node(init_state), to_node(next_state), label=[{'event': 'main', 'reset': t}])

        to_explore.append(next_state)

    while len(to_explore) > 0:
        print('n_to_explore:', len(to_explore))
        curr_state = to_explore.pop()

        possible_events = list(curr_state[2]['interrupts'].values())
        for active_timer in curr_state[1]['active']:
            if curr_state[1]['timers'][active_timer]['expiry_fn']:
                possible_events.append(curr_state[1]['timers'][active_timer]['expiry_fn'])

        for e in possible_events:
            print('APPLY TOP LEVEL')
            pprint.pprint(curr_state)
            for s, t in apply_event(e, 0, events, curr_state):
                if to_node(s) not in G:
                    G.add_node(to_node(s))
                    to_explore.append(s)

                '''
                print()
                print('Adding edge:')
                print('    SOURCE')
                pprint.pprint(curr_state)
                print('    DESTINATION')
                pprint.pprint(s)
                print('    EDGE')
                pprint.pprint(e)
                print()
                '''

                U, V = to_node(curr_state), to_node(s)
                if not G.has_edge(U, V):
                    G.add_edge(U, V, label=[{'event': e, 'reset': t}])
                else:
                    G.get_edge_data(U, V)['label'].append({'event': e, 'reset': t})

    if args.dump_dot:
        nx.nx_pydot.write_dot(G, args.dump_dot)

    nx.write_gpickle(G, args.out_graph_json)


