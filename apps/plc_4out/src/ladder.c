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

	read_in_table();
	PlcCycle();
	write_out_table();
}

void init_plc(void) {
	for(int i = 0; i < NUM_PINS; i++) {
		printk("Using PIN%d\n", output_pins[i]);
	}
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

/* Debugging Variables */
BOOL master_state = 0;
BOOL out0_state = 0;
BOOL out1_state = 0;
BOOL out2_state = 0;
BOOL out3_state = 0;
/* */

BOOL readStart;

BOOL Read_U_b_XMStart(void) {
	readStart = read_button_active_low(PIN4);
	// printk("%d\n", readStart);

	if (master_state != readStart) {
		master_state = readStart;
		if (readStart) {
			time_old = k_uptime_get_32();
			printk("4 Output Started...\n");			
		}
	} 
	return readStart;
}

BOOL Read_U_b_XStop(void) {
	return read_button_active_low(PIN7);
}

BOOL Read_U_b_YOut0(void) {
	return read_pin(PIN8);
}

void Write_U_b_YOut0(BOOL v) {
	if (out0_state != v) {
		out0_state = v;
		if (v) {
			time_cnt = k_uptime_get_32();
			printk("Output 0 Enabled at %d ms\n", time_cnt - time_old);			
		}
	}
	write_pin(PIN8, v);
}

BOOL Read_U_b_YOut1(void) {
	return read_pin(PIN9);
}

void Write_U_b_YOut1(BOOL v) {
	if (out1_state != v) {
		out1_state = v;
		if (v) {
			time_cnt = k_uptime_get_32();
			printk("Output 1 Enabled at %d ms\n", time_cnt - time_old);			
		}
	}
	write_pin(PIN9, v);
}

BOOL Read_U_b_YOut2(void) {
	return read_pin(PIN10);
}

void Write_U_b_YOut2(BOOL v) {
	if (out2_state != v) {
		out2_state = v;
		if (v) {
			time_cnt = k_uptime_get_32();
			printk("Output 2 Enabled at %d ms\n", time_cnt - time_old);			
		}
	}
	write_pin(PIN10, v);
}

BOOL Read_U_b_YOut3(void) {
	return read_pin(PIN11);
}

void Write_U_b_YOut3(BOOL v) {
	if (out3_state != v) {
		out3_state = v;
		if (v) {
			time_cnt = k_uptime_get_32();
			printk("Output 3 Enabled at %d ms\n", time_cnt - time_old);			
		}
	}
	write_pin(PIN11, v);
}




