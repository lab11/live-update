#include <sys/printk.h>
#include <kernel.h>

#include "tfm_gpio_veneers.h"
#include "ladder.h"

#define PIN 7

K_TIMER_DEFINE(scan_timer, plc_callback, NULL);

void main(void) {
	init_plc();

    // gpio veneer tests
    tfm_gpio_write_all(0xBEEF);
    printk("wrote %x to pins\n", 0xBEEF);
    u32_t data = tfm_gpio_read_all();
    printk("read all pins: %x\n", data);
    tfm_gpio_enable_outputs(0xFFFF);
    printk("enabled outputs on all pins\n");
    u8_t single_pin_data = tfm_gpio_read(2);
    printk("single PIN 2 value: %x\n", single_pin_data);

	k_timer_start(&scan_timer, K_MSEC(10), K_MSEC(10));
}
