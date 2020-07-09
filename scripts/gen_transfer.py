import argparse
import ast
from collections import defaultdict
from copy import deepcopy
from elftools.elf.elffile import ELFFile
import importlib.util
import json
import networkx as nx
import operator as op
import os
import pprint
import pygraphviz
import pyjson5
import re
import sys


# https://stackoverflow.com/questions/4383571/importing-files-from-different-folder
def module_from_file(module_name, file_path):
    spec = importlib.util.spec_from_file_location(module_name, file_path)
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module
    

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


def get_symbol_name(filename, addr_re):
    name = None
    def cb(l):
        nonlocal name
        name = l.split()[-1]
    search_symbols(filename, format(addr_re, 'x'), cb)
    return name


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
Expression evaluation
https://stackoverflow.com/questions/2371436/evaluating-a-mathematical-expression-in-a-string
'''
operators = {ast.Add: op.add, ast.Sub: op.sub, ast.Mult: op.mul, ast.Div: op.truediv,
        ast.Pow: op.pow, ast.BitXor: op.xor, ast.USub: op.neg,
        ast.LShift: op.lshift, ast.RShift: op.rshift, ast.BitOr: op.or_, ast.BitAnd: op.and_}


def eval_(node):
    if isinstance(node, ast.Num):
        return node.n
    elif isinstance(node, ast.BinOp):
        return operators[type(node.op)](eval_(node.left), eval_(node.right))
    elif isinstance(node, ast.UnaryOp):
        return opreators[type(node.op)](eval_(node.operand))
    else:
        raise TypeError(node)


def eval_expr(expr):
    return eval_(ast.parse(expr, mode='eval').body)


INIT_CALLS = [
    'gpio_pin_configure',
    'gpio_pin_interrupt_configure',
    'gpio_nrfx_init_callback',
    'gpio_add_callback',
    'k_timer_init',
]

def gen_hw_init(G, path):

    path = [to_state(e) for e in path]

    # Only generate HW init instructions for main
    if path[0]['event'] != 'main':
        return None

    init = []

    for e in path:
        if 'call' in e:
            fn_call = repr(e['call'])[1:-1] # strip quotes

            for ic in INIT_CALLS:
                if fn_call.startswith(ic): # found init API call that we want to pay attention to

                    curr_idx = fn_call.find('(')

                    args = []
                    while curr_idx < len(fn_call):

                        # look for a match to the current argument; better than nested parend regexes
                        found = False
                        for item in e['program_state']['environment']['items'][0]['items']:
                            if fn_call[curr_idx:].startswith(item['pretty']):
                                args.append(item['pretty'])
                                curr_idx += len(item['pretty'])
                                found = True
                                break

                        if not found:
                            curr_idx += 1

                    init.append({
                        'function': fn_call[:fn_call.find('(')],
                        'args': args,
                    })

    # post-process initialization calls

    # Evaluate expanded constants, etc.
    for i in init:
        evaluated_args = []
        for a in i['args']:
            try:
                # Get rid of U notation on unsigned vals
                constant_unsigned_re = re.compile('([0-9a-f]+U)')
                m = constant_unsigned_re.findall(a)
                if m:
                    for g in m:
                        a = a.replace(g, g[:-1]) # remove U

                # Get rid of L notation on long vals
                long_re = re.compile('([0-9a-f]+L)')
                m = long_re.findall(a)
                if m:
                    for g in m:
                        a = a.replace(g, g[:-1]) # remove L
                
                evaluated_args.append(eval_expr(a))
            except:
                evaluated_args.append(a)

        i['args'] = evaluated_args

    return init


EVENT_RESET_CALLS = [
    'k_timer_stop',
    'k_timer_start'
]

def gen_predicate(G, path):

    path = [to_state(e) for e in path]

    # Don't generate predicates for main
    if path[0]['event'] == 'main':
        return None

    predicate = {
        'event_name': path[0]['event'],
        'reset_ops': [],
        'constraints': []
    }

    for e in path:
        if 'call' in e:
            fn_call = repr(e['call'])[1:-1] # strip quotes

            for erc in EVENT_RESET_CALLS:
                if fn_call.startswith(erc): # found reset API call for an event
                    paren_idx = fn_call.find('(')
                    
                    # get call elements and check relevant argument
                    # XXX: assumes first argument is the reset operation state object
                    for item in e['program_state']['environment']['items'][0]['items']:
                        if fn_call[paren_idx+1:].startswith(item['pretty']):
                            predicate['reset_ops'].append(item['pretty'])

        if 'program_state' in e and 'constraints' in e['program_state'] and e['program_state']['constraints']:
            for c in e['program_state']['constraints']:
                # strip curly braces
                c['range'] = c['range'].replace('{', '[')
                c['range'] = c['range'].replace('}', ']')

                constraint = {
                    'symbol': '',
                    'range': to_state(c['range']), 
                }

                sym = c['symbol']
                if sym.startswith('derived_'): # handle derived symbols from globals
                    derived_re = re.compile('[^\{]+\{(.+),(.+)\}')
                    m = derived_re.match(sym)
                    if m:
                        pretty_symbol = m.group(2)
                        store_item = m.group(1)

                        # verify that this is a global
                        is_global = False
                        for store_cluster in e['program_state']['store']['items']:
                            if store_cluster['cluster'] == 'GlobalInternalSpaceRegion':
                                for i in store_cluster['items']:
                                    if i['value'] == store_item:
                                        is_global = True
                                        break
                        
                        if not is_global:
                            print('Error, constraint symbol is not global')
                            pprint.pprint(c)
                            pprint.pprint(e)
                            exit(1)

                        constraint['symbol'] = pretty_symbol

                    else:
                        print('Error, constraint symbol failed derived regex')
                        pprint.pprint(c)
                        pprint.pprint(e)
                        exit(1)

                elif sym.startswith('reg_'): # handle globals in register vals
                    reg_re = re.compile('[^\<]+\<(.*)\s(\S+)\>')
                    m = reg_re.match(sym)
                    if m:
                        pretty_symbol = m.group(2)
                        constraint['symbol'] = pretty_symbol
                    else:
                        print('Error, constraint symbol failed register regex')
                        pprint.pprint(c)
                        pprint.pprint(e)
                        exit(1)
                    
                else:
                    print('Error, constraint symbol could not be parsed')
                    pprint.pprint(e)
                    exit(1)

                predicate['constraints'].append(constraint)


    # post-process predicate, if needed

    # trim ampersands from pretty names
    trimmed_resets = []
    for r in predicate['reset_ops']:
        if r[0] == '&':
            trimmed_resets.append(r[1:])
    if trimmed_resets:
        predicate['reset_ops'] = trimmed_resets

    # remove duplicate constraints 
    predicate['constraints'] = [to_state(unique_c) for unique_c in {to_node(c) for c in predicate['constraints']}]

    return predicate

'''
Generates map[src symbol] = (dest symbol, size) relation for state transfer.
'''
def generate_state_transfer_map(original_symbol_f, update_symbol_f):

    original_symbols = get_symbols_in_section(original_symbol_f, '.app_data') + \
                       get_symbols_in_section(original_symbol_f, '.app_bss')
    original_symbols = [o for o in original_symbols if get_symbol_size(original_symbol_f, o+'$') != 0]

    update_symbols = get_symbols_in_section(update_symbol_f, '.app_data') + \
                     get_symbols_in_section(update_symbol_f, '.app_bss')
    update_symbols = [u for u in update_symbols if get_symbol_size(update_symbol_f, u+'$') != 0]

    transfer_map = {}

    for o_sym in original_symbols:
        if o_sym in update_symbols:
            size = get_symbol_size(original_symbol_f, o_sym+'$')
            if size != get_symbol_size(update_symbol_f, o_sym+'$'):
                print('error, expected', o_sym, 'to be', size, 'bytes in the update, but got', get_symbol_size(update_symbol_f, o_sym+'$'), 'instead')
                exit(1)

            transfer_map[o_sym] = (o_sym, size)

    unmapped_state = []
    for u_sym in update_symbols:
        if u_sym not in transfer_map: # didn't carry over from old app version
            unmapped_state.append(u_sym)

    return transfer_map, unmapped_state


'''
Generates list of calls [(fn ptr, args...)] for hardware reinitialization on update
'''
def generate_hw_init_calls(original_predicates_and_init, update_predicates_and_init, unmapped_symbols):

    init_calls = []

    orig_strs = [to_node(o) for o in original_predicates_and_init['init'][0]]
    update_strs = [to_node(u) for u in update_predicates_and_init['init'][0]]

    inits = [to_state(u) for u in update_strs]
    for ni in inits:

        fn = ni['function']
        args = []
        for a in ni['args']:
            if a == '((void *)0)': # null
                args.append(0)
            else:
                args.append(a)

        # special case some API calls
        if ni['function'] == 'k_timer_init':
            timer_name = args[0] if args[0][0] != '&' else args[0][1:] # strip the ampersand to match symbols
            if timer_name in unmapped_symbols:
                unmapped_symbols.remove(timer_name)

            init_calls.append((fn, *args))

        # essentially recreate function because of inlining
        # XXX: doesn't support ACTIVE_LOW right now, or 
        elif ni['function'] == 'gpio_pin_configure':
            port, pin, flags = args
            init_calls.append(('gpio_nrfx_config', port, pin, flags))

        elif ni['function'] == 'gpio_pin_interrupt_configure':
            port, pin, flags = args
            init_calls.append(('z_impl_gpio_pin_interrupt_configure', port, pin, flags))

        elif ni['function'] == 'gpio_nrfx_init_callback':
            cb = args[0] if args[0][0] != '&' else args[0][1:] # strip the ampersand if there
            if cb in unmapped_symbols:
                unmapped_symbols.remove(cb)

            init_calls.append((fn, *args))

        elif ni['function'] == 'gpio_add_callback':
            port, callback = args
            init_calls.append(('gpio_nrfx_manage_callback', port, callback))

        else:
            print('unrecognized hw init function', ni['function'], 'exiting...')
            exit(1)

    return init_calls, unmapped_symbols


def matching_constraints(orig_predicate, translated_constraints, updated_predicate, state_transfer_map, unmapped_symbols):

    orig_constraint_map = {tc['symbol']: tc['range'] for tc in translated_constraints}
    update_constraint_map = {uc['symbol']: uc['range'] for uc in updated_predicate['constraints']}

    # generate constraints on new symbols in the update, using lower bound as the initialization value
    new_inits = []
    for unmapped_sym in unmapped_symbols:
        unmapped_sym_range = custom_transfer.custom_state_transfer(unmapped_sym, orig_predicate, translated_constraints)
        if unmapped_sym_range == None:
            print('Error: required manual state transfer for new state symbol', unmapped_sym, 'missing.')
            exit(1)
        orig_constraint_map[unmapped_sym] = unmapped_sym_range
        # symbol, offset, value
        new_inits.append((unmapped_sym, 0, unmapped_sym_range[0][0]) if type(unmapped_sym_range[0]) == list else (unmapped_sym, 0, unmapped_sym_range[0]))

    for u_sym in update_constraint_map:
        if u_sym not in orig_constraint_map:
            if u_sym in state_transfer_map: # updated predicate has an additional constraint we don't satisfy
                return False, None

    # actually check the constraint ranges now
    for u_sym in update_constraint_map:
        update_ranges = deepcopy(update_constraint_map[u_sym])
        original_ranges = deepcopy(orig_constraint_map[u_sym])

        if type(original_ranges[0]) != list:
            original_ranges = [original_ranges]

        if type(update_ranges[0]) != list:
            update_ranges = [update_ranges]

        for o in original_ranges:
            found_matching = False
            for i, u in enumerate(update_ranges):
                if o[0] == u[0] and o[1] == u[1]:
                    found_matching = True
                    update_ranges.pop(i)
                    break

            if not found_matching:
                return False, None

    return True, new_inits


''' 
For each original predicate, "transfer" state into the update and make sure
there's a matching predicate. Requires manual state transfer for unknown
symbols by the application developer.
'''
def match_predicates_and_custom_transfer(original_predicates_and_init, update_predicates_and_init,
                                         state_transfer_map, hw_init_calls, unmapped_symbols):

    predicate_triples = []

    # Find a compatible updated predicate for the original predicate. We must be able to transform the original predicate
    # into the updated one by:
    #   - keeping the event name the same
    #   - ensuring the updated predicate resets all of the original operations, *at least*
    #   - translating constraint symbols with state_transfer_map, asking for the values of any unmapped symbols
    #   - ensuring that the updated constraints are as constrained (or less) than the original constraints
    for op in original_predicates_and_init['predicates']:
        for up in update_predicates_and_init['predicates']:

            if op['event_name'] != up['event_name']:
                continue

            reset = True
            for original_reset_op in op['reset_ops']:
                if original_reset_op not in up['reset_ops']:
                    reset = False
                    break

            if not reset:
                continue

            translated_constraints = [{
                'symbol': state_transfer_map[oc['symbol']][0],
                'range': oc['range'],
            } for oc in op['constraints']]

            matching, custom_state = matching_constraints(op, translated_constraints, up, state_transfer_map, unmapped_symbols)
            if matching:
                '''
                print()
                print('matching')
                print('op')
                pprint.pprint(op)
                print('up')
                pprint.pprint(up)
                pprint.pprint(custom_state)
                print()
                '''

                custom_hw = custom_transfer.custom_hardware_transfer(up)
                if custom_hw == None:
                    print('Error: expected custom hardware transfer not provided (None)')
                    exit(1)
                predicate_triples.append((op, custom_state, custom_hw))

    return predicate_triples


def generate_state_init_items(original_predicates_and_init, update_predicates_and_init, hw_init_calls):
    state_init_tuples = []

    # for every timer in the updated version, set the expiry callback appropriately (will be incorrect due to copy)
    # for every gpio interrupt callback in the updated version, set the callback appropriately (will be incorrect due to copy)
    # XXX not used because we reinit the structures as hw items anyway, but possibility is still here
    '''
    TIMER_EXPIRY_CB_OFFSET = 24
    GPIO_INTERRUPT_CB_OFFSET = 4
    for init_entry in update_predicates_and_init['init'][0]:
        if init_entry['function'] == 'k_timer_init':
            timer_name = init_entry['args'][0]
            if timer_name[0] == '&':
                timer_name = timer_name[1:]
            state_init_tuples.append((timer_name, TIMER_EXPIRY_CB_OFFSET, init_entry['args'][1]))

        if init_entry['function'] == 'gpio_init_callback':
            gpio_data = init_entry['args'][0]
            if gpio_data[0] == '&':
                gpio_data = gpio_data[1:]
            state_init_tuples.append((gpio_data, GPIO_INTERRUPT_CB_OFFSET, init_entry['args'][1]))
    '''

    # TODO other initialization

    return state_init_tuples


if __name__ == '__main__':

    parser = argparse.ArgumentParser(description='Static Analysis, Stage 3 - Transfer Instruction Generation')
    parser.add_argument('app_dir', help='Application directory, with state transfer function')
    parser.add_argument('in_original_predicates', help='Original predicates and hw init instructions (JSON)')
    parser.add_argument('in_original_symbols', help='Original application symbol table dump (e.g. with objdump)')
    parser.add_argument('in_update_predicates', help='Update predicates and hw init instructions (JSON)')
    parser.add_argument('in_update_symbols', help='Updated application symbol table dump (e.g. with objdump)')
    parser.add_argument('out', help='Output location for predicates hw RE-init instructions, and transfer instructions (JSON)')
    args = parser.parse_args()

    with open(args.in_original_predicates, 'r') as f:
        original_predicates_and_init = pyjson5.load(f)

    with open(args.in_update_predicates, 'r') as f:
        update_predicates_and_init = pyjson5.load(f)

    custom_transfer = module_from_file('custom_transfer', args.app_dir + '/custom_transfer.py')

    original_symbols = args.in_original_symbols
    update_symbols = args.in_update_symbols
    
    state_transfer_map, unmapped_symbols = \
            generate_state_transfer_map(original_symbols, update_symbols)
    hw_init_calls, unmapped_symbols = \
            generate_hw_init_calls(original_predicates_and_init, update_predicates_and_init, unmapped_symbols)
    predicate_triples = \
            match_predicates_and_custom_transfer(original_predicates_and_init, update_predicates_and_init,
                                                 state_transfer_map, hw_init_calls, unmapped_symbols)
    state_init_items = \
            generate_state_init_items(original_predicates_and_init, update_predicates_and_init, hw_init_calls)

    out = {
    }
    
    with open(args.out, 'w') as f:
        json.dump({ 
            'state_transfer': state_transfer_map,
            'predicate_transfer': predicate_triples,
            'hw_init': hw_init_calls,
            'mem_init': state_init_items,
        }, f)

