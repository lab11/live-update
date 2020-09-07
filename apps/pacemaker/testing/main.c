// Pacemaker Testing App

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

#define INPUT_TRACE NORMAL_HEART_TEST

#define BTN NRF_GPIO_PIN_MAP(0,13)

#define LED1 NRF_GPIO_PIN_MAP(0,17)
#define LED2 NRF_GPIO_PIN_MAP(0,18)

#define VSENSE_PIN NRF_GPIO_PIN_MAP(0,25)
#define ASENSE_PIN NRF_GPIO_PIN_MAP(0,24)
#define VPACE_PIN NRF_GPIO_PIN_MAP(0,23)
#define APACE_PIN NRF_GPIO_PIN_MAP(0,22)

#define UPDATE_FINISHED NRF_GPIO_PIN_MAP(0,18)
#define WRITE_FINISHED NRF_GPIO_PIN_MAP(0,19)

#define HEART_RATE_MAX_MS 810
#define HEART_RATE_MIN_MS 800
#define HEART_RATE_VENTRICLE_OFFSET 100
#define HEART_VENTRICLE_CHANNEL NRF_TIMER_CC_CHANNEL0
#define HEART_ATRIAL_CHANNEL NRF_TIMER_CC_CHANNEL1

static uint32_t prev_time = 0;

uint32_t get_rand_ms(uint32_t min, uint32_t max) {
    static uint32_t state = 0xf00d;

    uint32_t x = state;
    x ^= x << 13;
    x ^= x >> 17; 
    x ^= x << 5; 
    state = x;

    return min + (state % (max - min));
}

static uint32_t read_timer() {
    NRF_TIMER4->TASKS_CAPTURE[2] = 1;
    return NRF_TIMER4->CC[2];
}

void TIMER4_IRQHandler(void) {
    while (NRF_TIMER4->EVENTS_COMPARE[0] == 1 || NRF_TIMER4->EVENTS_COMPARE[1] == 1) {

        if (NRF_TIMER4->EVENTS_COMPARE[0] == 1) {
            NRF_TIMER4->EVENTS_COMPARE[0] = 0; // reset interrupt flag

            // XXX: force ventricle pacing all the time

            /*
            // send ventricle sense to pacemaker
            nrf_gpio_pin_write(VSENSE_PIN, 0);
            nrf_delay_us(500);
            nrf_gpio_pin_write(VSENSE_PIN, 1);

            //printf("ventricle timer\n");
            uint32_t curr_time = read_timer();
            uint32_t duration = curr_time >= prev_time ? curr_time - prev_time : (0xffff - prev_time) + curr_time;
            printf("VS,%lu\n", duration / 16);
            prev_time = curr_time;
            */
        }
    
        if (NRF_TIMER4->EVENTS_COMPARE[1] == 1) {
            NRF_TIMER4->EVENTS_COMPARE[1] = 0; // reset interrupt flag

            uint32_t atrial_delay_ticks = nrf_timer_ms_to_ticks(
                get_rand_ms(HEART_RATE_MIN_MS, HEART_RATE_MAX_MS),
                NRF_TIMER_FREQ_16MHz
            );
            NRF_TIMER4->CC[0] = read_timer() + nrf_timer_ms_to_ticks(HEART_RATE_VENTRICLE_OFFSET, NRF_TIMER_FREQ_16MHz);
            NRF_TIMER4->CC[1] += atrial_delay_ticks;

            // send atrial sense to pacemaker
            nrf_gpio_pin_write(ASENSE_PIN, 0);
            nrf_delay_us(500);
            nrf_gpio_pin_write(ASENSE_PIN, 1);

            // printf("atrial timer\n");
            uint32_t curr_time = read_timer();
            uint32_t duration = curr_time >= prev_time ? curr_time - prev_time : (0xffff - prev_time) + curr_time;
            printf("AS,%lu\n", duration / 16);
            prev_time = curr_time;
        }
    }
}

