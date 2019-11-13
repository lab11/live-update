/*
 * app/main.c
 *
 * Main non-secure application. 
 */

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "Driver_USART.h"
#include "target_cfg.h"

extern ARM_DRIVER_USART NS_DRIVER_STDIO;

FILE __stdout;
int fputc(int ch, FILE *f) {
    (void)NS_DRIVER_STDIO.Send((const unsigned char *)&ch, 1);
    return ch;
}

int _write(int fd, char *str, int len) {
    (void)NS_DRIVER_STDIO.Send(str, len);
    return len;
}

int main(void) {
    NS_DRIVER_STDIO.Initialize(NULL);
    NS_DRIVER_STDIO.Control(ARM_USART_MODE_ASYNCHRONOUS, 115200);

    printf("Hi from NS-land!!!\n");
    
    while(1);
}

