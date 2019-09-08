#include "interface.h"



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


typedef enum {
    Idle,
    LRI, 
    ASed
} pacemaker_state;

typedef enum {
    LRI, 
    ASed
} pacemaker_event;

// static enum pacestate state = LRI;

// timer
struct k_timer t;

//Prototype of eventhandlers
pacemaker_state LRIHandler(void) {
    return LRI;
}

pacemaker_state ASedHandler(void) {
    return ASed;
}


void timer_expire_cb(struct k_timer *t) {
    k_timer_stop(&t);

}

void timer_stop_cb(struct k_timer *t) {
    k_timer_start(t, TLRI - TAVI, TLRI - TAVI);
    printk("timer stopped\n");
}

void check_lri() {
    // gpio_enable_output(ATRIAL_PACE_PIN);

    // k_timer_init(&t, timer_expire_cb, timer_stop_cb);
    // k_timer_start(&t, TLRI - TAVI, TLRI - TAVI);

    
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
    k_sleep(5);
    
    // The clock is reset when a ventricular event (VS, VP) is received. 
    // If no atrial event has been sensed (AS), the component will deliver atrial pacing (AP) after TLRI-TAVI.
}

void check_avi() {

    // check_uri()
    // If no ventricular event has been sensed (VS) within TAVI after 
    // an atrial event (AS, AP), the component will deliver ventricular pacing (VP)
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
    pacemaker_state nextState = Idle;
    pacemaker_event nextEvent;

    // this function will initialize a timer
    // define inputs (from gpio pins)
    // use gpio_write or gpio_pin_write to write to pins?
    // use gpio_port_read to read inputs
    
    gpio_enable_output(ATRIAL_PACE_PIN);
    //Keep track of a global timer

    // k_timer_init(&t, timer_expire_cb, timer_stop_cb);
    // k_timer_start(&t, 500, 500);

    // while loop keeps checking if certain conditions have been met 
    while(1) {
        pacemaker_event nextEvent = ReadEvent() // Reading input from pins
        switch(nextState) {
            case Idle: 
        }
        // 
    }
}

