#include "sys/printk.h"
#include "kernel.h"

#include "ladder.h"

#define PIN 7

K_TIMER_DEFINE(scan_timer, plc_callback, NULL);

void main(void) {
	init_plc();
	k_timer_start(&scan_timer, K_MSEC(10), K_MSEC(10));
}