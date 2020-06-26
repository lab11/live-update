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

/* */

BOOL Read_U_b_XStart(void) { // button input
	return read_button_active_low(Start_PIN);
}

BOOL Read_U_b_XStop(void) { // button input
	return read_button_active_low(Stop_PIN);
}

BOOL Read_U_b_XBottle_IN(void) { // sensor input
	return read_button_active_low(Bottle_IN_PIN);
}

BOOL Read_U_b_XBottle_UP(void) { // sensor input
	return read_button_active_low(Bottle_UP_PIN);
}

BOOL Read_U_b_XPosition_DOWN(void) { // sensor input
	return read_button_active_low(Position_DOWN_PIN);
}


BOOL Read_U_b_YBottle_HOLD(void) {
	return read_pin(Bottle_HOLD_PIN);
}

void Write_U_b_YBottle_HOLD(BOOL v) {
	write_pin(Bottle_HOLD_PIN, v);
}

BOOL Read_U_b_YMotor_FORWARD(void) {
	return read_pin(Motor_FORWARD_PIN);
}

void Write_U_b_YMotor_FORWARD(BOOL v) {
	write_pin(Motor_FORWARD_PIN, v);
}

BOOL Read_U_b_YWater_ON(void) {
	return read_pin(Water_ON_PIN);
}

void Write_U_b_YWater_ON(BOOL v) {
	write_pin(Water_ON_PIN, v);
}

BOOL Read_U_b_YMotor_REVERSE(void) {
	return read_pin(Motor_REVERSE_PIN);
}

void Write_U_b_YMotor_REVERSE(BOOL v) {
	write_pin(Motor_REVERSE_PIN, v);
}

BOOL Read_U_b_YBottle_RELEASE(void) {
	return read_pin(Bottle_RELEASE_PIN);
}

void Write_U_b_YBottle_RELEASE(BOOL v) {
	write_pin(Bottle_RELEASE_PIN, v);
}



