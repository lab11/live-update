#pragma once

#include "drivers/gpio.h"

typedef signed short SWORD;
typedef unsigned char BOOL;


void enable_pin_outputs(uint8_t* pins, uint8_t num_pins);

void enable_pin_output(uint8_t pin);

void read_in_table(void);

void write_out_table(void);

/**************** For User ****************/

void set_pin(uint8_t pin);

void clr_pin(uint8_t pin);

void write_pin(uint8_t pin, BOOL val);

BOOL read_pin(uint8_t pin);

void LED_on(uint8_t led);

void LED_off(uint8_t led);

BOOL read_LED(uint8_t led);

void write_LED(uint8_t led, BOOL v);

BOOL read_button(uint8_t pin);
