/*
 * Simple Pacemaker (asynchronous)
 */

#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include "kernel.h"
// #include "interface.h"
#include "pacemaker.h"

// Params
#define TAVI    150
#define TURI    400

extern struct k_timer avi_timer;
extern struct k_timer uri_timer;
extern struct k_timer vrp_timer;

bool avi_expired = 0;
bool uri_expired = 0;

extern bool VP_allowed;
extern bool VSed;

// When AVI timer expires, then as long as URI timer has expired and ventrical event has not been sensed, 
// then ventrical pacing should occur.
void avi_timer_expire_cb(struct k_timer *t) {
    printk("avi timer expired \n");
    avi_expired = 1;

    if (VSed == 1) {
        return;
    } else {
        if (uri_expired == 1 && VP_allowed == 1) {
            // Notify parent thread that VP should be triggered
            tfm_gpio_set(VENTRICLE_PACE_PIN);
            observe(VENTRICLE);
        } else if (uri_expired == 0 && VP_allowed == 0) {
            // TODO: sleep?   
        }
    }
    
}

// When URI timer expires, then ventrical pacing events should be reenabled.
void uri_timer_expire_cb(struct k_timer *t) {
    // k_continue(shock_expire_cb);
    printk("uri timer expired \n");
    uri_expired = 1;

    if (k_timer_status_get(&vrp_timer) == 0) {
        VP_allowed = 1;
    }

    //TODO: need to figure out what to do if vrp_timer has not yet expired

    // if (avi_expired == 1) {
    //     // Notify parent thread that VP should be triggered
    //     printk("avi expired...observing ventricle event \n");
    //     observe(VENTRICLE);
    // } 
    // Not else
    return;
    
}

void avi_observe() {
    printk("avi_observe \n");
    k_timer_start(&avi_timer, TAVI, 0);
}

/* Watches for ventricle events*/
void uri_observe() {
    printk("uri_observe \n");
    k_timer_start(&uri_timer, TURI, 0);
}
