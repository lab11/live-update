''' 
Custom state transfer routine. Returns safe range for new state variables
given the specific update point or None to indicate an unknown symbol.
'''
def custom_state_transfer(symbol, predicate, constraints):
    return None

'''
Custom hardware state transfer routine. Returns a list of hw syscalls
to be applied given a specific predicate or None if we can't handle
that case.
'''
def custom_hardware_transfer(predicate):
    for c in predicate['constraints']:
        if c['symbol'] == 'toggled':
            for r in c['range']:
                if r[0] <= 0 and 0 <= r[1]:
                    return [
                        ('gpio_nrfx_port_set_bits_raw', 'gpio_dev', 1 << 3),
                        ('gpio_nrfx_port_clear_bits_raw', 'gpio_dev', 1 << 2),
                    ]
                elif r[0] <= 1 and 1 <= r[1]:
                    return [
                        ('gpio_nrfx_port_clear_bits_raw', 'gpio_dev', 1 << 3)
                    ]
    return None

