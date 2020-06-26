#include <sys/printk.h>
#include <kernel.h>

#include "tfm_gpio_veneers.h"
#include "ladder.h"

#define PLC_SCAN_TIME 10

struct k_timer scan_timer;

void main(void) {
	init_plc();
    k_timer_init(&scan_timer, plc_callback, NULL);

	k_timer_start(&scan_timer, K_MSEC(PLC_SCAN_TIME), K_MSEC(PLC_SCAN_TIME));
}
