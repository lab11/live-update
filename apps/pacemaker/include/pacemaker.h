#include "kernel.h"
#include "tfm_gpio_veneers.h"

// Pins
#define LED                 2
#define VENTRICLE_SENSE_PIN 6 //Vget! action
#define ATRIAL_SENSE_PIN    7 //Aget! action
#define VENTRICLE_PACE_PIN  8 //VP!
#define ATRIAL_PACE_PIN     9 //AP!; the shocker

typedef enum {
    VENTRICLE=0, 
    ATRIAL=1
} EventType_t;

void observe(EventType_t event);
void lri_observe();
void aei_observe();
void lri_timer_stop_cb(struct k_timer *t);
void aei_timer_stop_cb(struct k_timer *t);
void aei_timer_expire_cb(struct k_timer *t);
void vrp_observe();
void uri_observe();
void avi_observe();
void uri_timer_expire_cb(struct k_timer *t);
void avi_timer_expire_cb(struct k_timer *t);
void vrp_timer_expire_cb(struct k_timer *t);
void notify_fsms_ventricle();
void atrial_pace();
void ventricle_pace();