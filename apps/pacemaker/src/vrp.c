/*
 * Pacemaker model implementation (asynchronous)
 */

#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#include "pacemaker.h"

extern struct k_timer vrp_timer;

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

/*
// Watches for ventricle events
void vrp_observe() {
    // printk("vrp_observe \n");
    VP_allowed = 0;
    k_timer_start(&vrp_timer, TVRP, 0);
}

// // Timer expire callbacks
void vrp_timer_expire_cb(struct k_timer *t) {
    // Allow VP events to occur globally
    // printk("vrp timer expired \n");
    VP_allowed = 1;
}
*/
