#pragma once

#include <kernel.h>

#include "plc_platform.h"

// typedef signed short SWORD;
// typedef unsigned char BOOL;

/* Available Components on nrf9160 Board */
#define LED1 2
#define LED2 3
#define LED3 4
#define LED4 5
#define BUTTON1 6
#define BUTTON2 7
#define SWITCH1 8
#define SWITCH2 9


#define USE_LED 1					/* true if the program uses on-board LEDs. false otherwise */

#define NUM_PINS 0					/* Number of pins to use as outputs */
static uint8_t output_pins[] = {};	/* List pins to use as outputs as {PIN0, PIN1, ...} */

/************** DO NOT EDIT THESE FUNCTIONS **************/

/* LDmicro-generated PLC scan */
void PlcCycle(void);	

/* Use this function as your timer callback function */
void plc_callback(struct k_timer *t);

/* Always call this before you initialize your plc timer */
void init_plc(void); 	

/************************** END **************************/


/************* FUNCTIONS CREATED BY LDMICRO **************/

BOOL Read_U_b_Xbutton(void);

BOOL Read_U_b_Yled(void);

void Write_U_b_Yled(BOOL v);

BOOL Read_U_b_Ytx(void);

void Write_U_b_Ytx(BOOL v);

