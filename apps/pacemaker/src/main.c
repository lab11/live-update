// #include "interface.h"
#include "stdbool.h"
#include <stdarg.h>
#include <stddef.h>
#include <string.h>
#include "pacemaker.h"
#include "kernel.h"
#include "tfm_gpio_veneers.h"

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
bool VSed;
bool ASed;

// Timers
struct k_timer avi_timer;
struct k_timer uri_timer;
struct k_timer vrp_timer;
struct k_timer lri_timer;
struct k_timer aei_timer;

char * prev_event;

/* Callbacks for GPIO interrupts */
void ventricle_sense_cb(void) {
    printk("Sensed ventricle event! \n");
    observe("ventricle");
}

void atrial_sense_cb(void) {
    printk("Sensed atrial event! \n");
    ASed = true;
    observe("atrial");
}


void notify_fsms_ventricle() {
    uri_observe("ventricle");
    vrp_observe("ventricle");
    aei_observe("ventricle");
    lri_observe("ventricle");
}

void notify_fsms_atrial() {
    avi_observe("atrial");
}

/* Used by other threads to submit obervations 
   If being invoked for a Ventricle Pacing event, then calling thread MUST guarantee that VP_allowed = 1 prior to calling 
   Can also be invoked by GPIO inputs in which case VP_allowed should not be checked
*/
void observe(char* event) {
    if (!strcmp(event, "ventricle")) {
        // The below function calls need to be chained together
        printk("\n");
        printk("Observing Ventricle Event... \n");
        VP_allowed = 0;
        k_timer_stop(&lri_timer);
        k_timer_stop(&avi_timer);
        tfm_gpio_set(LED);
        // prev_event = "ventricle";
        notify_fsms_ventricle();
    } else if (!strcmp(event, "atrial")) {
        printk("\n");
        printk("Observing Atrial Event... \n");
        // prev_event="atrial";
        notify_fsms_atrial();
    }
}

void main(void) {
    // Enable output gpio pins
    tfm_gpio_enable_output(LED);
    tfm_gpio_enable_output(VENTRICLE_PACE_PIN);
    tfm_gpio_enable_output(ATRIAL_PACE_PIN);


    // Enable gpio interrupts
    gpio_int_config ventricle_cfg = {
        .type = 0,
        .polarity = 0,
        .cb = ventricle_sense_cb
    };
    tfm_gpio_interrupt_enable(VENTRICLE_SENSE_PIN, &ventricle_cfg);

    gpio_int_config atrial_cfg = {
        .type = 1,
        .polarity = 1,
        .cb = atrial_sense_cb
    };
    tfm_gpio_interrupt_enable(ATRIAL_SENSE_PIN, &atrial_cfg);


    // Initialize timers
    k_timer_init(&lri_timer, NULL, lri_timer_stop_cb);
    k_timer_init(&aei_timer, aei_timer_expire_cb, aei_timer_stop_cb);
    k_timer_init(&avi_timer, avi_timer_expire_cb, NULL);
    k_timer_init(&uri_timer, uri_timer_expire_cb, NULL);
    k_timer_init(&vrp_timer, vrp_timer_expire_cb, NULL);

}