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
#define TVRP    150

extern bool VP_allowed;

// VRP timer
extern struct k_timer vrp_timer;

/* Watches for ventricle events*/
void vrp_observe() {
    printk("vrp_observe \n");
    VP_allowed = 0;
    k_timer_start(&vrp_timer, TVRP, 0);
}

// /* Timer expire callbacks */
void vrp_timer_expire_cb(struct k_timer *t) {
    // Allow VP events to occur globally
    printk("vrp timer expired \n");
    VP_allowed = 1;
}
