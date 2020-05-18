/*
 * Simple Pacemaker (asynchronous)
 */

#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include "kernel.h"
#include "pacemaker.h"

// Params
#define TAVI    150
#define TURI    400

extern struct k_timer avi_timer;
extern struct k_timer uri_timer;
extern struct k_timer vrp_timer;

bool avi_expired = 0;
bool uri_expired = 1;
bool avi_extended = 0;

extern bool VP_allowed;
extern bool VSed;

// When AVI timer expires, then as long as URI timer has expired and ventrical event has not been sensed, 
// then ventrical pacing should occur.
void avi_timer_expire_cb(struct k_timer *t) {
    avi_expired = 1;
    // printk("AVI Expired \n");
    // printk("VSed: %d \n", VSed);
    // printk("uri exp? %d vp all? %d \n", uri_expired, VP_allowed);
    if (VSed == 1) {
        return;
    } else {
        if (uri_expired == 1 && VP_allowed == 1) {
            // printk("V Pacing.. \n");
            ventricle_pace();
        } else if (uri_expired == 0 && VP_allowed == 0) {
            // printk("Extending AVI \n");
            avi_extended = 1;
        }
    }
    
}

// When URI timer expires, then ventrical pacing events should be reenabled.
void uri_timer_expire_cb(struct k_timer *t) {
    uri_expired = 1;

    if (k_timer_status_get(&vrp_timer) == 0) {
        VP_allowed = 1;
    }

    if (avi_extended == 1) {
        // printk("VP in URI");
        ventricle_pace();
        avi_extended = 0;
    }

    return;
    
}

void avi_observe() {
    k_timer_start(&avi_timer, TAVI, 0);
}

/* Watches for ventricle events*/
void uri_observe() {
    uri_expired = 0;
    k_timer_start(&uri_timer, TURI, 0);
}
