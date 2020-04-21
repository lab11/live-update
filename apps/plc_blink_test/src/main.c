#include "nrf_gpio.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_pwr_mgmt.h"
#include "nrf_serial.h"
#include "nrf_drv_clock.h"
#include "nrf_drv_spi.h"
#include "nrf_sdh.h"
#include "nrfx_gpiote.h"
#include "nrf_delay.h"

#include "app_timer.h"
// #include "app_button.h"

// #include "buckler.h"
// #include "gpio.h" 
// #include "display.h"
// #include "simple_logger.h"
// #include "app_scheduler.h"


/*
 * NOTES
 * See app_timer.h for detailed use of timer module
 */

/******************* GLOBALS ********************/

#define BUTTON_SIM 25					// gpio pin that simulates button press
#define MUSCA_INPUT 24		 			// gpio pin to read input from musca 
#define LED1 17

ret_code_t error_code;          		// error checking - variously used 
APP_TIMER_DEF(PUSH_TIMER);				// timer id to call PLC Cycle function at regular intervals

/********************* INIT *********************/

static void rtt_init(void) {
    error_code = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(error_code);

    NRF_LOG_DEFAULT_BACKENDS_INIT();
    printf("Log initialized!\n");
}

static void gpio_init(void) {
	nrf_gpio_cfg_input(MUSCA_INPUT);
	nrf_gpio_cfg_output(BUTTON_SIM);
	nrf_gpio_cfg_output(LED1);

	nrf_gpio_pin_clear(LED1);
	nrf_gpio_pin_set(BUTTON_SIM);
}

/************************************************/

static void sim_push_button(void) {
	nrf_gpio_pin_clear(BUTTON_SIM);

	nrf_delay_ms(100);

	nrf_gpio_pin_set(BUTTON_SIM);
}

static void musca_input_callback() {
	
}

/************************************************/

int main(void) {
	rtt_init();

	// initialize clock
	error_code = nrf_drv_clock_init();
	APP_ERROR_CHECK(error_code);
	nrf_drv_clock_lfclk_request(NULL);

	// initialize timers
	error_code = app_timer_init();
	APP_ERROR_CHECK(error_code);

	// create testing timer
	error_code = app_timer_create(&PUSH_TIMER, APP_TIMER_MODE_REPEATED, sim_push_button);
	APP_ERROR_CHECK(error_code);

	// start testing timer
	error_code = app_timer_start(PUSH_TIMER, APP_TIMER_TICKS(5000), NULL);
	APP_ERROR_CHECK(error_code);

	while(true) {
		__WFI();
	}
}