#include "sys/printk.h"
#include "kernel.h"

#include "tfm_gpio_veneers.h"

#define PIN 3

bool toggled = false;
struct k_timer gpio_timer;

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

void main(void) {
    tfm_gpio_enable_output(PIN);

    k_timer_init(&gpio_timer, gpio_timer_handler, NULL);
    k_timer_start(&gpio_timer, K_SECONDS(1), K_SECONDS(1));
}

