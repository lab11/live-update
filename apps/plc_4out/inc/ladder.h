#pragma once

#include <kernel.h>

#include "plc_platform.h"

typedef signed short SWORD;
typedef unsigned char BOOL;

#define USE_LED 0					/* true if the program uses on-board LEDs. false otherwise */

#define NUM_PINS 4					/* Number of pins to use as outputs */
static uint8_t output_pins[] __attribute__((section(".rodata"))) = {PIN8, PIN9, PIN10, PIN11};	/* List pins to use as outputs as {PIN0, PIN1, ...} */

/************** DO NOT EDIT THESE FUNCTIONS **************/

/* LDmicro-generated PLC scan */
void PlcCycle(void);	

/* Use this function as your timer callback function */
void plc_callback(struct k_timer *t);

/* Always call this before you initialize your plc timer */
void init_plc(void); 	

/************************** END **************************/


/************* FUNCTIONS CREATED BY LDMICRO **************/

BOOL Read_U_b_XMStart(void);

BOOL Read_U_b_XStop(void);

BOOL Read_U_b_YOut0(void);
void Write_U_b_YOut0(BOOL v);

BOOL Read_U_b_YOut1(void);
void Write_U_b_YOut1(BOOL v);

BOOL Read_U_b_YOut2(void);
void Write_U_b_YOut2(BOOL v);

BOOL Read_U_b_YOut3(void);
void Write_U_b_YOut3(BOOL v);