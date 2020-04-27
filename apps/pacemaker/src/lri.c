/*
 * Simple Pacemaker (asynchronous)
 */

// #include "interface.h"
#include "pacemaker.h"
#include "kernel.h"
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

// Params
#define TAVI    150
#define TLRI    1000

extern struct k_timer lri_timer;
extern struct k_timer aei_timer;

/* Was an Atrial event sensed during AEI? */
extern bool ASed;

/* Watches for ventricle events*/
void lri_observe() {
    // printk("lri_observe \n");
    k_timer_start(&lri_timer, TLRI, 0);
}

/* Watches for events*/
void aei_observe() {
    // printk("aei_observe \n");
    ASed = 0;
    k_timer_start(&aei_timer, TLRI-TAVI, 0);
}

/* Timer stop callbacks*/
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
    /* Triggers AP if no AS sensed */
    // printk("aei timer expired \n");
    if (ASed == 0) {
        // Notify parent thread that VP should be triggered
        // tfm_gpio_set(ATRIAL_PACE_PIN);
        // observe(ATRIAL);
        atrial_pace();
    }
}