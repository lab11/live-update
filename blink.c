#include <stdio.h>
#include "cmsis_os2.h"
#include "tfm_gpio_veneers.h"

#define PIN 4

static uint8_t on = 0;

void blink_init() {
    tfm_gpio_enable_output(PIN); 
}

int main() {
    blink_init();

    for (;;) {
        osDelay(500); // only blocking call (as if we were waiting for loop body 'callback')

        if (on) tfm_gpio_clear(PIN);
        else tfm_gpio_set(PIN);

        on = ~on;
    }
}
