#pragma once

#include "tfm_gpio_veneers.h"

typedef signed short SWORD;
typedef unsigned char BOOL;

#define PIN0 0
#define PIN1 1
#define PIN2 2		/* Do Not Use if LED Jumpers are Connected */
#define PIN3 3		/* Do Not Use if LED Jumpers are Connected */
#define PIN4 4		/* Do Not Use if LED Jumpers are Connected */
#define PIN5 5
#define PIN6 6
#define PIN7 7
#define PIN8 8
#define PIN9 9
#define PIN10 10
#define PIN11 11
#define PIN12 12
#define PIN13 13
#define PIN14 14
#define PIN15 15

typedef enum {
	R = 2,
	G = 3,
	B = 4
} LED;

void enable_pin_outputs(uint8_t* pins, uint8_t num_pins);

void enable_pin_output(uint8_t pin);

void read_in_table(void);

void write_out_table(void);

/**************** For User ****************/

void set_pin(uint8_t pin);

void clr_pin(uint8_t pin);

void write_pin(uint8_t pin, BOOL val);

BOOL read_pin(uint8_t pin);

void LED_on(LED color);

void LED_off(LED color);

BOOL read_LED(LED color);

void write_LED(LED color, BOOL v);

BOOL read_button_active_low(uint8_t pin);
