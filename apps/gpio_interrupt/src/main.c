#include "sys/printk.h"
#include "tfm_gpio_veneers.h"

#define INTPIN 10

void test_cb(void) {
    printk("Got an interrupt!\n");
}

void main(void) {
    tfm_gpio_interrupt_enable(INTPIN, test_cb);
}

