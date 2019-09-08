#include "interface.h"
#include "lri.c"
#include "avi.c"
#include "vrp.c"
#include "stdbool.h"
#include <stdarg.h>


// Pins
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
#define STACKSIZE 1024
#define PRIORITY 7

// static enum pacestate state = LRI;
// bool VSed;
// bool ASed;

bool VP_allowed;

// Timers
static struct k_timer avi_timer;
static struct k_timer uri_timer;
static struct k_timer vrp_timer;
static struct k_timer lri_timer;
static struct k_timer aei_timer;

char * event;

/* Will notify all FSM threads that atrial event or ventricular event has been sensed; takes var args*/
void notify_threads(int arg_count, ...) {
    va_list valist;
    va_start(valist, arg_count);
    int i;

    for (i = 0; i < arg_count; i++) {
      void (*fun_ptr)() = va_arg(valist, int); // Not sure about this syntax
      (*fun_ptr)(event); // This should call the observe function in the respective thread
   }

   va_end(valist);
}

/* Used by other threads to submit obervations 
   If being invoked for a Ventricle Pacing event, then calling thread MUST guarantee that VP_allowed = 1 prior to calling 
   Can also be invoked by GPIO inputs in which case VP_allowed should not be checked
*/
void observe(char* event) {
    if (!strcmp(event, "ventricle")) {
        // The below function calls need to be chained together
        event = "ventricle";
        gpio_write(VENTRICLE_PACE_PIN, 0x1); // sets pin=on
        notify_threads(3, &uri_observe, &vrp_observe, &aei_observe, &lri_observe);
        gpio_write(VENTRICLE_PACE_PIN, 0x0); // sets pin=on
    } else if (!strcmp(event, "atrial")) {
        event="atrial";
        gpio_write(ATRIAL_PACE_PIN, 0x1); // sets pin=on
        notify_threads(1, &avi_observe);
        gpio_write(ATRIAL_PACE_PIN, 0x0); // sets pin=on
    }

}

void main(void) {
    gpio_enable_output(ATRIAL_PACE_PIN);
    gpio_enable_output(VENTRICLE_PACE_PIN);

    /* These are copied over from pacemaker_example code, but not sure how they function */
    // gpio_add_oneshot(VENTRICLE_SENSE_PIN, ventricle_sense_cb);
    // gpio_add_oneshot(VENTRICLE_PACE_PIN, ventricle_pace_cb);
    // gpio_add_oneshot(ATRIAL_SENSE_PIN, atrial_pace_cb);

    /* Three threads corresponding to three groups of timing components will be spawned */
    K_THREAD_DEFINE(lri_id, STACKSIZE, lri_entry, NULL, NULL, NULL,PRIORITY, 0, K_NO_WAIT); // need to include in interface.h maybe?
    K_THREAD_DEFINE(avi_id, STACKSIZE, avi_entry, NULL, NULL, NULL,PRIORITY, 0, K_NO_WAIT);
    K_THREAD_DEFINE(vrp_id, STACKSIZE, vrp_entry, NULL, NULL, NULL,PRIORITY, 0, K_NO_WAIT);

    // Force ventrical pace event
    observe("ventricle")

    // TODO: Add support for receiving inputs from GPIO pins
}

