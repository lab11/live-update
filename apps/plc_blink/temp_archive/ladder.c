// #include <kernel.h>
#include <sys/printk.h>

#include "ladder.h"

#define NUM_LED_PINS 3
static uint8_t LED_pins[] __attribute__((section(".rodata"))) = {2, 3, 4, 5};
// super hacky change this later
// this is a useful thing to tell the compiler where a variable should be stored

// static uint32_t time_old = 0;
// static uint32_t time_cnt = 0;

void plc_callback(struct k_timer *t) {	
	// printk("Timer Fired!!\n\n");
	// time_old = time_cnt;
	// time_cnt = k_uptime_get_32();
	// printk("elapsed time since last callback: %d\n", time_cnt - time_old);
	// printk("Reading...\n");
	read_in_table();
	// printk("Executing...\n");
	PlcCycle();
	// printk("Writing...\n");
	write_out_table();
	// printk("Scan Complete.\n");
}

void init_plc(void) {
	printk("Initializing PLC...\n");

	enable_pin_inputs(input_pins, NUM_INPUTS);
	printk("Inputs Initialized!\n");

	read_in_table();

	uint8_t num_pins = NUM_PINS;

	// enable_pin_outputs(LED_pins, num_pins);

	if (USE_LED) {
		printk("LEDs in use...\n");
		num_pins += 4;
		uint8_t tmp[num_pins];
		int i = 0;
		for (int j = 0; j < NUM_PINS; j++) {
			tmp[i] = output_pins[j];
			printk("Adding pin %d...\n", tmp[i]);
			i++;
		}
		for (int k = 0; k < 4; k++) {
			tmp[i] = k + 2;
			printk("Adding pin %d...\n", tmp[i]);
			i++;
		}
		enable_pin_outputs(tmp, num_pins);
	} else {
		enable_pin_outputs(output_pins, num_pins);
	}

	write_out_table();
	// printk("plc initialized\n");
}

/**************** Define Auto-Generated Functions Below ****************/

BOOL Read_U_b_Xbutton(void) {
	return (BOOL) read_button(BUTTON1);
}

BOOL Read_U_b_Yled(void) {
	return (BOOL) read_LED(LED1);
}

void Write_U_b_Yled(BOOL v) {
	write_LED(LED1, v);
}

BOOL Read_U_b_Ytx(void) {
	return (BOOL) read_LED(LED4);
}

void Write_U_b_Ytx(BOOL v) {
	write_LED(LED4, v);
}
