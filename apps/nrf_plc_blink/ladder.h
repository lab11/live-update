#pragma once

typedef signed short SWORD;
typedef unsigned char BOOL;

#define BUTTON1 13
#define LED1 17

void Config_GPIO(void);

BOOL Read_U_b_Xbutton(void);

BOOL Read_U_b_Yled(void);

void Write_U_b_Yled(BOOL v);

void PlcCycle(void);