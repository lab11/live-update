#include <sys/printk.h>

// #include "drivers/gpio.h"
#include "plc_platform.h"

static uint32_t gpio_in_table = 0x00000000;
static uint32_t gpio_out_table = 0x00000000;
static gpio_port_pins_t output_mask = 0x00000000;

struct device *gpio_dev;

void enable_pin_outputs(uint8_t* pins, uint8_t num_pins) {
	gpio_dev = device_get_binding("GPIO_0");
	// uint32_t out = 0;
	for (uint8_t i = 0; i < num_pins; i++) {
		// printk("Enabling PIN%d...\n", pins[i]);
		int ret = gpio_pin_configure(gpio_dev, pins[i], GPIO_OUTPUT_INACTIVE);
    	if (ret) {
	    	// printk("gpio_pin_configure failed with error code: %d\n", ret);
	    	return;
    	}
		output_mask |= (1 << pins[i]);
	}
	// For Musca
	// tfm_gpio_enable_outputs(out);

	return;
}

void enable_pin_output(uint8_t pin) {
	if (gpio_pin_configure(gpio_dev, pin, GPIO_OUTPUT_INACTIVE) != 0) {
		printk("Error! Could not enable PIN %d!\n", pin);
	}
	output_mask |= (1 << pin);
	return;
}

void read_in_table(void) {
	if (gpio_port_get_raw(gpio_dev, &gpio_in_table) != 0) {
		printk("Error! read_in_table() failed!\n");
	}
	// printk("%x\n", gpio_in_table);
	return;
}

void write_out_table(void) {
	if (gpio_port_set_masked_raw(gpio_dev, output_mask, gpio_out_table) != 0) {
		printk("Error! write_out_table() failed!\n");
	}
	// printk("%x\n\n", gpio_out_table);
	return;
}

void set_pin(uint8_t pin) {
	if (pin > 31) { return; }
	gpio_out_table |= (1 << pin);
	return;
}

void clr_pin(uint8_t pin) {
	if (pin > 31) { return; }
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

void LED_on(uint8_t led) {
	clr_pin(led);
	return;
}

void LED_off(uint8_t led) {
	set_pin(led);
	return;
}

BOOL read_LED(uint8_t led) {
	return (BOOL) read_pin(led);
}

void write_LED(uint8_t led, BOOL v) {
	if (v == 0) {
		LED_off(led);
	} else {
		LED_on(led);
	}
}

BOOL read_button(uint8_t pin) {
	return read_pin(pin) ? 0 : 1;
}

