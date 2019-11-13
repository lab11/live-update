#include <stdint.h>

// KERNEL
extern void (*printk)(const char *fmt, ...);
extern void (*k_sleep)(int ms);

// GPIO
extern void (*gpio_enable_output)(int pin);
extern void (*gpio_write)(int pin, int val);
extern void (*gpio_disable)(int pin);

// TIMER
struct k_timer {
    char contents[44];
} __attribute__ ((aligned (4)));

typedef void (*k_timer_expiry_t)(struct k_timer *t);
typedef void (*k_timer_stop_t)(struct k_timer *t);

extern void (*k_timer_init)(struct k_timer *t, k_timer_expiry_t exp_fn, k_timer_stop_t stop_fn);
extern void (*k_timer_start)(struct k_timer *t, int duration, int period);
extern void (*k_timer_stop)(struct k_timer *t);
extern uint32_t (*k_timer_status_get)(struct k_timer *t);

