#pragma once

#include "tfm_gpio_veneers.h"

typedef signed short SWORD;
typedef unsigned char BOOL;

#define BUTTON 7
#define LED 2

void init_plc(void);

BOOL Read_U_b_Xbutton(void);

BOOL Read_U_b_Yled(void);

void Write_U_b_Yled(BOOL v);

void PlcCycle(void);