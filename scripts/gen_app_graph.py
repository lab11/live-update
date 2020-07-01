import argparse
from collections import defaultdict
from copy import deepcopy
from elftools.elf.elffile import ELFFile
import json
import networkx as nx
import os
import pprint
import pygraphviz
import pyjson5
import re


'''
Convert state object to hashable form in networkx graph
'''
def to_node(state):
    return json.dumps(state)


'''
Convert networkx graph node back into a state object
'''
def to_state(node):
    return json.loads(node)


'''
Hide inline calls that dirty up the final graph. References the list of
supported API or incline calls for which we ignore subcalls.
'''
HIDDEN_INLINES = [
    'k_ms_to_ticks_ceil32',
    'z_tmcvt',
]

HIDDEN_SYSCALLS = [
    'device_get_binding',

    'gpio_pin_set',
    'gpio_pin_configure',
    'gpio_pin_interrupt_configure',
    'gpio_init_callback',
    'gpio_add_callback',

    'k_timer_init',
    'k_timer_start',
]
def hide_calls(G):
    newG = deepcopy(G)

    for n in G:
        n_state = to_state(n)

        if 'program_state' not in n_state:
            continue

        remove = False
        for i in HIDDEN_INLINES:
            if 'call' in n_state and n_state['call'].startswith(i):
                remove = True
                break

        for loc in n_state['program_state']['environment']['items']:
            if loc['calling'] in HIDDEN_SYSCALLS:
                remove = True
                break

        # Remove node, assuming one incoming edge (tree structure)
        if remove:
            predecessor = list(newG.predecessors(n))[0]
            successors = list(newG.successors(n))

            newG.remove_node(n)

            for s in successors:
                newG.add_edge(predecessor, s)

    return newG


'''
Links procedure calls across compilation units
'''
def link_calls(G):

    newG = deepcopy(G)

    entry_nodes = {}
    for n in G:
        if 'currentID' not in to_state(n):
            entry_nodes[to_state(n)['event']] = n
    print(entry_nodes.keys())

    call_leaves = {}
    for k, v in entry_nodes.items():
        call_leaves[k] = [d for d in nx.descendants(G, v) if len(list(G.successors(d))) == 0]

    for n in G:
        if 'call' in to_state(n):
            paren_idx = to_state(n)['call'].find('(')
            fn_name = to_state(n)['call'][:paren_idx]

            if fn_name in entry_nodes:
                predecessor = list(newG.predecessors(n))[0]
                successors = list(newG.successors(n))

                newG.remove_node(n)

                newG.add_edge(predecessor, entry_nodes[fn_name])
                for l in call_leaves[fn_name]:
                    for s in successors:
                        newG.add_edge(l, s)

    return newG


'''
Replaces statement pretty representations with their values, if known
'''
def replace_with_value(e, e_str):
    # target the first stack frame
    for stmt in e['program_state']['environment']['items'][0]['items']:
        if stmt['value'].startswith('&code') or \
           stmt['value'].startswith('&SymRegion') or \
           stmt['value'].startswith('lazyCompoundVal') or \
           stmt['value'].startswith('conj_$') or \
           stmt['value'].startswith('reg_$'):
            continue
        try:
            int(stmt['pretty'])
            continue
        except: # not an integer constant, proceed with replacement
            paren_idx = e_str.find('(') # figure out where args start
            e_str = e_str[:paren_idx+1] + e_str[paren_idx+1:].replace(stmt['pretty'], stmt['value'])

    return e_str


'''
Replaces statement value representations with their pretty forms, if known
'''
def replace_with_pretty(e, e_str):
    # target the first stack frame
    for stmt in e['program_state']['environment']['items'][0]['items']:
        if stmt['pretty'].startswith('&code') or \
           stmt['pretty'].startswith('&SymRegion') or \
           stmt['pretty'].startswith('lazyCompoundVal') or \
           stmt['pretty'].startswith('conj_$'):
            continue
        try:
            int(stmt['value'])
            continue
        except: # not an integer constant, proceed with replacement
            paren_idx = e_str.find('(') # figure out where args start
            e_str = e_str[:paren_idx+1] + e_str[paren_idx+1:].replace(stmt['value'], stmt['pretty'])

    if e['program_state']['store']:
        for cluster in e['program_state']['store']['items']:
            for i in cluster['items']:
                paren_idx = e_str.find('(')
                e_str = e_str[:paren_idx+1] + e_str[paren_idx+1:].replace(i['value'], cluster['cluster'])

    return e_str


