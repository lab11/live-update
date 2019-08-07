/*
 * Simple Pacemaker (sequential, non-updatable)
 */

#include "interface.h"

#define VENTRICLE_SENSE_PIN 5
#define VENTRICLE_PACE_PIN  6
#define ATRIAL_SENSE_PIN    7
#define ATRIAL_PACE_PIN     8 // the shocker

#define TLRI_MS 1000
#define TAVI_MS 150

enum pacemaker_state {
    LRI =  0,
    ASed = 1
};

static enum pacestate state = LRI;
static struct k_timer t;

void timer_expire_cb(struct k_timer *t) {
    k_timer_stop(&t);

    // Shock the heart!
    gpio_write(ATRIAL_PACE_PIN, 0x1);
    k_sleep(1);
    gpio_write(ATRIAL_PACE_PIN, 0x0);
}

void timer_stop_cb(struct k_timer *t) {
    // restart the timer!
    k_timer_start(t, TLRI_MS - TAVI_MS, TLRI_MS - TAVI_MS);
}

void main(void) {

    gpio_enable_output(ATRIAL_PACE_PIN);

    k_timer_init(&t, timer_expire_cb, timer_stop_cb);
    k_timer_start(&t, TLRI_MS - TAVI_MS, TLRI_MS - TAVI_MS);

    while(1) {
        switch (state) {
        case LRI:
            if (gpio_read(VENTRICLE_SENSE_PIN) || gpio_read(VENTRICLE_PACE_PIN)) {
                k_timer_stop(&t);
            } else if (gpio_read(ATRIAL_SENSE_PIN)) {
                state = ASed;
            }
            break;
        case ASed:
            if (gpio_read(VENTRICLE_SENSE_PIN) || gpio_read(VENTRICLE_PACE_PIN)) {
                k_timer_stop(&t);
                state = LRI;
            }
        }
    }
    k_sleep(5);
}

