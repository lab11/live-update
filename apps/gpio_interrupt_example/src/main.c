// https://docs.zephyrproject.org/latest/reference/peripherals/gpio.html

#include "kernel.h"
#include "drivers/gpio.h"
#include "sys/printk.h"

#define PIN1 10
#define PIN2 11

struct device *gpio_dev;
struct gpio_callback cb1_data;
struct gpio_callback cb2_data;

void pin1_cb(struct device *dev, struct gpio_callback *cb, u32_t pin) {
    printk("Got an interrupt on pin %d!\n", pin);
}

void pin2_cb(struct device *dev, struct gpio_callback *cb, u32_t pin) {
    printk("Got another interrupt on pin %d!\n", pin);
}

void main(void) {

    gpio_dev = device_get_binding("GPIO_0");

    int ret = gpio_pin_configure(gpio_dev, PIN1, GPIO_INPUT);
    if (ret) {
        printk("gpio_pin_configure(%d) failed with error code: %d\n", PIN1, ret);
        return;
    }

    ret = gpio_pin_configure(gpio_dev, PIN2, GPIO_INPUT);
    if (ret) {
        printk("gpio_pin_configure(%d) failed with error code: %d\n", PIN2, ret);
        return;
    }

    ret = gpio_pin_interrupt_configure(gpio_dev, PIN1, GPIO_INT_EDGE_TO_ACTIVE);
    if (ret) {
        printk("gpio_pin_interrupt_configure(%d) failed with error code: %d\n", PIN1, ret);
        return;
    }

    ret = gpio_pin_interrupt_configure(gpio_dev, PIN2, GPIO_INT_EDGE_TO_INACTIVE);
    if (ret) {
        printk("gpio_pin_configure(%d) failed with error code: %d\n", PIN2, ret);
        return;
    }

    gpio_init_callback(&cb1_data, pin1_cb, BIT(PIN1));
    gpio_add_callback(gpio_dev, &cb1_data);

    gpio_init_callback(&cb2_data, pin2_cb, BIT(PIN2));
    gpio_add_callback(gpio_dev, &cb2_data);
}

