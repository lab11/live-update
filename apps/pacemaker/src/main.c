/*
 * Pacemaker model implementation (asynchronous)
 */

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#include "pacemaker.h"

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

//struct k_timer aei_timer;
//struct k_timer diagnostic_timer;

// GPIO
struct device *gpio_dev;
struct gpio_callback ventricle_sense_pin_data;
struct gpio_callback atrial_sense_pin_data;

pacing_t current_pace = VENTRICLE;

// Reset timers that make up `t` in the model
void reset_t() {
    k_timer_start(&lri_timer, K_MSEC(TLRI-TAVI), K_MSEC(0));
    k_timer_start(&avi_timer, K_MSEC(TAVI), K_MSEC(0));
    k_timer_start(&vrp_timer, K_MSEC(TVRP), K_MSEC(0));
}

// GPIO inputs
void ventricle_sense_cb(struct device *dev, struct gpio_callback *cb, u32_t pin) {
    printk("Ve S\n");
    observe_ventricle_get();
}

void atrial_sense_cb(struct device *dev, struct gpio_callback *cb, u32_t pin) {
    printk("At S\n");
    observe_atrial_sense();
}

// Pacing outputs
void ventricle_pace() {
    printk("Ve P\n");

    current_pace = VENTRICLE;
    gpio_pin_set(gpio_dev, VENTRICLE_PACE_PIN, 1);
    k_timer_start(&pacing_timer, K_MSEC(1), K_MSEC(0));
    
    observe_ventricle_pace();
}

void atrial_pace() {
    printk("At P\n");

    current_pace = ATRIAL;
    gpio_pin_set(gpio_dev, ATRIAL_PACE_PIN, 1);
    k_timer_start(&pacing_timer, K_MSEC(1), K_MSEC(0));

    observe_atrial_pace();
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

/* Used by other threads to submit obervations 
   If being invoked for a Ventricle Pacing event, then calling thread MUST guarantee that VP_allowed = 1 prior to calling 
   Can also be invoked by GPIO inputs in which case VP_allowed should not be checked
*/
/*
void observe(EventType_t event) {
    if (event == VENTRICLE) {
        // The below function calls need to be chained together
        // printk("\n");
        // printk("Now observing Ventricle Event... \n");
        VP_allowed = 0;
        k_timer_stop(&lri_timer);
        k_timer_stop(&avi_timer);
        tfm_gpio_set(LED);
        tfm_gpio_clear(LED);
        notify_fsms(event);
    } else if (event == ATRIAL) {
        // printk("\n");
        // printk("Observing Atrial Event... \n");
        notify_fsms(event);
    }
}
*/

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

    gpio_init_callback(&ventricle_sense_pin_data, ventricle_sense_cb, BIT(VENTRICLE_SENSE_PIN));
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

    gpio_init_callback(&atrial_sense_pin_data, atrial_sense_cb, BIT(ATRIAL_SENSE_PIN));
    gpio_add_callback(gpio_dev, &atrial_sense_pin_data);

    // Initialize timers
    k_timer_init(&lri_timer, lri_timer_expire_cb, NULL);
    k_timer_init(&avi_timer, avi_timer_expire_cb, NULL);
    k_timer_init(&uri_clk, uri_clk_expire_cb, NULL);
    k_timer_init(&vrp_timer, vrp_timer_expire_cb, NULL);

    k_timer_init(&pacing_timer, stop_pace_cb, NULL);

    // Used for diagnostic purposes to measure latency
    // k_timer_init(&diagnostic_timer, NULL, NULL);

    // Uncomment to force ventricle event when no external inputs available
    printk("Forcing Ventricle Event\n");
    observe_ventricle_get();
}
