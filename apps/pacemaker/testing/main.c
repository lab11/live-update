// Pacemaker Teting App

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "nrf.h"
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "nrf_drv_gpiote.h"
#include "nrf_gpiote.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_serial.h"
#include "app_timer.h"
#include "nrf_drv_clock.h"
#include "nrf_timer.h"
#include "tests/test_cases1.h"
#include "SEGGER_RTT.h"

#include <time.h>

#define BTN NRF_GPIO_PIN_MAP(0,13)

#define LED1 NRF_GPIO_PIN_MAP(0,17)
#define LED2 NRF_GPIO_PIN_MAP(0,18)

#define GPIO1 NRF_GPIO_PIN_MAP(0,25)
#define GPIO2 NRF_GPIO_PIN_MAP(0,24)
#define GPIO3 NRF_GPIO_PIN_MAP(0,23)
#define GPIO4 NRF_GPIO_PIN_MAP(0,22)

#define TAVI    150
#define TLRI    1000

uint32_t tick_count;
uint32_t ms;
char* output_trace;

uint32_t cycles(void) {
	return DWT->CYCCNT;
}

void in_pin_handler(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action) {

    nrf_timer_task_trigger(NRF_TIMER1, NRF_TIMER_TASK_CAPTURE1);
    uint32_t elapsed_time = nrf_timer_cc_read(NRF_TIMER1, NRF_TIMER_CC_CHANNEL1) - nrf_timer_cc_read(NRF_TIMER1, NRF_TIMER_CC_CHANNEL0);

    if (pin == GPIO4) printf("AP,%lu \n", elapsed_time/16);
    if (pin == GPIO3) printf("VP,%lu \n", elapsed_time/16);

}

int main(void) {
    nrf_drv_gpiote_init();
    ret_code_t error_code = NRF_SUCCESS;
    error_code = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(error_code);

    if(!nrf_drv_gpiote_is_init()) {
        error_code = nrf_drv_gpiote_init();
        APP_ERROR_CHECK(error_code);
    }

    // NRF_LOG_DEFAULT_BACKENDS_INIT();

    // Configure Output
    nrf_gpio_cfg_output(GPIO1);
    nrf_gpio_cfg_output(GPIO2);

    // Configure Inputs and Interrupts
    nrf_drv_gpiote_in_config_t in_config = GPIOTE_CONFIG_IN_SENSE_TOGGLE(false);
    in_config.pull = NRF_GPIO_PIN_PULLDOWN;
    error_code = nrf_drv_gpiote_in_init(GPIO3, &in_config, in_pin_handler);
    APP_ERROR_CHECK(error_code);
    nrf_drv_gpiote_in_event_enable(GPIO3, true);
    
    nrf_drv_gpiote_in_config_t in_config1 = GPIOTE_CONFIG_IN_SENSE_TOGGLE(false);
    in_config1.pull = NRF_GPIO_PIN_PULLDOWN;
    error_code = nrf_drv_gpiote_in_init(GPIO4, &in_config1, in_pin_handler);
    APP_ERROR_CHECK(error_code);
    nrf_drv_gpiote_in_event_enable(GPIO4, true);

    // 10 Second Setup period
    for(int i = 0; i < 10; i++) {
        printf("%d \n", i);
        nrf_delay_ms(1000);
    }

    // Configure high frequency timer
    nrf_timer_bit_width_set(NRF_TIMER1, NRF_TIMER_BIT_WIDTH_32);
    nrf_timer_frequency_set(NRF_TIMER1, NRF_TIMER_FREQ_16MHz);
    nrf_timer_task_trigger(NRF_TIMER1, NRF_TIMER_TASK_START);
    nrf_timer_task_trigger(NRF_TIMER1, NRF_TIMER_TASK_CAPTURE0);
    

    // nrf_timer_task_trigger(NRF_TIMER1, NRF_TIMER_TASK_CAPTURE1);
    // uint32_t elapsed_time = nrf_timer_cc_read(NRF_TIMER1, NRF_TIMER_CC_CHANNEL1) - nrf_timer_cc_read(NRF_TIMER1, NRF_TIMER_CC_CHANNEL0);

    uint32_t elapsed_time;

    // Run an input trace
    char *pChr = strtok (ARR9_HEART_TEST, ",;");
    printf("Initiating input trace... \n");
    while (pChr != NULL) {
        if (isdigit(pChr[0])){
            nrf_delay_ms(atoi(pChr));
            pChr = strtok (NULL, ",;");
            if (pChr[0] == 'V') {
                nrf_timer_task_trigger(NRF_TIMER1, NRF_TIMER_TASK_CAPTURE1);
                nrf_gpio_pin_write(GPIO1, 0);
                elapsed_time = nrf_timer_cc_read(NRF_TIMER1, NRF_TIMER_CC_CHANNEL1) - nrf_timer_cc_read(NRF_TIMER1, NRF_TIMER_CC_CHANNEL0);
                printf("VS,%lu \n", elapsed_time/16);
                nrf_gpio_pin_write(GPIO1, 1);

            } else if(pChr[0] == 'A') {
                nrf_timer_task_trigger(NRF_TIMER1, NRF_TIMER_TASK_CAPTURE1);
                nrf_gpio_pin_write(GPIO2, 0);
                elapsed_time = nrf_timer_cc_read(NRF_TIMER1, NRF_TIMER_CC_CHANNEL1) - nrf_timer_cc_read(NRF_TIMER1, NRF_TIMER_CC_CHANNEL0);
                printf("AS,%lu \n", elapsed_time/16);
                nrf_gpio_pin_write(GPIO2, 1);
            }
        } else {
            pChr = strtok (NULL, ",;");
        }   
    }
    printf("Finished trace. \n");

    return 0;
}

