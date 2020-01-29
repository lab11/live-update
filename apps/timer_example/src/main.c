#include "kernel.h"
#include "sys/printk.h"

uint8_t trigger_count = 0;

void timer_handler(struct k_timer *t) {
    trigger_count++;
    printk("Timer trigger number %d\n", trigger_count);
}

K_TIMER_DEFINE(t, timer_handler, NULL);

void main(void) {
    k_timer_start(&t, K_SECONDS(1), K_SECONDS(1));
}

