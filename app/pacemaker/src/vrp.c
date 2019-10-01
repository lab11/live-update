/*
 * Simple Pacemaker (asynchronous)
 */

#include "interface.h"
#include "stdbool.h"
#include <stddef.h>
#include "pacemaker.h"

// Params
#define TVRP    150

extern bool VP_allowed;

// VRP timer
extern struct k_timer vrp_timer;

/* Watches for ventricle events*/
void vrp_observe(char* event) {
    VP_allowed = 0;
    if(strcmp(event, "ventricle") == 0) {
       k_timer_start(&vrp_timer, TVRP, 0);
    }
}

// /* Timer expire callbacks */
void vrp_timer_expire_cb(struct k_timer *t) {
    // Allow VP events to occur globally
    printk("vrp timer expired");
    VP_allowed = 1;
}

/* Thread entry point */
// void vrp_entry() {
    // Initialization
    // k_timer_init(&vrp_timer, NULL, NULL);

    // VP_allowed = 0;
// }