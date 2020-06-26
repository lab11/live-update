# Bottle Wash Expected Behavior

On `Start` input, the program begins and continues until it receives a `Stop` input.

1. Controller detects a bottle (`Bottle_IN` input triggers and begins holding the bottle)

1. Give servo 1s to clamp the bottle

1. Motor begins rotating bottle (`Motor_FORWARD` output on) until it until a sensor indicates upside-down position (`Position_DOWN` input triggers) at which point it stops. (`Motor_FORWARD` output off)

1. Once stopped, the water turns on (`water_ON` output on) and stays on for 5s

1. After water ends, Motor rotates bottle back upright (`Motor_REVERSE` output on) until sensor indicated upright position (`Bottle_UP` input on) at which point it stops. (`Motor_REVERSE` output off)

1. Once stopped, the servo waits 1s before releasing the bottle (`Bottle_RELEASE` output on)



So variables should trigger in this order:

1. Input: `Start` 1 (Button)
1. Input: `Bottle_IN` 1 Output: `Bottle_HOLD` 1
1. Output: `Motor_FORWARD` 1
1. Input: `Position_DOWN` 1 Output: `Motor_FORWARD` 0
1. Output: `water_ON` 1, 0
1. Output: `Motor_REVERSE`1
1. Input: `Bottle_UP` 1 Output: `Motor_REVERSE` 0
1. Output: `Bottle_RELEASE` 1

# Pin Mapping

NRF -> MUSCA 

PIN13	->	PIN7
PIN14	->	PIN6

PIN2 	->	PIN4
PIN26	->	PIN3
PIN27	->	PIN2

PIN22	->	PIN8
PIN20	->	PIN9
PIN19	->	PIN10
PIN18	->	PIN11
PIN17	->	PIN12