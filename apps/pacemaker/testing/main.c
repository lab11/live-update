// RTT test app
//
// Says hello

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "nrf.h"
#include "nrf_delay.h"
#include "nrf_gpio.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_serial.h"

#include "test_cases.h"

#define BTN NRF_GPIO_PIN_MAP(0,13)

#define LED1 NRF_GPIO_PIN_MAP(0,17)
#define LED2 NRF_GPIO_PIN_MAP(0,18)

#define GPIO1 NRF_GPIO_PIN_MAP(0,22)
#define GPIO2 NRF_GPIO_PIN_MAP(0,19)
#define GPIO3 NRF_GPIO_PIN_MAP(0,20)

#define TAVI    150
#define TLRI    1000

uint32_t cycles(void) {
	return DWT->CYCCNT;
}

int main(void) {

    ret_code_t error_code = NRF_SUCCESS;
    error_code = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(error_code);
    NRF_LOG_DEFAULT_BACKENDS_INIT();

    nrf_gpio_cfg_output(GPIO1);
    nrf_gpio_cfg_output(GPIO3);

    // nrf_gpio_cfg_input(GPIO2, NRF_GPIO_PIN_PULLUP);

    // Initialize.
    nrf_gpio_cfg_output(LED1);
    nrf_gpio_cfg_output(LED2);
    // nrf_gpio_pin_set(LED1);

    // nrf_gpio_cfg_input(BTN, NRF_GPIO_PIN_PULLUP);

    for(int i = 0; i < 10; i++) {
        printf("%d \n", i);
        nrf_delay_ms(700);
    }
 
    // Enter main loop.
    while (1) {
        DWT->CYCCNT = 0
	    DWT->CTRL |= 0x1; 

        uint32_t cycle_count = cycles();
        // Ventricle Sensing
        printf("Ventricle event \n");
        nrf_gpio_pin_write(GPIO1, 0);
        nrf_gpio_pin_write(GPIO1, 1);
        nrf_gpio_pin_toggle(LED1);

        nrf_delay_ms(TLRI - TAVI);
        nrf_gpio_pin_toggle(LED1);
        // Atrial Sensing
        printf("Atrial event \n");
        nrf_gpio_pin_write(GPIO3, 0);
        nrf_gpio_pin_write(GPIO3, 1);
        nrf_gpio_pin_toggle(LED2);

        nrf_delay_ms(TAVI);
        nrf_gpio_pin_toggle(LED2);



        char delim[] = ";";
        char *ptr = strtok(NORMAL_HEART_TEST, delim);

        while(ptr != NULL) {
            char delim2[] = ",";
            char *ptr2 = strtok(ptr, delim2);

            while(ptr2 != NULL) {
                printf("'%s'\n", ptr2);
                ptr2 = strtok(NULL, delim2);
            }
            
            ptr = strtok(NULL, delim);
        }

        // printf("%s \n", NORMAL_HEART_TEST);
        
        // FILE *fp;
        // char buff[255];

        // fp = fopen("normal_heart_test.txt", "r");
        // fscanf(fp, "%s", buff);
        // printf("1 : %s\n", buff );

        // fclose(fp);

        return 0;



    }
}

