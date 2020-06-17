import argparse
from collections import defaultdict
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
    range_re = re.compile('\{.*\[(-?\d+), (-?\d+)\].*\}')

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
            if c[0] not in mem: return False

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


def apply_action(e, state, edge_data):
    #print('apply_action:', e['event'], e['call'] if 'call' in e else '', e['caller'], e['previousID'], e['currentID'])

    if 'call_gpio' in e['event']:
        if "tfm_gpio_enable_output" in e['call']:
            new_state = deepcopy(state)
            mem, timers, gpio = new_state

            r = re.compile('tfm_gpio_enable_output\((.*)\)$')
            m = r.match(e['call'])
            if m:
                pin = int(m.group(1))
                gpio['output_enabled'] |= (1 << pin)

            return (new_state, [])

        elif "tfm_gpio_disable" in e['call']:
            new_state = deepcopy(state)
            mem, timers, gpio = new_state

            r = re.compile('tfm_gpio_disable\((.*)\)$')
            m = r.match(e['call'])
            if m:
                pin = int(m.group(1))
                gpio['output_enabled'] &= ~(1 << pin)

            return (new_state, [])

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

            return (new_state, [])

        elif "tfm_gpio_interrupt_disable" in e['call']:
            new_state = deepcopy(state)
            mem, timers, gpio = new_state

            r = re.compile('tfm_gpio_interrupt_disable\((.*)\)$')
            m = r.match(e['call'])
            if m:
                pin = int(m.group(1))
                gpio['interrupt_enabled'] &= ~(1 << pin)
                del gpio['interrupts'][pin]

            return (new_state, [])

        elif "tfm_gpio_clear" in e['call']:
            new_state = deepcopy(state)
            mem, timers, gpio = new_state

            r = re.compile('tfm_gpio_clear\((.*)\)$')
            m = r.match(e['call'])
            if m:
                pin = int(m.group(1))
                gpio['set'] &= ~(1 << pin)

            return (new_state, [])

        elif "tfm_gpio_set" in e['call']:
            new_state = deepcopy(state)
            mem, timers, gpio = new_state

            r = re.compile('tfm_gpio_set\((.*)\)$')
            m = r.match(e['call'])
            if m:
                pin = int(m.group(1))
                gpio['set'] |= (1 << pin)

            return (new_state, [])

        elif "tfm_gpio_write_all" in e['call']:
            new_state = deepcopy(state)
            mem, timers, gpio = new_state

            r = re.compile('tfm_gpio_write_all\((.*)\)$')
            m = r.match(e['call'])
            if m:
                val = int(m.group(1))
                gpio['set'] = val

            return (new_state, [])

    elif 'call_timer' in e['event']:
        if e['call'].startswith("k_timer_init"):
            new_state = deepcopy(state)
            mem, timers, gpio = new_state

            r = re.compile('k_timer_init\((.*), (.*), (.*)\)$')
            m = r.match(e['call'])
            if m:
                timers['timers'][m.group(1)] = {
                    'expiry_fn': None if '(void *)0' in m.group(2) else m.group(2),
                    'stop_fn': None if '(void *)0' in m.group(3) else m.group(3)
                }

            return (new_state, [])

        elif e['call'].startswith("k_timer_start"):
            new_state = deepcopy(state)
            mem, timers, gpio = new_state
            reset_timers = []

            r = re.compile('k_timer_start\((.*), (.*), (.*)\)$')
            m = r.match(e['call'])
            if m:
                if edge_data and 'label' in edge_data and 'vars' in edge_data['label']  and 'stopped_timer' in edge_data['label']['vars']:
                    t_name = edge_data['label']['vars']['stopped_timer']
                else:
                    t_name = m.group(1)

                #print('T NAME', t_name)
                timers['timers'][t_name]['duration_ms'] = \
                    eval(m.group(2).replace('U', ''))
                timers['timers'][t_name]['period_ms'] = \
                    eval(m.group(3).replace('U', ''))
                if t_name not in timers['active']:
                    timers['active'].append(t_name)

                reset_timers.append(t_name)

            return (new_state, reset_timers)

        elif e['call'].startswith("k_timer_stop"):
            new_state = deepcopy(state)
            mem, timers, gpio = new_state

            if edge_data and 'label' in edge_data and 'vars' in edge_data['label']  and 'stopped_timer' in edge_data['label']['vars']:
                t_name = edge_data['label']['vars']['stopped_timer']
            else:
                r = re.compile('k_timer_stop\((.*)\)$')
                m = r.match(e['call'])

                t_name = m.group(1)

            if m and t_name in timers['active']:
                timers['active'].remove(m.group(1)) 

            return (new_state, [t_name])

    elif 'call_other' in e['event']:
        #print('call other')
        new_state = deepcopy(state)
        mem, timers, gpio = new_state

        return (new_state, [])

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

        return (new_state, [])

    else:
        print('DIDN\'T IMPLEMENT apply_action for some e:', e)
        exit(1)


