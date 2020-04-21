#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "app_error.h"
#include "app_timer.h"
#include "nrf.h"
#include "nrf_delay.h"
#include "nrf_log.h"
#include "nrf_gpio.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_pwr_mgmt.h"
#include "nrf_serial.h"
#include "nrf_drv_clock.h"
#include "nrf_drv_spi.h"
#include "nrf_sdh.h"
#include "nrfx_gpiote.h"
// #include "app_button.h"

// #include "buckler.h"
// #include "gpio.h" 
// #include "display.h"
// #include "simple_logger.h"
// #include "app_scheduler.h"

#include "ladder.h"

/*
 * NOTES
 * See app_timer.h for detailed use of timer module
 */

/******************* GLOBALS ********************/
ret_code_t error_code;          		// error checking - variously used 
APP_TIMER_DEF(SCAN_TIMER);				// timer id to call PLC Cycle function at regular intervals

/********************* INIT *********************/

static void rtt_init(void) {
    error_code = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(error_code);

    NRF_LOG_DEFAULT_BACKENDS_INIT();
    printf("Log initialized!\n");
}



/************************************************/


int main(void) {
	rtt_init();
	Config_GPIO();

	error_code = nrf_drv_clock_init();
	APP_ERROR_CHECK(error_code);
	nrf_drv_clock_lfclk_request(NULL);

	error_code = app_timer_init();
	APP_ERROR_CHECK(error_code);

	error_code = app_timer_create(&SCAN_TIMER, APP_TIMER_MODE_REPEATED, PlcCycle);
	APP_ERROR_CHECK(error_code);

	error_code = app_timer_start(SCAN_TIMER, APP_TIMER_TICKS(10), NULL);
	APP_ERROR_CHECK(error_code);

	while(true) {
		__WFI();
	}
}