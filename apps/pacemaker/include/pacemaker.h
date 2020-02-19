#include "kernel.h"
#include "tfm_gpio_veneers.h"

// Pins
#define LED                 2
#define VENTRICLE_SENSE_PIN 5 //Vget! action
#define VENTRICLE_PACE_PIN  6 //VP!
#define ATRIAL_SENSE_PIN    7 //Aget! action
#define ATRIAL_PACE_PIN     8 //AP!; the shocker

void observe(char* event);
void lri_observe(char* event);
void aei_observe(char* event);
void lri_timer_stop_cb(struct k_timer *t);
void aei_timer_stop_cb(struct k_timer *t);
void aei_timer_expire_cb(struct k_timer *t);
void vrp_observe(char* event);
void uri_observe(char* event);
void avi_observe(char* event);
void uri_timer_expire_cb(struct k_timer *t);
void avi_timer_expire_cb(struct k_timer *t);
void vrp_timer_expire_cb(struct k_timer *t);
void notify_fsms_ventricle();