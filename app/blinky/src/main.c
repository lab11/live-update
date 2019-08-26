#include "interface.h"

unsigned int blink_state = 0;
struct k_timer t;

void timer_expire_cb(struct k_timer *t) {
    blink_state ^= 1;
    printk("setting state to %d\n", blink_state);
    gpio_write(2, blink_state);
}

void timer_stop_cb(struct k_timer *t) {
    printk("timer stopped\n");
}

void main(void) {

    gpio_enable_output(2);

    k_timer_init(&t, timer_expire_cb, timer_stop_cb);
    k_timer_start(&t, 500, 500);
}

