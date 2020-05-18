#include <kernel.h>
#include <sys/printk.h>

#include "ladder.h"

#define NUM_LED_PINS 3
static uint8_t LED_pins[] __attribute__((section(".rodata"))) = {2, 3, 4};
// super hacky change this later
// this is a useful thing to tell the compiler where a variable should be stored

static uint32_t time_old = 0;
static uint32_t time_cnt = 0;

void plc_callback(struct k_timer *t) {	
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
	enable_pin_outputs(output_pins, NUM_PINS);


	if (USE_LED) {
		enable_pin_outputs(LED_pins, NUM_LED_PINS);
		LED_off(R);
		LED_off(G);
		LED_off(B);
	}

	write_out_table();
	printk("plc initialized\n");
}

/**************** Define Auto-Generated Functions Below ****************/

BOOL Read_U_b_Xbutton(void) {
	BOOL state = (BOOL) read_button(PIN7);
	state = state ? 0 : 1;		// swapping because button is active low
	// printk("Button State: %d\n", state);
	return state;
}

BOOL Read_U_b_Yled(void) {
	return (BOOL) read_LED(B);
}

void Write_U_b_Yled(BOOL v) {
	write_LED(B, v);
}

BOOL Read_U_b_Ytx(void) {
	return (BOOL) read_LED(G);
}

void Write_U_b_Ytx(BOOL v) {
	write_LED(G, v);
}
