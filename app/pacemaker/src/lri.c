/*
 * Simple Pacemaker (asynchronous)
 */

#include "interface.h"
#include "stdbool.h"
#include <stddef.h>
#include "pacemaker.h"

// Params
#define TAVI    150
#define TLRI    1000

extern struct k_timer lri_timer;
extern struct k_timer aei_timer;

/* Was an Atrial event sensed during AEI? */
bool ASed = 0;

/* Watches for ventricle events*/
void lri_observe(char* event) {
    printk("lri_observe");
    if(strcmp(event, "ventricle") == 0) {
        k_timer_stop(&lri_timer); // This will also restart the timer with expiration time = TLRI
    }
}

/* Watches for events*/
void aei_observe(char* event) {
    printk("aei_observe");
    if(strcmp(event, "ventricle") == 0) {
        ASed = 0;
        k_timer_stop(&aei_timer);
    } else if(strcmp(event, "atrial") == 0) {
        ASed = 1;
    }
}

/* Timer stop callbacks*/
void lri_timer_stop_cb(struct k_timer *t) {
    // restart the timer!
    k_timer_start(t, TLRI, 0); // restart timer to expire in TLRI
}

void aei_timer_stop_cb(struct k_timer *t) {
    // restart the timer!
    k_timer_start(t, TLRI-TAVI, 0); // restart timer to expire in TLRI
}

/* Timer expire callbacks */
void aei_timer_expire_cb(struct k_timer *t) {
    /* Triggers AP if no AS sensed */
    printk("aei timer expired");
    if (ASed == 0) {
        // Notify parent thread that VP should be triggered
        observe("atrial");
    }
}

/* Thread entry point */
// void lri_entry() {
    // Initialization
    // k_timer_init(&lri_timer, NULL, lri_timer_stop_cb);
    // k_timer_init(&aei_timer, aei_timer_expire_cb, aei_timer_stop_cb);

    // ASed = 0;
// }