/*
 * Pacemaker model implementation (asynchronous)
 */

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#include "kernel.h"
#include "drivers/gpio.h"

#define TAVI    150
#define TLRI    1000
#define TPVARP  100
#define TVRP    150
#define TURI    400
#define TPVAB   50

// Pins
#define LED                 2
#define VENTRICLE_SENSE_PIN 10 //Vget! action
#define ATRIAL_SENSE_PIN    11 //Aget! action
#define VENTRICLE_PACE_PIN  12 //VP!
#define ATRIAL_PACE_PIN     13 //AP!; the shocker

typedef enum {
    VENTRICLE=0, 
    ATRIAL=1
} pacing_t;

extern void gpio_nrfx_init_callback(struct gpio_callback *, gpio_callback_handler_t, gpio_port_pins_t);

void reset_t();
void observe_ventricle_sense();
void ventricle_pace();
void atrial_pace();

// LRI
void lri_ventricle_sense();
void lri_ventricle_pace();
void lri_atrial_sense();
void lri_timer_expire_cb(struct k_timer *t);

// AVI
void avi_ventricle_sense();
void avi_atrial_sense();
void avi_atrial_pace();
void avi_timer_expire_cb(struct k_timer *t);

// URI
void uri_ventricle_sense();
void uri_ventricle_pace();
void uri_clk_expire_cb(struct k_timer *t);

// VRP
void vrp_ventricle_get();
void vrp_ventricle_pace();
void vrp_timer_expire_cb(struct k_timer *t);

// Prototypes
void observe_ventricle_get();
void observe_ventricle_pace();
void observe_atrial_sense();
void observe_atrial_pace();

// Timers
struct k_timer lri_timer;
struct k_timer avi_timer;
struct k_timer uri_clk;
struct k_timer vrp_timer;
struct k_timer pacing_timer;

// GPIO
struct device *gpio_dev;
struct gpio_callback ventricle_sense_pin_data;
struct gpio_callback atrial_sense_pin_data;

// -- AVI Component --
static bool uri_expired = false;

enum avi_state_t {Idle, AVI, WaitURI};
static enum avi_state_t s = Idle;

void avi_ventricle_sense() {
    if (s == AVI || s == WaitURI) {
        s = Idle;
    }
}

void avi_atrial_sense() {
    if (s == Idle) {
        reset_t();
        s = AVI;
    }
}

void avi_atrial_pace() {
    if (s == Idle) {
        // NOTE: assumes reset_t is already called by lri_timer_expire_cb -> atrial_pace()
        // reset_t();
        s = AVI;
    }
}

void avi_timer_expire_cb(struct k_timer *t) {
    if (s != AVI) return;
    
    if (uri_expired) {
        ventricle_pace();
        s = Idle;
    } else {
        s = WaitURI;
    }
}

void avi_clk_expire() {
    if (s == WaitURI) {
        ventricle_pace();
        s = Idle;
    }
}

// -- URI Component --
void uri_ventricle();

void uri_ventricle_sense() {
    uri_ventricle();
}

void uri_ventricle_pace() {
    uri_ventricle();
}

void uri_ventricle() {
    uri_expired = false;
    k_timer_start(&uri_clk, K_MSEC(TURI), K_MSEC(0));
}

void uri_clk_expire_cb(struct k_timer *t) {
    uri_expired = true;
    avi_clk_expire();
}

// -- LRI Component --
static bool ASed = false;

void lri_ventricle();

void lri_ventricle_sense() {
    // NOTE: assumes reset_t is already called by vrp_ventricle_get()
    // reset_t();
    lri_ventricle();
}

void lri_ventricle_pace() {
    reset_t();
    lri_ventricle();
}

void lri_ventricle() {
    ASed = false;
}

void lri_atrial_sense() {
    ASed = true;
}

void lri_timer_expire_cb(struct k_timer *t) {
    reset_t();
    atrial_pace();
}

// -- VRP Component --

static bool vrp = false;

void vrp_ventricle_get() {
    if (!vrp) {
        vrp = true;
        reset_t();
        observe_ventricle_sense();
    }
}

void vrp_ventricle_pace() {
    if (!vrp) {
        // NOTE: assumes reset_t is already called by lri_ventricle_pace()
        // reset_t();
        vrp = true;
    }
}

void vrp_timer_expire_cb(struct k_timer *t) {
    vrp = false;
}

// -- Main Component --
pacing_t current_pace = VENTRICLE;

// Reset timers that make up `t` in the model
void reset_t() {
    k_timer_start(&lri_timer, K_MSEC(TLRI-TAVI), K_MSEC(0));
    k_timer_start(&avi_timer, K_MSEC(TAVI), K_MSEC(0));
    k_timer_start(&vrp_timer, K_MSEC(TVRP), K_MSEC(0));
}

// GPIO inputs
void ventricle_sense_cb(struct device *dev, struct gpio_callback *cb, u32_t pin) {
    observe_ventricle_get();
    printk("UVe S\n");
}

