#include <sys/printk.h>

#include "tfm_gpio_veneers.h"
#include "plc_platform.h"

static uint32_t gpio_in_table = 0x00000000;
static uint32_t gpio_out_table = 0x00000000;

// read CYCCTR
uint32_t cntr;

void enable_pin_outputs(uint8_t* pins, uint8_t num_pins) {
	uint32_t out = 0;
	for (uint8_t i = 0; i < num_pins; i++) {
		out |= (1 << pins[i]);
	}
	printk("Enabling Outputs: %x\n", out);
	tfm_gpio_enable_outputs(out);
	return;
}

void enable_pin_output(uint8_t pin) {
	if (tfm_gpio_enable_output(pin) != 0) {
		printk("Error! Cannot Enable Pin!");
	}
	return;
}

void read_in_table(void) {
	gpio_in_table = tfm_gpio_read_all();
	if (((gpio_in_table >> 7) & 1) == 0) {
        cntr = *(uint32_t *) 0xE000E018;
    }
	// printk("%x\n", gpio_in_table);
	return;
}

void write_out_table(void) {
	if (((gpio_out_table >> 3) & 1) == 0) {
        uint32_t tmp = *(uint32_t *) 0xE000E018;
        printk("Elapsed Time from Cycle Counter: %d\n\n", cntr - tmp);
    }
	// printk("%x\n", gpio_out_table);
	tfm_gpio_write_all(gpio_out_table);
	return;
}

void set_pin(uint8_t pin) {
	if (pin > 15) { return; }
	gpio_out_table |= (1 << pin);
	return;
}

void clr_pin(uint8_t pin) {
	if (pin > 15) { return; }
	gpio_out_table &= ~(1 << pin);
	return;
}

void write_pin(uint8_t pin, BOOL val) {
	if (val) {
		set_pin(pin);
	} else {
		clr_pin(pin);
	}
	return;
}

BOOL read_pin(uint8_t pin) {
	if (pin > 15) { return pin; }
	return (BOOL) (gpio_in_table >> pin) % 2;
}

void LED_on(LED color) {
	clr_pin(color);
	return;
}

void LED_off(LED color) {
	set_pin(color);
	return;
}

BOOL read_LED(LED color) {
	return (BOOL) read_pin(color);
}

void write_LED(LED color, BOOL v) {
	if (v == 0) {
		LED_off(color);
	} else {
		LED_on(color);
	}
}

BOOL read_button(uint8_t pin) {
	return read_pin(pin);
}

