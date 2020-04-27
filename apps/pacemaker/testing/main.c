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
#include "nrf_drv_gpiote.h"
#include "nrf_gpiote.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_serial.h"

#include "app_timer.h"
// #include "nrf_drv_clock.c"
#include "nrf_drv_clock.h"
// hal/timer.h
// include nrf.h and nrf_timer.h to get access 
#include "nrf_timer.h"
#include "test_cases.h"

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

APP_TIMER_DEF(m_repeated_timer_id);     /**< Handler for repeated timer used to blink LED 1. */
// APP_TIMER_DEF(m_single_shot_timer_id);  /**< Handler for single shot timer used to light LED 2. */

// clock_t start;
// clock_t end;
// int function_time;

uint32_t cycles(void) {
	return DWT->CYCCNT;
}

void in_pin_handler(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action) {

    // tick_count = app_timer_cnt_get();
    // ms = tick_count*((APP_TIMER_CONFIG_RTC_FREQUENCY+1)*1000) / APP_TIMER_CLOCK_FREQ;
    // printf("Timer count: %lu \n", tick_count);
    // printf("ms count: %lu \n", ms);

    nrf_timer_task_trigger(NRF_TIMER1, NRF_TIMER_TASK_CAPTURE1);
    uint32_t elapsed_time = nrf_timer_cc_read(NRF_TIMER1, NRF_TIMER_CC_CHANNEL1) - nrf_timer_cc_read(NRF_TIMER1, NRF_TIMER_CC_CHANNEL0);

    if (pin == GPIO4) printf("Atrial sense on pin %d \n", pin);
    if (pin == GPIO3) printf("Ventricle sense on pin %d \n", pin);

    printf("Elapsed Time (ms): %lu \n", elapsed_time/16000);

    
}

/**@brief Function starting the internal LFCLK oscillator.
 *
 * @details This is needed by RTC1 which is used by the Application Timer
 *          (When SoftDevice is enabled the LFCLK is always running and this is not needed).
 */
// static void lfclk_request(void)
// {
//     ret_code_t err_code = nrf_drv_clock_init();
//     APP_ERROR_CHECK(err_code);
//     nrf_drv_clock_lfclk_request(NULL);
// }

/**@brief Timeout handler for the repeated timer.
 */
// static void repeated_timer_handler(void * p_context)
// {
//     printf("Timed out!");
//     // nrf_drv_gpiote_out_toggle(LED_1);
// }

/**@brief Create timers.
 */
// static void create_timers()
// {
//     ret_code_t err_code;

//     // Create timers
//     err_code = app_timer_create(&m_repeated_timer_id,
//                                 APP_TIMER_MODE_REPEATED,
//                                 repeated_timer_handler);
//     APP_ERROR_CHECK(err_code);
// }

// /**@brief Timeout handler for the single shot timer.
//  */
// static void single_shot_timer_handler(void * p_context)
// {
//     nrf_drv_gpiote_out_clear(LED_2);
// }

bool is_int(char* input){
    if( !isdigit(input[0]) ){
        return false;
    }
    
    return true;
}

int main(void) {
    nrf_drv_gpiote_init();
    // app_timer_init();
    ret_code_t error_code = NRF_SUCCESS;
    error_code = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(error_code);

    if(!nrf_drv_gpiote_is_init()) {
        error_code = nrf_drv_gpiote_init();
        APP_ERROR_CHECK(error_code);
    }

    NRF_LOG_DEFAULT_BACKENDS_INIT();

    // lfclk_request();

    // Configure Output
    nrf_gpio_cfg_output(GPIO1);
    nrf_gpio_cfg_output(GPIO2);

    // Attempt to get hw timer working

    // NRF_TIMER_Type * timer = NRF_TIMER1;
    nrf_timer_bit_width_set(NRF_TIMER1, NRF_TIMER_BIT_WIDTH_32);
    nrf_timer_frequency_set(NRF_TIMER1, NRF_TIMER_FREQ_16MHz);

    nrf_timer_task_trigger(NRF_TIMER1, NRF_TIMER_TASK_START);
    
    nrf_timer_task_trigger(NRF_TIMER1, NRF_TIMER_TASK_CAPTURE0);
    
    for(int i = 0; i < 10; i++) {
        printf("%d \n", i);
        nrf_delay_ms(1000);
    }

    nrf_timer_task_trigger(NRF_TIMER1, NRF_TIMER_TASK_CAPTURE1);
    uint32_t elapsed_time = nrf_timer_cc_read(NRF_TIMER1, NRF_TIMER_CC_CHANNEL1) - nrf_timer_cc_read(NRF_TIMER1, NRF_TIMER_CC_CHANNEL0);

    printf("Elapsed Time: %lu \n", elapsed_time);
    // multiply ticks by 1/16 million

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

    // Initialize.
    // nrf_gpio_cfg_output(LED1);
    // nrf_gpio_cfg_output(LED2);
    // nrf_gpio_pin_set(LED1);

    /* some working code */
    // tick_count = app_timer_cnt_get();
    // printf("Timer count: %lu \n", tick_count);
    // SEGGER_RTT_printf(1, "%sTime:%s%s %.7d\n",
    //                   RTT_CTRL_RESET,
    //                   RTT_CTRL_BG_BRIGHT_RED,
    //                   RTT_CTRL_TEXT_BRIGHT_WHITE,
    //                   1111111
    //                   );

    char *pChr = strtok (SAMPLE_HEART_TEST, ",;");
    printf("Sending sense event: %s \n", pChr);
    pChr = strtok (NULL, ",;");
    while (pChr != NULL) {
        if (isdigit(pChr[0])){
            printf("Waiting for %s ms \n", pChr);
            nrf_delay_ms(atoi(pChr));
            pChr = strtok (NULL, ",;");
            if (pChr[0] == 'V') {
                nrf_gpio_pin_write(GPIO1, 0);
                printf("Sending V sense event: %s \n", pChr);
                nrf_gpio_pin_write(GPIO1, 1);
                // nrf_gpio_pin_set(GPIO1);
                
                // nrf_gpio_pin_clear(GPIO1);
            } else {
                nrf_gpio_pin_write(GPIO2, 0);
                printf("Sending A sense event: %s \n", pChr);
                nrf_gpio_pin_write(GPIO2, 1);
                // nrf_gpio_pin_set(GPIO2);
                // printf("Sending A sense event: %s \n", pChr);
                // nrf_gpio_pin_clear(GPIO2);
            }
        } else {
            pChr = strtok (NULL, ",;");
        }   
    }

    // Enter main loop.
    // create_timers();

    // ret_code_t err_code;
    // err_code = app_timer_start(m_repeated_timer_id, APP_TIMER_TICKS(200000), NULL);
    // APP_ERROR_CHECK(err_code);

    while (1) { 

        // tick_count = app_timer_cnt_get();
        // ms = tick_count*((APP_TIMER_CONFIG_RTC_FREQUENCY+1)*1000) / APP_TIMER_CLOCK_FREQ;
        // printf("Timer count: %lu \n", tick_count);
        // printf("ms count: %lu \n", ms);

    }
    return 0;
}

