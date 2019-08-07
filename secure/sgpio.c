#define GPIO_BASE 0x50110000

void __attribute__((cmse_nonsecure_entry))
gpio_enable_output(int pin) {
    int *gpio_base = (int *)GPIO_BASE;
    *(gpio_base + 4) |= (0x1 << pin);
}

void __attribute__((cmse_nonsecure_entry))
gpio_write(int pin, int val) {
    int *gpio_base = (int *)GPIO_BASE;

    if (!val) {
        *(gpio_base + 1) |= 0x1 << pin; 
    } else {
        *(gpio_base + 1) &= ~(0x1 << pin);
    }
}

int __attribute__((cmse_nonsecure_entry))
gpio_read(int pin) {
    return 0;
}

void __attribute__((cmse_nonsecure_entry))
gpio_disable(int pin) {
    int *gpio_base = (int *)GPIO_BASE;
    *(gpio_base + 5) |= (0x1 << pin);
}
