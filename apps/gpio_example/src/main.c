//docs.zephyrproject.org/latest/reference/peripherals/gpio.html

#include "kernel.h"
#include "drivers/gpio.h"
#include "sys/printk.h"

#define PIN 4

bool toggled = false;

struct device *gpio_dev;
struct k_timer gpio_timer;

void gpio_timer_handler(struct k_timer *t) {
    if (toggled) {
        printk("setting pin %d to %d...\n", PIN, 0);
	    gpio_pin_set(gpio_dev, PIN, 0);
    } else {
        printk("setting pin %d to %d...\n", PIN, 1);
	    gpio_pin_set(gpio_dev, PIN, 1);
    }
    toggled ^= 1;
}

void main(void) {
    gpio_dev = device_get_binding("GPIO_0");
    int ret = gpio_pin_configure(gpio_dev, PIN, GPIO_OUTPUT_INACTIVE);
    if (ret) {
	    printk("gpio_pin_configure failed with error code: %d\n", ret);
	    return;
    }
    
    k_timer_init(&gpio_timer, gpio_timer_handler, NULL);
    k_timer_start(&gpio_timer, K_SECONDS(1), K_SECONDS(1));
}