'''
Convert symbolic expression object into readable string
'''
def sym_expr_to_str(state, e):
    if type(e) is str: # constants will be simple strings
        return e

    if e['type'] == 'data':
        if e['subtype'] == 'conjured':
            return replace_with_value(state, e['dump'])
        elif e['subtype'] == 'derived':
            return e['region']
        else:
            print('Error: unexpected symbolic expression subtype for type `data`, exiting...')
            exit(1)
    elif e['type'] == 'binary':
        return sym_expr_to_str(state, e['lhs']) + ' ' + e['operator'] + ' ' + sym_expr_to_str(state, e['rhs'])
    else:
        print('Error: unexpected symbolic expression type in sym_expr_to_str, exiting...')
        exit(1)
    

'''
Export the generated CFG to a usable form for visualization
'''
def export_graph(G, out_file, show_constraints):

    elements = []

    def get_node_id(n):
        n_state = to_state(n)
        #id_str = ' [' + (n_state['currentID'] if 'currentID' in n_state else '0') + ']'
        id_str = ' [' + (n_state['source_range'] if 'currentID' in n_state else 'ENTRY') + ']'

        if 'caller' not in n_state:
            return n_state['event'] + id_str
            #return n_state['event'] + ' | 0 [parent = ENTRY]'
        elif 'call' in n_state['event']:
            return replace_with_value(n_state, n_state['call']) + id_str
        elif 'bind' in n_state['event']:
            lhs = n_state['bindloc_region']
            if 'bindval_nonloc_concrete_int' in n_state:
                rhs = n_state['bindval_nonloc_concrete_int']
            elif 'bindval_symbolic_expression' in n_state:
                rhs = sym_expr_to_str(n_state, n_state['bindval_symbolic_expression'])
            elif 'bindval_loc_mem_region_val' in n_state:
                rhs = n_state['bindval_loc_mem_region_val']
            else:
                print('Error: did not recognize bind RHS, exiting...')
                exit(1)
            return lhs + ' := ' + rhs + id_str
        #return n_state['caller'] + ' | ' + n_state['currentID'] + ' [parent = ' + n_state['previousID'] + ']'


    def get_edge_constraint(n):
        n_state = to_state(n)

        if 'program_state' not in n_state or \
           'constraints' not in n_state['program_state'] or \
           not n_state['program_state']['constraints']:
            return ''

        constraint_str = ''
        for c in n_state['program_state']['constraints']:
            constraint_str += replace_with_pretty(n_state, c['symbol']) + ' in range ' + c['range'] + ', '

        # strip off last comma
        return constraint_str[:-2]


    for n in G:
        node_element = {
            'group': 'nodes',
            'data': {
                'id': get_node_id(n),
                'raw': to_state(n)
            }
        }
        elements.append(node_element)

    edge_dict = {}
    for i, e in enumerate(G.edges()):
        edge_constraints = '' if not show_constraints else get_edge_constraint(e[1])
        edge_element = {
            'group': 'edges',
            'data': {
                'id': '[edge] '+get_node_id(e[0]) + ' --> ' + get_node_id(e[1]) + '(' + edge_constraints + ')',
                'source': get_node_id(e[0]),
                'target': get_node_id(e[1]),
                'constraints': edge_constraints,
            },
        }
        if edge_element not in elements:
            elements.append(edge_element)

            src_target_pair = get_node_id(e[0]) + '->' + get_node_id(e[1])
            if src_target_pair in edge_dict:
                edge_dict[src_target_pair].append(edge_element)
            else:
                edge_dict[src_target_pair] = [edge_element]

        
        def find_min_constraint(events):
            for e in events:
                if e['data']['constraints'] == '':
                    return e

            for e1 in events:
                print(e1['data']['constraints'])
                e1_constraint_dict = {c['symbol']: c['range'] for c in e1['data']['constraints']}
                min_constrained = True
                for e2 in events:
                    e2_constraint_dict = {c['symbol']: c['range'] for c in e2['data']['constraints']}

                    if len(e2_constraint_dict.keys()) < len(e1_constraint_dict.keys()):
                        continue

                    for s, r in e1_constraint_dict.items():
                        if s not in e2_constraint_dict or e2_constraint_dict[s] != r:
                            min_constrained = False
                            break

                if min_constrained:
                    return e1

            return None


        # if multiple edges go from one program location to the next and one
        # doesn't have any constraints, ignore the other edges for the viz
        for k, v in edge_dict.items():
            if len(v) <= 1:
                continue

            '''
            e_min = find_min_constraint(v)
            if e_min:
                for e in v:
                    if e != e_min:
                        elements.remove(e)
            '''

            found = False
            for e in v:
                if e['data']['constraints'] == '':
                    found = True

            if found:
                for e in v:
                    if e['data']['constraints'] != '' and e in elements:
                        elements.remove(e) 

    json.dump(elements, out_file, indent=4, separators=(',', ': '))


