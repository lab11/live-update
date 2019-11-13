#include "sys/printk.h"
#include "tfm_gpio_veneers.h"

void int10_cb(void) {
    printk("Got an interrupt on pin 10!\n");
}

void int11_cb(void) {
    printk("Got an interrupt on pin 11!\n");
}

void main(void) {
    gpio_int_config cfg = {
        .type = 1,
        .polarity = 0,
        .cb = NULL
    };

    cfg.cb = int10_cb;
    tfm_gpio_interrupt_enable(10, &cfg);

    cfg.polarity = 1;
    cfg.cb = int11_cb;
    tfm_gpio_interrupt_enable(11, &cfg);
}

