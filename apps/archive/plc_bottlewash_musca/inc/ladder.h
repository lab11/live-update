#pragma once

#include <kernel.h>

#include "plc_platform.h"

typedef signed short SWORD;
typedef unsigned char BOOL;

#define USE_LED 0					/* true if the program uses on-board LEDs. false otherwise */

#define NUM_PINS 5					/* Number of pins to use as outputs */
static uint8_t output_pins[] __attribute__((section(".rodata"))) = {PIN8, PIN9, PIN10, PIN11, PIN12};	/* List pins to use as outputs as {PIN0, PIN1, ...} */

/* PIN-IO MAPPING */
#define Start_PIN 			PIN7
#define Stop_PIN 			PIN6
#define Bottle_IN_PIN 		PIN2
#define Bottle_UP_PIN		PIN3
#define Position_DOWN_PIN	PIN4

#define Bottle_HOLD_PIN		PIN8
#define Motor_FORWARD_PIN	PIN9
#define Water_ON_PIN		PIN10
#define Motor_REVERSE_PIN	PIN11
#define Bottle_RELEASE_PIN	PIN12

/**********************/

/************** DO NOT EDIT THESE FUNCTIONS **************/

/* LDmicro-generated PLC scan */
void PlcCycle(void);	

/* Use this function as your timer callback function */
void plc_callback(struct k_timer *t);

/* Always call this before you initialize your plc timer */
void init_plc(void); 	

/************************** END **************************/


/************* FUNCTIONS CREATED BY LDMICRO **************/

BOOL Read_U_b_XStart(void);

BOOL Read_U_b_XStop(void);

BOOL Read_U_b_XBottle_IN(void);

BOOL Read_U_b_XBottle_UP(void);

BOOL Read_U_b_XPosition_DOWN(void);

BOOL Read_U_b_YBottle_HOLD(void);
void Write_U_b_YBottle_HOLD(BOOL v);

BOOL Read_U_b_YMotor_FORWARD(void);
void Write_U_b_YMotor_FORWARD(BOOL v);

BOOL Read_U_b_YWater_ON(void);
void Write_U_b_YWater_ON(BOOL v);

BOOL Read_U_b_YMotor_REVERSE(void);
void Write_U_b_YMotor_REVERSE(BOOL v);

BOOL Read_U_b_YBottle_RELEASE(void);
void Write_U_b_YBottle_RELEASE(BOOL v);