def apply_event_path(eventG, path, start_state):
    #print()
    #print('PATH (length', len(path), ')')

    curr_state = start_state
    curr_in_edge = None

    reset_timers = []
    for t_name, t in start_state[1]['timers'].items():
        if t['expiry_fn'] == path[0]['caller']:
            reset_timers.append(t_name)

    for idx, s in enumerate(path):
        if idx > 0:
            curr_in_edge = eventG.get_edge_data(to_node(path[idx-1]), to_node(path[idx]))

        constraint_fn = constraints_to_fn(s['program_state']['constraints'])
        if not constraint_fn(curr_state):
            #print(idx, 'state does not meet constraints:')
            #print('  constraints:', s['program_state']['constraints'])
            #print('  state:', curr_state)
            break # done processing path

        next_state, r = apply_action(s, curr_state, curr_in_edge)

        curr_state = next_state
        reset_timers += r

    return curr_state, list(set(reset_timers))


def apply_event(eventG, name, paths, start_state):

    #print()
    #print()
    #print('EVENT', name)

    results = []

    event_paths = paths[name]
    for p in event_paths:
        #print(p)
        p = [to_state(s) for s in p]
        result = apply_event_path(eventG, p, start_state)
        if result:
            #print(result)
            results.append(result)

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



    #print(len(events))
    #print(e['event'], e['caller'], 'calling', e['call'] if 'call' in e else '', e['currentID'])
    eventG = nx.DiGraph()

    top_level_events = [e for e in events if e['previousID'] == '0']
    top_level_callers = [e['caller'] for e in top_level_events]

    frontier = top_level_events
    for n in frontier:
        #print(n['caller'], n['program_state']['constraints'])
        eventG.add_node(to_node(n))

    # build initial graph without interproc. dependencies
    while len(frontier) != 0:
        curr_n = frontier.pop()

        for next_n in events:
            if curr_n['currentID'] == next_n['previousID'] and curr_n['caller'] == next_n['caller']:
                if to_node(next_n) not in eventG:
                    eventG.add_node(to_node(next_n))
                eventG.add_edge(to_node(curr_n), to_node(next_n))
                frontier.append(next_n)

    # get leaf node information for each event
    leaves = {} # top level call name -> leaf nodes
    for top_level_node in [to_node(e) for e in events if e['previousID'] == '0']:
        leaf_list = []

        #print(to_state(top_level_node)['caller'])
        #print(len(nx.descendants(eventG, top_level_node)))
        for s in nx.descendants(eventG, top_level_node):
            if len(list(eventG.successors(s))) == 0:
                leaf_list.append(s)

        if len(leaf_list) == 0:
            leaf_list.append(top_level_node)

        leaves[to_state(top_level_node)['caller']] = leaf_list

    # get timer stop callback linkage
    timer_stop_cbs = {}
    for n in eventG:
        curr_n = to_state(n)
        if curr_n['event'] == 'call_timer':
            m = re.compile('^k_timer_init\((.*), (.*), (.*)\)$').match(curr_n['call'])
            if m:
                timer = m.group(1)
                stop_cb = m.group(3)
                timer_stop_cbs[timer] = stop_cb

    # insert procedure calls
    for n in eventG:
        curr_n = to_state(n)


        if curr_n['event'] == 'call_other':
            m = re.compile('^(.*)\(.*\)$').match(curr_n['call'])
            if m:
                fn = m.group(1) 
                if fn in top_level_callers:

                    # unlink next nodes
                    curr_n_successors = list(eventG.successors(n))
                    for s in curr_n_successors:
                        eventG.remove_edge(n, s)

                    for t in [e for e in events if e['previousID'] == '0']:
                        if t['caller'] == fn:

                            eventG.add_edge(n, to_node(t))

                            for l in leaves[fn]:
                                for s in curr_n_successors:
                                    eventG.add_edge(l, s)

            else:
                print('RIP failed call_other fn name regex')

        elif curr_n['event'] == 'call_timer':
            m = re.compile('^k_timer_stop\((.*)\)$').match(curr_n['call'])
            if m:
                timer_name = m.group(1)
                if timer_name in timer_stop_cbs and timer_stop_cbs[timer_name] in top_level_callers:

                    # unlink next nodes
                    curr_n_successors = list(eventG.successors(n))
                    for s in curr_n_successors:
                        eventG.remove_edge(n, s)

                    for t in [e for e in events if e['previousID'] == '0']:
                        if t['caller'] == timer_stop_cbs[timer_name]:

                            eventG.add_edge(n, to_node(t), label={'vars':{'stopped_timer': timer_name}})

                            for l in leaves[timer_stop_cbs[timer_name]]:
                                for s in curr_n_successors:
                                    eventG.add_edge(l, s)

    # Calculate paths through graph
    paths = defaultdict(list)
    for n in eventG:
        n_state = to_state(n)
        if n_state['caller'] in top_level_callers and len(list(eventG.predecessors(n))) == 0:
            #print('PATHS - for', n_state['caller'])
            descendant_nodes = nx.descendants(eventG, n)
            for dest_n in descendant_nodes:
                if len(list(eventG.successors(dest_n))) == 0:
                    #print('  SINK', to_state(dest_n)['call'] if 'call' in to_state(dest_n) else '', to_state(dest_n)['caller'])
                    paths[n_state['caller']] += list(nx.all_simple_paths(eventG, n, dest_n))

            if len(descendant_nodes) == 0:
                paths[n_state['caller']].append([n])

    for e in paths:
        #print(e, len(paths[e]))
        for p in paths[e]:
            #print('  ', len(p), type(p))
            pass
    
    nx.nx_pydot.write_dot(eventG, 'TEST_EVENT.dot')

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
    for next_state, reset_timers in apply_event(eventG, 'main', paths, init_state):
        G.add_node(to_node(next_state))
        G.add_edge(to_node(init_state), to_node(next_state), label=[{'event': 'main', 'reset': reset_timers}])

        to_explore.append(next_state)

        '''
        print()
        print()
        print('Adding main edge:')
        print()
        print('    EDGE')
        pprint.pprint({
            'event': 'main',
            'reset': t
        })
        print('    SOURCE')
        pprint.pprint(init_state)
        print('    DESTINATION')
        pprint.pprint(next_state)
        print()
        '''

    while len(to_explore) > 0:
        #print('n_to_explore:', len(to_explore))
        curr_state = to_explore.pop()

        possible_events = list(curr_state[2]['interrupts'].values())
        for active_timer in curr_state[1]['active']:
            if curr_state[1]['timers'][active_timer]['expiry_fn']:
                possible_events.append(curr_state[1]['timers'][active_timer]['expiry_fn'])

        for e in possible_events:
            #print('APPLY TOP LEVEL')
            #pprint.pprint(curr_state)
            for next_state, t in apply_event(eventG, e, paths, curr_state):
                if to_node(next_state) not in G:
                    G.add_node(to_node(next_state))
                    to_explore.append(next_state)

                '''
                print()
                print()
                print('Adding edge:')
                print()
                print('    EDGE')
                pprint.pprint({
                    'event': e,
                    'reset': t
                })
                print('    SOURCE')
                pprint.pprint(curr_state)
                print('    DESTINATION')
                pprint.pprint(s)
                print()
                '''

                U, V = to_node(curr_state), to_node(next_state)
                if not G.has_edge(U, V):
                    G.add_edge(U, V, label=[{'event': e, 'reset': t}])
                else:
                    G.get_edge_data(U, V)['label'].append({'event': e, 'reset': t})

    if args.dump_dot:
        nx.nx_pydot.write_dot(G, args.dump_dot)

    nx.write_gpickle(G, args.out_graph_json)

