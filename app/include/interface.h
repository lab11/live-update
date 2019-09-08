
extern void (*printk)(const char *fmt, ...);
extern void (*k_sleep)(int ms);

extern void (*gpio_enable_output)(int pin);
extern void (*gpio_write)(int pin, int val);
extern void (*gpio_disable)(int pin);

struct k_timer {
    char contents[44];
};

typedef void (*k_timer_expiry_t)(struct k_timer *t);
typedef void (*k_timer_stop_t)(struct k_timer *t);
extern void (*k_timer_init)(struct k_timer *t, k_timer_expiry_t exp_fn, k_timer_stop_t stop_fn);
extern void (*k_timer_start)(struct k_timer *t, int duration, int period);
extern void (*k_timer_stop)(struct k_timer *t);

extern K_THREAD_DEFINE(name, stack_size, entry, p1, p2, p3, prio, options, delay);
