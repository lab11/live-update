/*
 * Simple Pacemaker (asynchronous)
 */

#include "interface.h"

#define VENTRICLE_SENSE_PIN 5
#define VENTRICLE_PACE_PIN  6
#define ATRIAL_SENSE_PIN    7
#define ATRIAL_PACE_PIN     8 // the shocker

#define TLRI_MS 1000
#define TAVI_MS 150

static struct k_timer t;
static struct k_timer shock_t;

void ventricle_sense_cb() {
    k_continue(timer_stop_cb);

    k_timer_stop(&t);
    gpio_add_oneshot(VENTRICLE_SENSE_PIN, ventricle_sense_cb);
}

void ventricle_pace_cb() {
    k_continue(timer_stop_cb);

    k_timer_stop(&t);
    gpio_add_oneshot(VENTRICLE_PACE_PIN, ventricle_pace_cb);
}

void atrial_pace_cb() {
    k_continue(ventrical_sense_cb);
    k_continue(ventrical_pace_cb);
}

void timer_stop_cb(struct k_timer *t) {
    // restart the timer!
    k_timer_start(t, TLRI_MS - TAVI_MS, 0); // restart timer to expire in TLRI_MS - TAVI_MS
}

void timer_expire_cb(struct k_timer *t) {
    k_continue(shock_expire_cb);

    // Shock the heart!
    gpio_write(ATRIAL_PACE_PIN, 0x1); // sets pin=on
    k_timer_start(&shock_t, 1, 0);
    
    gpio_write(ATRIAL_PACE_PIN, 0x0); // sets pin=off
}

void shock_expire_cb(struct k_timer *t) {
    k_continue(timer_stop_cb); // calls timer stop callback to restart timer

    gpio_write(ATRIAL_PACE_PIN, 0x0); // sets pin=off
    k_timer_stop(&t); // stops main timer
}

void shock_stop_cb(struct k_timer *t) {}

void main(void) {

    // Initialization
    k_timer_init(&t, timer_expire_cb, timer_stop_cb);
    k_timer_init(&shock_t, shock_expire_cb, timer_stop_cb);

    gpio_enable_output(ATRIAL_PACE_PIN);

    // Set up pending tasks
    gpio_add_oneshot(VENTRICLE_SENSE_PIN, ventricle_sense_cb);
    gpio_add_oneshot(VENTRICLE_PACE_PIN, ventricle_pace_cb);
    gpio_add_oneshot(ATRIAL_SENSE_PIN, atrial_pace_cb);
    k_timer_start(&t, TLRI_MS - TAVI_MS, 0); // force one-shot; after TLRI_MS - TAVI_MS, then timer_expire_cb will be called
}