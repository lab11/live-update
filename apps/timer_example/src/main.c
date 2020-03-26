#include "kernel.h"
#include "sys/printk.h"

uint8_t trigger_count = 0;

void timer_handler(struct k_timer *t) {
    trigger_count++;
    printk("Timer trigger number %d\n", trigger_count);
}

struct k_timer t;

void main(void) {
    k_timer_init(&t, timer_handler, NULL);
    k_timer_start(&t, K_SECONDS(1), K_SECONDS(1));
}

