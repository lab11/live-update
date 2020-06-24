/*
 * Pacemaker model implementation (asynchronous)
 */

#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#include "pacemaker.h"

extern struct k_timer lri_timer;

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

/*

// Watches for events
void aei_observe() {
    ASed = 0;
    k_timer_start(&aei_timer, TLRI-TAVI, 0);
}

// Timer stop callbacks
void lri_timer_stop_cb(struct k_timer *t) {
    // restart the timer!
    // printk("lri timer stopped \n");
    k_timer_start(t, TLRI, 0); // restart timer to expire in TLRI
}

void aei_timer_stop_cb(struct k_timer *t) {
    // restart the timer!
    // printk("aei timer stopped \n");
    k_timer_start(t, TLRI-TAVI, 0); // restart timer to expire in TLRI
}

// If AEI timer expires and no atrial event has been sensed, then trigger atrial pacing
void aei_timer_expire_cb(struct k_timer *t) {
    // Triggers AP if no AS sense
    // printk("aei timer expired \n");
    if (ASed == 0) {
        // Notify parent thread that VP should be triggered
        // tfm_gpio_set(ATRIAL_PACE_PIN);
        // observe(ATRIAL);
        atrial_pace();
    }
}

*/
