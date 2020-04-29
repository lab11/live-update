import json
import networkx as nx
from itertools import chain, combinations
import pygraphviz

def powerset(iterable):
    s = list(iterable)
    return [list(x) for x in chain.from_iterable(combinations(s, r) for r in range(len(s) + 1))]


#with open('test_graph.json', 'r') as f:
with open('pacemaker.json', 'r') as f:
    data = json.load(f)

# timer expirations mean the timer is implicitly stopped
timer_expiration_to_timer_map = {}
for timer in data['timers']:
    timer_val = data['timers'][timer]
    if not timer_val['expire_callback']:
        continue

    timer_expiration_to_timer_map[timer_val['expire_callback']] = timer
    data['top_level_fns'][timer_val['expire_callback']]['timers_stopped'].append(timer)

G = nx.DiGraph()
G.add_nodes_from([('waiting',tuple(sorted(set(x)))) for x in powerset(data['timers'])])

for fn in data['top_level_fns']:

    potential_active_timer_lists = powerset(data['timers'])

    # special case main
    if fn == 'main':
        possible_active_timer_lists = [[]]
    elif fn in timer_expiration_to_timer_map:
        possible_active_timer_lists = [l for l in potential_active_timer_lists if timer_expiration_to_timer_map[fn] in l]
    else:
        possible_active_timer_lists = potential_active_timer_lists

    #print(fn)
    #print(possible_active_timer_lists)

    for active_timers in possible_active_timer_lists:
        G.add_node((fn+'_stop', tuple(sorted(set(active_timers)))))
        G.add_edge(('waiting',tuple(sorted(set(active_timers)))), (fn+'_stop', tuple(sorted(set(active_timers)))))

        timers_after_stop = [t for t in active_timers if t not in data['top_level_fns'][fn]['timers_stopped'] and t not in data['top_level_fns'][fn]['timers_started']]
        G.add_node((fn+'_start', tuple(sorted(set(timers_after_stop)))))
        G.add_edge((fn+'_stop', tuple(sorted(set(active_timers)))), (fn+'_start', tuple(sorted(set(timers_after_stop)))))

        timers_after_start = timers_after_stop + data['top_level_fns'][fn]['timers_started']
        G.add_edge((fn+'_start', tuple(sorted(set(timers_after_stop)))), ('waiting', tuple(sorted(set(timers_after_start)))))

#print(G.nodes())
start_node = ('main_stop', ())
to_remove = []
for n in G.nodes():
    if not nx.has_path(G, start_node, n):
        to_remove.append(n)

for n in to_remove:
    G.remove_node(n)

A = nx.nx_agraph.to_agraph(G)
A.layout('dot')
A.draw('timer_graph.png')

