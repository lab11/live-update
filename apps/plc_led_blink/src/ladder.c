#include <kernel.h>
#include <sys/printk.h>

#include "ladder.h"

#define NUM_LED_PINS 3
static uint8_t LED_pins[] = {2, 3, 4};

void plc_callback(struct k_timer *t) {	
	read_in_table();
	PlcCycle();
	write_out_table();
}

void init_plc(void) {
	enable_pin_outputs(output_pins, NUM_PINS);

	if (USE_LED) {
		enable_pin_outputs(LED_pins, NUM_LED_PINS);
		LED_off(R);
		LED_off(G);
		LED_off(B);
	}

	printk("plc initialized\n");
}

/**************** Define Auto-Generated Functions Below ****************/

BOOL Read_U_b_Xbutton(void) {
	BOOL state = (BOOL) read_button(PIN7);
	printk("Button State: %d/n", state);
	return state;
}

BOOL Read_U_b_Yled(void) {
	return (BOOL) read_LED(R);
}

void Write_U_b_Yled(BOOL v) {
	write_LED(R, v);
}
