/*
 * Simple Pacemaker (asynchronous)
 */

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


bool VP_allowed;
bool VS_allowed;
bool VSed;
bool ASed;

// Timers
struct k_timer avi_timer;
struct k_timer uri_timer;
struct k_timer vrp_timer;
struct k_timer lri_timer;
struct k_timer aei_timer;
struct k_timer diagnostic_timer;

/* Callbacks for GPIO interrupts */
void ventricle_sense_cb(void) {
    printk("Sensed ventricle event! \n");
    observe(VENTRICLE);
}

void atrial_sense_cb(void) {
    printk("Sensed atrial event! \n");
    ASed = true;
    observe(ATRIAL);
}

void notify_fsms(EventType_t event) {
    if (event == VENTRICLE) {
        uri_observe();
        vrp_observe();
        aei_observe();
        lri_observe();
    } else if (event == ATRIAL) {
        avi_observe();
    }
}

/* Used by other threads to submit obervations 
   If being invoked for a Ventricle Pacing event, then calling thread MUST guarantee that VP_allowed = 1 prior to calling 
   Can also be invoked by GPIO inputs in which case VP_allowed should not be checked
*/
void observe(EventType_t event) {
    if (event == VENTRICLE) {
        // The below function calls need to be chained together
        printk("\n");
        printk("Observing Ventricle Event... \n");
        VP_allowed = 0;
        k_timer_stop(&lri_timer);
        k_timer_stop(&avi_timer);
        tfm_gpio_set(LED);
        notify_fsms(event);
    } else if (event == ATRIAL) {
        printk("\n");
        printk("Observing Atrial Event... \n");
        notify_fsms(event);
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

    // Used for diagnostic purposes to measure latency
    k_timer_init(&diagnostic_timer, NULL, NULL);

    // Uncomment to force ventricle event when no external inputs available
    printk("Forcing Ventricle Event");
    observe(VENTRICLE);
}