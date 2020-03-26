#include "sys/printk.h"
#include "kernel.h"

#include "tfm_gpio_veneers.h"

#define PIN 7

bool toggled = false;
void gpio_timer_handler(struct k_timer *t) {
    if (toggled) {
        printk("setting pin %d...\n", PIN);
        tfm_gpio_set(PIN);
    } else {
        printk("clearing pin %d...\n", PIN);
        tfm_gpio_clear(PIN);
    }
    toggled ^= 1;
}

K_TIMER_DEFINE(gpio_timer, gpio_timer_handler, NULL);

void main(void) {
    tfm_gpio_enable_output(PIN);
    k_timer_start(&gpio_timer, K_SECONDS(1), K_SECONDS(1));
}

