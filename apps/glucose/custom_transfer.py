''' 
Custom state transfer routine. Returns safe range for new state variables
given the specific update point or None to indicate an unknown symbol.
'''
def custom_state_transfer(symbol, predicate, constraints):
    return []

'''
Custom hardware state transfer routine. Returns a list of hw syscalls
to be applied given a specific predicate or None if we can't handle
that case.
'''
def custom_hardware_transfer(predicate):
    return []

