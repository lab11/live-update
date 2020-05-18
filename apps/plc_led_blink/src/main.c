#include <sys/printk.h>
#include <kernel.h>

#include "tfm_gpio_veneers.h"
#include "ladder.h"

#define PIN 7

#define PLC_SCAN_TIME 10

struct k_timer scan_timer;

void main(void) {
	init_plc();
    k_timer_init(&scan_timer, plc_callback, NULL);

    // gpio veneer tests
    // tfm_gpio_write_all(0xBEEF);
    // printk("wrote %x to pins\n", 0xBEEF);
    // u32_t data = tfm_gpio_read_all();
    // printk("read all pins: %x\n", data);
    // tfm_gpio_enable_outputs(0xFFFF);
    // printk("enabled outputs on all pins\n");
    // u8_t single_pin_data = tfm_gpio_read(2);
    // printk("single PIN 2 value: %x\n", single_pin_data);

	k_timer_start(&scan_timer, K_MSEC(PLC_SCAN_TIME), K_MSEC(PLC_SCAN_TIME));
}
