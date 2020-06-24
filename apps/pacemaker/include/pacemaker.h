#include "kernel.h"
#include "drivers/gpio.h"

#define TAVI    150
#define TLRI    1000
#define TPVARP  100
#define TVRP    150
#define TURI    400
#define TPVAB   50

// Pins
#define LED                 2
#define VENTRICLE_SENSE_PIN 6 //Vget! action
#define ATRIAL_SENSE_PIN    7 //Aget! action
#define VENTRICLE_PACE_PIN  8 //VP!
#define ATRIAL_PACE_PIN     9 //AP!; the shocker

typedef enum {
    VENTRICLE=0, 
    ATRIAL=1
} pacing_t;

void reset_t();
void observe_ventricle_sense();
void ventricle_pace();
void atrial_pace();

// LRI
void lri_ventricle_sense();
void lri_ventricle_pace();
void lri_atrial_sense();
void lri_timer_expire_cb(struct k_timer *t);

// AVI
void avi_ventricle_sense();
void avi_atrial_sense();
void avi_atrial_pace();
void avi_timer_expire_cb(struct k_timer *t);

// URI
void uri_ventricle_sense();
void uri_ventricle_pace();
void uri_clk_expire_cb(struct k_timer *t);

// VRP
void vrp_ventricle_get();
void vrp_ventricle_pace();
void vrp_timer_expire_cb(struct k_timer *t);

/*
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
*/

