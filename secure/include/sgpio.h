#ifndef __SGPIO_H__
#define __SGPIO_H__

void gpio_enable_output(int pin);

void gpio_write(int pin, int val);

void gpio_disable(int pin);

#endif // SGPIO
