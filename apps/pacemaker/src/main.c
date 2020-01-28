#include "interface.h"
#include "stdbool.h"
#include <stdarg.h>
#include <stddef.h>
#include <string.h>
#include "pacemaker.h"

// Pins
#define LED                 2
#define VENTRICLE_SENSE_PIN 5 //Vget! action
#define VENTRICLE_PACE_PIN  6 //VP!
#define ATRIAL_SENSE_PIN    7 //Aget! action
#define ATRIAL_PACE_PIN     8 //AP!; the shocker

// Params
#define TAVI    150
#define TLRI    1000
#define TPVARP  100
#define TVRP    150
#define TURI    400
#define TPVAB   50

// Thread stuff
// #define STACKSIZE 1024
// #define PRIORITY 7

// static enum pacestate state = LRI;
// bool VSed;
// bool ASed;

bool VP_allowed;

// Timers
struct k_timer avi_timer;
struct k_timer uri_timer;
struct k_timer vrp_timer;
struct k_timer lri_timer;
struct k_timer aei_timer;

char * prev_event;

/* Will notify all FSM threads that atrial event or ventricular event has been sensed; takes var args*/
// void notify_fsms(int arg_count, ...) {
//     va_list valist;
//     va_start(valist, arg_count);
//     int i;
//     printk("notifying fsms...");
//     for (i = 0; i < arg_count; i++) {
//       void (*fun_ptr)() = va_arg(valist, int); // Not sure about this syntax
//       (*fun_ptr)(prev_event); // This should call the observe function in the respective thread
//    }

//    va_end(valist);
// }

void notify_fsms_ventricle() {
    printk("notifying fsms...");
    uri_observe("ventricle");
    vrp_observe("ventricle");
    aei_observe("ventricle");
    lri_observe("ventricle");
}

void notify_fsms_atrial() {
    printk("notifying fsms...");
    avi_observe("atrial");
}

/* Used by other threads to submit obervations 
   If being invoked for a Ventricle Pacing event, then calling thread MUST guarantee that VP_allowed = 1 prior to calling 
   Can also be invoked by GPIO inputs in which case VP_allowed should not be checked
*/
void observe(char* event) {
    if (!strcmp(event, "ventricle")) {
        // The below function calls need to be chained together
        printk("Observing Ventricle Event...");
        prev_event = "ventricle";
        gpio_write(VENTRICLE_PACE_PIN, 0x1); // sets pin=on
        // notify_fsms(4, &uri_observe, &vrp_observe, &aei_observe, &lri_observe);
        notify_fsms_ventricle();
        gpio_write(VENTRICLE_PACE_PIN, 0x0); // sets pin=on
    } else if (!strcmp(event, "atrial")) {
        printk("Observing Atrial Event...");
        prev_event="atrial";
        gpio_write(ATRIAL_PACE_PIN, 0x1); // sets pin=on
        // notify_fsms(1, &avi_observe);
        notify_fsms_atrial();
        gpio_write(ATRIAL_PACE_PIN, 0x0); // sets pin=on
    }

}

void main(void) {
    gpio_enable_output(ATRIAL_PACE_PIN);
    gpio_enable_output(VENTRICLE_PACE_PIN);
    gpio_enable_output(LED);

    /* Add gpio callbacks ; TODO: needs to be fixed by Jean-Luc */
    // gpio_add_oneshot(VENTRICLE_SENSE_PIN, ventricle_sense_cb);
    // gpio_add_oneshot(VENTRICLE_PACE_PIN, ventricle_pace_cb);
    // gpio_add_oneshot(ATRIAL_SENSE_PIN, atrial_pace_cb);

    /* Three threads corresponding to three groups of timing components will be spawned */
    // K_THREAD_DEFINE(lri_id, STACKSIZE, lri_entry, NULL, NULL, NULL,PRIORITY, 0, K_NO_WAIT); // need to include in interface.h maybe?
    // K_THREAD_DEFINE(avi_id, STACKSIZE, avi_entry, NULL, NULL, NULL,PRIORITY, 0, K_NO_WAIT);
    // K_THREAD_DEFINE(vrp_id, STACKSIZE, vrp_entry, NULL, NULL, NULL,PRIORITY, 0, K_NO_WAIT);

    // Force ventrical pace event

    // init lri and aei
    k_timer_init(&lri_timer, NULL, lri_timer_stop_cb);
    k_timer_init(&aei_timer, aei_timer_expire_cb, aei_timer_stop_cb);

    // init avi and uri
    k_timer_init(&avi_timer, avi_timer_expire_cb, NULL);
    k_timer_init(&uri_timer, uri_timer_expire_cb, NULL);

    // init vrp
    k_timer_init(&vrp_timer, NULL, NULL);

    printk("Forcing ventricle event...");
    observe("ventricle");
    // printk("line 112");

}

