#include "interface.h"

// timer
struct k_timer t;

// Params
#define TAVI    150
#define TLRI    1000
#define TPVARP  100
#define TVRP    150
#define TURI    400
#define TPVAB   50


void timer_expire_cb(struct k_timer *t) {

}

void timer_stop_cb(struct k_timer *t) {
    printk("timer stopped\n");
}

void check_lri() {
    // The clock is reset when a ventricular event (VS, VP) is received. 
    // If no atrial event has been sensed (AS), the component will deliver atrial pacing (AP) after TLRI-TAVI.
}

void check_avi() {
    // If no ventricular event has been sensed (VS) within TAVI after 
    // an atrial event (AS, AP), the component will deliverventricular pacing (VP)
}

void check_uri() {
    //  URI component uses a global clock clk to track the time after a ventricular event (VS, VP).
    // If the global clock value is less than TURI when the AVI component is about to 
    // deliver VP, AVI component will hold VP and deliver it after the global clock reaches TURI. 
}

void begin_vrp() {
    // a VRP period follows each ventricular event (VS, VP) in order to filter noise.
}

void main(void) {
    // this function will initialize a timer
    // define inputs (from gpio pins)
    // use gpio_write or gpio_pin_write to write to pins?
    // use gpio_port_read to read inputs
    // gpio_enable_output(2);
    //Keep track of a global timer

    // k_timer_init(&t, timer_expire_cb, timer_stop_cb);
    // k_timer_start(&t, 500, 500);

    // while loop keeps checking if certain conditions have been met 
    while(true) {
        // 
    }
}

