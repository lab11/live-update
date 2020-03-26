#include "sys/printk.h"
#include "tfm_gpio_veneers.h"

void pin10_cb(void) {
    printk("Got an interrupt on pin 10!\n");
}

void pin11_cb(void) {
    printk("Got an interrupt on pin 11!\n");
}

void main(void) {
    gpio_int_config pin10_cfg = {
        .type = 1,
        .polarity = 0,
        .cb = pin10_cb
    };
    tfm_gpio_interrupt_enable(10, &pin10_cfg);

    gpio_int_config pin11_cfg = {
        .type = 1,
        .polarity = 1,
        .cb = pin11_cb
    };
    tfm_gpio_interrupt_enable(11, &pin11_cfg);
}

