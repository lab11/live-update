import argparse
import ast
from collections import defaultdict
from copy import deepcopy
from elftools.elf.elffile import ELFFile
import json
import networkx as nx
import operator as op
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


def gen_split_phase(G, path):

    path = [to_state(e) for e in path]

    # Only list split phase ops for main
    if path[0]['event'] != 'main':
        return None

    ops = []

    for e in path:
        if 'call' in e:
            fn_call = repr(e['call'])[1:-1] # strip quotes
            if fn_call.startswith('k_timer_init'): # found split-phase timer

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

                ops.append(args[0][1:] if args[0][0] == '&' else args[0]) # strip ampersand for timer struct if there

    return ops



EVENT_RESET_CALLS = [
    'k_timer_stop',
    'k_timer_start'
]

def gen_predicate(G, path, split_phase_ops):

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

                elif sym.startswith('((derived_') or sym.startswith('conj_$'): # XXX problems
                    continue 
                else:
                    print('Warning: constraint symbol {} could not be parsed'.format(sym))
                    #pprint.pprint(e)
                    #exit(1)
                    continue

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

    # add list of ops that should be inactive
    inactive_ops = deepcopy(split_phase_ops)
    for r in predicate['reset_ops']:
        if r in inactive_ops:
            inactive_ops.remove(r)
    predicate['inactive_ops'] = inactive_ops

    return predicate


if __name__ == '__main__':

    parser = argparse.ArgumentParser(description='Static Analysis, Stage 2 - Predicate Instruction Generation')
    parser.add_argument('in_graph', help='Path for graph (networkX format, pickled)')
    parser.add_argument('out', help='Output location for predicates and hw init instructions (JSON)')
    args = parser.parse_args()
    
    eventG = nx.read_gpickle(args.in_graph)

    '''
    Path Parsing
    '''
    print('  generating paths...')
    
    # map of call entry -> graph node, indexed by top-level call name
    entry_nodes = {}
    for n in eventG:
        if len(list(eventG.predecessors(n))) == 0 and len(list(eventG.successors(n))) != 0:
           entry_nodes[to_state(n)['event']] = n
     
    paths = []
    for entry_fn in entry_nodes:
        for n in eventG:
            if len(list(eventG.successors(n))) != 0: # ignore non-leaf nodes
                continue

            paths += list(nx.all_simple_paths(eventG, entry_nodes[entry_fn], n))   

    '''
    Predicate Generation
    '''
    print('  generating predicates...')

    # get all possible split-phase ops (i.e. timers)
    split_phase_ops = []
    for p in paths:
        sp = gen_split_phase(eventG, p)
        if sp:
            split_phase_ops += sp
    
    split_phase_ops = list(set(split_phase_ops))

    predicates = []
    for p in paths:
        predicate = gen_predicate(eventG, p, split_phase_ops)
        if predicate:
            predicates.append(predicate)

    # remove duplicate predicates
    predicates = [to_state(unique_p) for unique_p in {to_node(p) for p in predicates}]

    '''
    Hardware Initialization
    '''
    print('  generating hardware initialization...')
    init_instructions = []
    for p in paths:
        hw_init = gen_hw_init(eventG, p)
        if hw_init:
            init_instructions.append(hw_init)

    # remove duplicate init instructions
    init_instructions = [to_state(unique_i) for unique_i in {to_node(i) for i in init_instructions}]

    # eliminate subsets of init instructions, due to potential error cases we don't care about
    actual_init_instructions = []
    for hw1 in init_instructions:
        subset = False

        hw1_set = {to_node(h) for h in hw1}
        for hw2 in init_instructions:
            hw2_set = {to_node(h) for h in hw2}

            if hw1_set == hw2_set:
                continue

            subset = hw1_set.issubset(hw2_set)
            if subset:
                break

        if not subset:
            actual_init_instructions.append(hw1)

    init_instructions = actual_init_instructions

    '''
    Output
    '''
    print('  writing output to {}...'.format(args.out))

    with open(args.out, 'w') as f:
        print(pprint.pformat({
            'predicates': predicates,
            'init': init_instructions,
        }), file=f)
        print('', file=f)

    print('  done.')

