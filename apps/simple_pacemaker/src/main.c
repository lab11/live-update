/*
 * Simple Pacemaker (asynchronous)
 */

#include "stdbool.h"
#include <stdarg.h>
#include <stddef.h>
#include <string.h>
#include "kernel.h"
#include "tfm_gpio_veneers.h"

// Params
#define TAVI    150
#define TLRI    1000
#define TPVARP  100
#define TVRP    150
#define TURI    400
#define TPVAB   50

typedef enum {
    VENTRICLE=0, 
    ATRIAL=1
} EventType_t;

bool ASed;

// Timers
struct k_timer avi_timer;
struct k_timer aei_timer;

void avi_observe() {
    k_timer_start(&avi_timer, TAVI, 0);
}

void aei_observe() {
    ASed = false;
    k_timer_start(&aei_timer, TLRI-TAVI, 0);
}

void notify_fsms(EventType_t event) {
    if (event == VENTRICLE) {
        aei_observe();
    } else if (event == ATRIAL) {
        avi_observe();
    }
}

void observe(EventType_t event) {
    if (event == VENTRICLE) {
        printk("Observe VENTRICLE\n");
        k_timer_stop(&avi_timer);
        notify_fsms(event);
    } else if (event == ATRIAL) {
        printk("Observe ATRIAL\n");
        notify_fsms(event);
    }
}

void avi_timer_expire_cb(struct k_timer *t) {
    observe(VENTRICLE);
}

void atrial_sense_cb(void) {
    ASed = true;
    observe(ATRIAL);
}

void aei_timer_expire_cb(struct k_timer *t) {
    if (!ASed) {
        observe(ATRIAL);
    }
}

void main(void) {

    k_timer_init(&aei_timer, aei_timer_expire_cb, NULL);
    k_timer_init(&avi_timer, avi_timer_expire_cb, NULL);

    observe(VENTRICLE);
}