void in_pin_handler(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action) {

    if (pin == VPACE_PIN) {

        NRF_TIMER4->CC[0] = 0;

        uint32_t curr_time = read_timer();
        uint32_t duration = curr_time >= prev_time ? curr_time - prev_time : (0xffff - prev_time) + curr_time;
        printf("VP,%lu\n", duration / 16);
        prev_time = curr_time;

        // printf("ventricle pace happened at %d would have generated get at %d\n", actual_ticks, expected_ticks);

    } else if (pin == APACE_PIN) {
        uint32_t atrial_delay_ticks = nrf_timer_ms_to_ticks(
            get_rand_ms(HEART_RATE_MIN_MS, HEART_RATE_MAX_MS),
            NRF_TIMER_FREQ_16MHz
        );

        uint32_t actual_ticks = read_timer();
        NRF_TIMER4->CC[1] = actual_ticks + atrial_delay_ticks;
        NRF_TIMER4->CC[0] = actual_ticks + nrf_timer_ms_to_ticks(HEART_RATE_VENTRICLE_OFFSET, NRF_TIMER_FREQ_16MHz);

        uint32_t curr_time = read_timer();
        uint32_t duration = curr_time >= prev_time ? curr_time - prev_time : (0xffff - prev_time) + curr_time;
        printf("AP,%lu\n", duration / 16);
        prev_time = curr_time;

        // printf("atrial pace happened at %d would have generated get at %d\n", actual_ticks, expected_ticks);
    } else if (pin == WRITE_FINISHED) {
        printf("W,0\n");
    } else if (pin == UPDATE_FINISHED) {
        printf("U,0\n");
    }
    // fflush(stdout)
}

int main(void) {

    printf("S,0\n");

    // Init GPIO and RTT for logging
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
    nrf_gpio_cfg_output(VSENSE_PIN);
    nrf_gpio_cfg_output(ASENSE_PIN);

    // Configure Inputs and Interrupts
    nrf_drv_gpiote_in_config_t in_config = GPIOTE_CONFIG_IN_SENSE_LOTOHI(false);
    in_config.pull = NRF_GPIO_PIN_PULLDOWN;

    error_code = nrf_drv_gpiote_in_init(VPACE_PIN, &in_config, in_pin_handler);
    APP_ERROR_CHECK(error_code);
    nrf_drv_gpiote_in_event_enable(VPACE_PIN, true);
    
    in_config.pull = NRF_GPIO_PIN_PULLDOWN;
    error_code = nrf_drv_gpiote_in_init(APACE_PIN, &in_config, in_pin_handler);
    APP_ERROR_CHECK(error_code);
    nrf_drv_gpiote_in_event_enable(APACE_PIN, true);

    error_code = nrf_drv_gpiote_in_init(UPDATE_FINISHED, &in_config, in_pin_handler);
    APP_ERROR_CHECK(error_code);
    nrf_drv_gpiote_in_event_enable(UPDATE_FINISHED, true);

    error_code = nrf_drv_gpiote_in_init(WRITE_FINISHED, &in_config, in_pin_handler);
    APP_ERROR_CHECK(error_code);
    nrf_drv_gpiote_in_event_enable(WRITE_FINISHED, true);

    // Start Atrial and Ventricle state machine timers
    NRF_TIMER4->BITMODE = 3; // 32-bit
    NRF_TIMER4->PRESCALER = 0; // 16 MHz clock
    NRF_TIMER4->INTENSET = 1 << TIMER_INTENSET_COMPARE0_Pos;
    NRF_TIMER4->INTENSET = 1 << TIMER_INTENSET_COMPARE1_Pos;

    NVIC_ClearPendingIRQ(TIMER4_IRQn);
    NVIC_SetPriority(TIMER4_IRQn, 0);
    NVIC_EnableIRQ(TIMER4_IRQn);

    NRF_TIMER4->TASKS_CLEAR = 1;

    uint32_t atrial_delay_ticks = nrf_timer_ms_to_ticks(
        get_rand_ms(HEART_RATE_MIN_MS, HEART_RATE_MAX_MS),
        NRF_TIMER_FREQ_16MHz
    ); 
    NRF_TIMER4->CC[1] = atrial_delay_ticks;
    NRF_TIMER4->CC[0] = atrial_delay_ticks + nrf_timer_ms_to_ticks(HEART_RATE_VENTRICLE_OFFSET, NRF_TIMER_FREQ_16MHz);

    NRF_TIMER4->TASKS_START = 1;

    nrf_gpio_pin_write(VSENSE_PIN, 0);
    nrf_delay_us(500);
    nrf_gpio_pin_write(VSENSE_PIN, 1);

    // send initial ventricle sense to pacemaker
    /*
    nrf_gpio_pin_write(VSENSE_PIN, 0);
    nrf_delay_us(500);
    nrf_gpio_pin_write(VSENSE_PIN, 1);
    */

    //printf("end main\n");

    volatile int b = 1;
    while (b);

    return 0;
}