void atrial_sense_cb(struct device *dev, struct gpio_callback *cb, u32_t pin) {
    observe_atrial_sense();
    printk("UAt S\n");
}

// Pacing outputs
void ventricle_pace() {

    current_pace = VENTRICLE;
    gpio_pin_set(gpio_dev, VENTRICLE_PACE_PIN, 1);
    k_timer_start(&pacing_timer, K_MSEC(1), K_MSEC(0));
    
    observe_ventricle_pace();
    printk("UVe P\n");
}

void atrial_pace() {

    current_pace = ATRIAL;
    gpio_pin_set(gpio_dev, ATRIAL_PACE_PIN, 1);
    k_timer_start(&pacing_timer, K_MSEC(1), K_MSEC(0));

    observe_atrial_pace();
    printk("UAt P\n");
}

void stop_pace_cb(struct k_timer *t) {
    gpio_pin_t pin = current_pace == VENTRICLE ? VENTRICLE_PACE_PIN : ATRIAL_PACE_PIN;
    gpio_pin_set(gpio_dev, pin, 0);
}

// Observation functions; route to different components
void observe_ventricle_get() {
    vrp_ventricle_get();
}

void observe_ventricle_sense() {
    lri_ventricle_sense();    
    avi_ventricle_sense();    
    uri_ventricle_sense();
}

void observe_ventricle_pace() {
    lri_ventricle_pace();
    uri_ventricle_pace();
    vrp_ventricle_pace();
}

void observe_atrial_sense() {
    lri_atrial_sense();
    avi_atrial_sense();
}

void observe_atrial_pace() {
    avi_atrial_pace();
}

void main(void) {
    gpio_dev = device_get_binding("GPIO_0");

    // Configure output pins
    int ret = gpio_pin_configure(gpio_dev, LED, GPIO_OUTPUT_INACTIVE);
    if (ret) {
        printk("gpio_pin_configure failed on pin %d with code: %d\n", LED, ret);
        return;
    }

    ret = gpio_pin_configure(gpio_dev, VENTRICLE_PACE_PIN, GPIO_OUTPUT_INACTIVE);
    if (ret) {
        printk("gpio_pin_configure failed on pin %d with code: %d\n", VENTRICLE_PACE_PIN, ret);
        return;
    }

    ret = gpio_pin_configure(gpio_dev, ATRIAL_PACE_PIN, GPIO_OUTPUT_INACTIVE);
    if (ret) {
        printk("gpio_pin_configure failed on pin %d with code: %d\n", ATRIAL_PACE_PIN, ret);
        return;
    }

    // Configure sense interrupts
    ret = gpio_pin_configure(gpio_dev, VENTRICLE_SENSE_PIN, GPIO_INPUT); 
    if (ret) {
        printk("gpio_pin_configure failed on pin %d with code: %d\n", VENTRICLE_SENSE_PIN, ret);
        return;
    }

    ret = gpio_pin_interrupt_configure(gpio_dev, VENTRICLE_SENSE_PIN, GPIO_INT_EDGE_TO_ACTIVE); 
    if (ret) {
        printk("gpio_pin_interrupt_configure failed on pin %d with code: %d\n", VENTRICLE_SENSE_PIN, ret);
        return;
    }

    gpio_nrfx_init_callback(&ventricle_sense_pin_data, ventricle_sense_cb, BIT(VENTRICLE_SENSE_PIN));
    gpio_add_callback(gpio_dev, &ventricle_sense_pin_data);

    ret = gpio_pin_configure(gpio_dev, ATRIAL_SENSE_PIN, GPIO_INPUT); 
    if (ret) {
        printk("gpio_pin_configure failed on pin %d with code: %d\n", ATRIAL_SENSE_PIN, ret);
        return;
    }

    ret = gpio_pin_interrupt_configure(gpio_dev, ATRIAL_SENSE_PIN, GPIO_INT_EDGE_TO_ACTIVE); 
    if (ret) {
        printk("gpio_pin_interrupt_configure failed on pin %d with code: %d\n", ATRIAL_SENSE_PIN, ret);
        return;
    }

    gpio_nrfx_init_callback(&atrial_sense_pin_data, atrial_sense_cb, BIT(ATRIAL_SENSE_PIN));
    gpio_add_callback(gpio_dev, &atrial_sense_pin_data);

    // Initialize timers
    k_timer_init(&lri_timer, lri_timer_expire_cb, NULL);
    k_timer_init(&avi_timer, avi_timer_expire_cb, NULL);
    k_timer_init(&uri_clk, uri_clk_expire_cb, NULL);
    k_timer_init(&vrp_timer, vrp_timer_expire_cb, NULL);

    k_timer_init(&pacing_timer, stop_pace_cb, NULL);

    // Uncomment to force ventricle event when no external inputs available
    //printk("Forcing Ventricle Event\n");
    //observe_ventricle_get();
}