'''
Load all the clang analyzer events as JSON fragments. Goes through each event
json fragment and loads them individually, escaping strings that aren't easily
escaped on the C++ checker side.
'''
def load_clang_events(event_file):

    events = []

    event = ''
    for i, l in enumerate(event_file.readlines()):
        # search for beginning of next event, and start adding lines
        if event == '' and l == '{\n':
            event += l 
        elif event != '':
            # Fix escaping in call name output
            call_re = re.compile('^\"call\": \"(.*)\",$')
            m = call_re.match(l)
            if m:
                event += '\"call\": \"'+m.group(1).replace(r'"', r'\"')+'\",\n'
            # found the end of the event
            elif l == '}},\n': 
                event += '}}'
                events.append(pyjson5.decode(event))
                event = ''
            else:
                event += l

    # Use `live_symbols` state events before call or bind events (if applicable) to
    # set execution constraints
    '''
    for i, e in enumerate(events):
        # Check that we aren't considering a `live_symbols` state directly
        if e['event'] == 'live_symbols':
            continue

        # First, figure out which symbols we already have constraints on
        if e['program_state']['constraints']:
            event_constrained_symbols = {c['symbol'] : i for i, c in enumerate(e['program_state']['constraints'])}
        else:
            event_constrained_symbols = {}

        curr_idx = i - 1
        while events[curr_idx]['event'] == 'live_symbols' and curr_idx >= 0:
            if events[curr_idx]['program_state']['constraints']:
                for c in events[curr_idx]['program_state']['constraints']:
                    if c['symbol'] not in event_constrained_symbols:
                        if not e['program_state']['constraints']:
                            e['program_state']['constraints'] = []
                        e['program_state']['constraints'].append(c)
                    # Sanity check that the constraints (should) be the same
                    elif c['range'] != e['program_state']['constraints'][event_constrained_symbols[c['symbol']]]['range']: 
                        print('error: range does not match between live symbols and other event')
                        print(c['range'])
                        print(e['program_state']['constraints'][event_constrained_symbols[c['symbol']]]['range'])
                        print(e['currentID'])
                        exit(1)
            break # only look at first live_state  for now

            curr_idx -= 1
    '''

    return [e for e in events if e['event'] != 'live_symbols']

    
if __name__ == '__main__':

    parser = argparse.ArgumentParser(description='Static Analysis, Stage 1 - CFG Generation')
    parser.add_argument('app_clang_json', help='Clang checker json output')
    parser.add_argument('out_graph', help='Output location for graph (networkX format, pickled)')
    parser.add_argument('out_graph_export', help='Output location for viz export (graph in JSONish format)')
    parser.add_argument('--show_inline', action='store_true', default=False, help='Display inline calls not part of the application')
    parser.add_argument('--show_constraints', action='store_true', default=False, help='Display edge constraints')
    args = parser.parse_args()

    # First, load all of the clang analysis events in a useful form
    with open(args.app_clang_json, 'r') as f:
        clang_events = load_clang_events(f)

    print('  data loaded ({} events)'.format(len(clang_events)))

    # Identify top-level entries into application (e.g. timer callbacks, main, interrupt handlers)
    top_level_callers = set([e['caller'] for e in clang_events if \
            'previousID' in e and e['previousID'] == '0'])

    eventG = nx.DiGraph()

    # Build out graph linking events by previous/currentIDs. Start by adding
    # root event nodes, then all events, followed by graph edges
    for call in top_level_callers:
        eventG.add_node(to_node({'event': call}))
    for e in clang_events:
        eventG.add_node(to_node(e))

    frontier = []
    for e in clang_events:
        if e['caller'] in top_level_callers and int(e['previousID']) == 0:
            eventG.add_edge(to_node({'event': e['caller']}), to_node(e))
            frontier.append(e)

    # Explore the remainder of the paths breadth-first
    explored = []
    while len(frontier) != 0:
        curr_e = frontier.pop()
        if curr_e in explored:
            continue

        curr_idx = clang_events.index(curr_e)
        for next_e in clang_events[curr_idx+1:]:
            #for next_e in [e for e in clang_events if e['previousID'] == curr_e['currentID'] and e['caller'] == curr_e['caller']]:
            if next_e['currentID'] == curr_e['currentID']:
                break
            if next_e['previousID'] == curr_e['currentID']:
                eventG.add_edge(to_node(curr_e), to_node(next_e))
                frontier.append(next_e)

        explored.append(curr_e)

    print('  initial graph done')

    if not args.show_inline:
        eventG = hide_calls(eventG)
        print('  hid inline and syscalls')

    eventG = link_calls(eventG) 
    print('  linked procedure calls')

    with open(args.out_graph_export, 'w') as f:
        export_graph(eventG, f, args.show_constraints)
    print('  exported visualization graph to ' + args.out_graph_export)

    nx.write_gpickle(eventG, args.out_graph)
    print('  exported networkx graph (pickled) to ' + args.out_graph)

    print('  done.')

