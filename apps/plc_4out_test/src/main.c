#include "nrf_gpio.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_pwr_mgmt.h"
#include "nrf_serial.h"
#include "nrf_drv_clock.h"
#include "nrf_drv_spi.h"
#include "nrf_sdh.h"
#include "nrf_gpiote.h"
#include "nrfx_gpiote.h"
#include "nrf_delay.h"
#include "nrfx_timer.h"

#include "app_timer.h"
// #include "app_gpiote.h"
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

#define MUSCA_ON 25						// gpio pin that starts outputs			(output)
#define MUSCA_OFF 24		 			// gpio pin that stops outputs			(output)	(use NRF_BUTTON)
#define MUSCA_OUT3 19					// gpio pin to read out3 from musca		(input)
#define MUSCA_OUT2 20					// gpio pin to read out2 from musca		(input)
#define MUSCA_OUT1 22					// gpio pin to read out1 from musca		(input)
#define MUSCA_OUT0 23					// gpio pin to read out0 from musca		(input)

#define LED1 17							// specify LED pin

#define TIMER_PERIOD_MS 10000			// desired timer period in milliseconds
#define NRF_BUTTON 13					// Button 1 on nrf52

ret_code_t error_code;          		// error checking - variously used 
nrfx_err_t nrfx_err_code;				// error checking for nrfx libraries

// APP_TIMER_DEF(PUSH_TIMER);				// timer id to call PLC Cycle function at regular intervals

uint32_t tx_time;						// time signal sent to musca
uint32_t rx_time;						// time signal received by nrf
uint32_t res_time;						// approximate response time

nrfx_gpiote_in_config_t musca_out_config = NRFX_GPIOTE_CONFIG_IN_SENSE_LOTOHI(1);

nrfx_gpiote_in_config_t musca_stop_config = NRFX_GPIOTE_CONFIG_IN_SENSE_HITOLO(1);

nrfx_timer_t musca_start_timer = NRFX_TIMER_INSTANCE(1);					// Timer Instance
nrfx_timer_config_t timer_config = NRFX_TIMER_DEFAULT_CONFIG;		// Default Timer Configuration


/* NRFX Timer Callback Function */
static void musca_start(nrf_timer_event_t evt_type, void* p_context);	

/* NRFX GPIOTE Callback Functions */
static void musca_out_cb(nrfx_gpiote_pin_t pin, nrf_gpiote_polarity_t action);

static void musca_stop(nrfx_gpiote_pin_t pin, nrf_gpiote_polarity_t action);

/* App Timer Callback Function */
static void release_button(void * p_context);

APP_TIMER_DEF(BSIM_TIMER);

/********************* INIT *********************/

void rtt_init(void) {
    error_code = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(error_code);

    NRF_LOG_DEFAULT_BACKENDS_INIT();
    printf("Log initialized!\n");

}

void gpio_init(void) {
	printf("Initializing GPIO and GPIOTE peripherals...\n");

	/* Configure Inputs:
	 * MUSCA_OFF
	 * MUSCA_OUT0
	 * MUSCA_OUT1
	 * MUSCA_OUT2
	 * MUSCA_OUT3
	 */ 
	printf("Configuring Inputs...");

	nrfx_gpiote_init();

	nrfx_gpiote_in_init(NRF_BUTTON, 
				&musca_stop_config, 
				(nrfx_gpiote_evt_handler_t) musca_stop);
	nrfx_gpiote_in_init(MUSCA_OUT0,
				&musca_out_config,
				(nrfx_gpiote_evt_handler_t) musca_out_cb);
	nrfx_gpiote_in_init(MUSCA_OUT1,
				&musca_out_config,
				(nrfx_gpiote_evt_handler_t) musca_out_cb);
	nrfx_gpiote_in_init(MUSCA_OUT2,
				&musca_out_config,
				(nrfx_gpiote_evt_handler_t) musca_out_cb);
	nrfx_gpiote_in_init(MUSCA_OUT3,
				&musca_out_config,
				(nrfx_gpiote_evt_handler_t) musca_out_cb);

	nrfx_gpiote_in_event_enable(MUSCA_OFF, true);
	nrfx_gpiote_in_event_enable(MUSCA_OUT0, true);
	nrfx_gpiote_in_event_enable(MUSCA_OUT1, true);
	nrfx_gpiote_in_event_enable(MUSCA_OUT2, true);
	nrfx_gpiote_in_event_enable(MUSCA_OUT3, true);

	/* Configure Outputs:
	 * MUSCA_ON
	 * LED1
	 *
	 * Note: MUSCA_OFF is a directly linked to a button that the musca will read directly anyways
	 */ 

	printf("Configuring Outputs...");

	nrf_gpio_cfg_output(MUSCA_ON);
	nrf_gpio_cfg_output(LED1);

	nrf_gpio_pin_set(MUSCA_ON);
	nrf_gpio_pin_set(LED1);

	printf("GPIO and GPIOTE initialized!\n");
}

void timer_init(void) {
	printf("Initializing APP_TIMER Module...\n");

	// initialize clock

	error_code = nrf_drv_clock_init();
	APP_ERROR_CHECK(error_code);
	nrf_drv_clock_lfclk_request(NULL);

	error_code = app_timer_init();
	APP_ERROR_CHECK(error_code);

	error_code = app_timer_create(&BSIM_TIMER, APP_TIMER_MODE_SINGLE_SHOT, release_button);
	APP_ERROR_CHECK(error_code);

	printf("Initializing NRFX 16MHz High Frequency Timer...\n");

	timer_config.interrupt_priority = 7;
	timer_config.bit_width = NRF_TIMER_BIT_WIDTH_32;
	nrfx_timer_init(&musca_start_timer, 
					&timer_config, 
					(nrfx_timer_event_handler_t) musca_start);

	uint32_t timer_period_ticks = nrfx_timer_ms_to_ticks(&musca_start_timer, TIMER_PERIOD_MS);
	printf("Timer Period Set for %d ms...\n", TIMER_PERIOD_MS);

	nrfx_timer_extended_compare(&musca_start_timer,
								NRF_TIMER_CC_CHANNEL0,
								timer_period_ticks,
								NRF_TIMER_SHORT_COMPARE0_CLEAR_MASK,
								true);

	printf("Timer Initialized!\n"); // Remember to enable the timer in main
}

/***********************************************/

/******** Callback and Helper Functions ********/

float timer_16MHz_ticks_to_ms(uint32_t ticks) {
	return ((float) ticks) / 16000;
}

static void musca_start(nrf_timer_event_t evt_type, void* p_context) {
	printf("Starting 4 Ouputs Now...");
	error_code = app_timer_start(BSIM_TIMER, APP_TIMER_TICKS(50), NULL);
	APP_ERROR_CHECK(error_code);

	tx_time = nrfx_timer_capture(&musca_start_timer, 1);
	nrf_gpio_pin_clear(MUSCA_ON);
}

static void release_button(void* p_context) {
	nrf_gpio_pin_set(MUSCA_ON);
}

static void musca_out_cb(nrfx_gpiote_pin_t pin, nrf_gpiote_polarity_t action) {
	rx_time = nrfx_timer_capture(&musca_start_timer, 2);
	res_time = (rx_time < tx_time) ? (rx_time + (1 + ~tx_time)) : (rx_time - tx_time);

	// printf("Tx Time: %d\n", tx_time);
	// printf("Rx Time: %d\n", rx_time);
	// printf("Response Time: %d ticks\n", res_time);
	switch(pin) {
		case MUSCA_OUT0 :
			printf("OUT0 Res Time: %f ms\n", timer_16MHz_ticks_to_ms(res_time));
			break;
		case MUSCA_OUT1 :
			printf("OUT1 Res Time: %f ms\n", timer_16MHz_ticks_to_ms(res_time));
			break;
		case MUSCA_OUT2 :
			printf("OUT2 Res Time: %f ms\n", timer_16MHz_ticks_to_ms(res_time));
			break;
		case MUSCA_OUT3 :
			printf("OUT3 Res Time: %f ms\n\n", timer_16MHz_ticks_to_ms(res_time));
			break;
	}
	return;
}

static void musca_stop(nrfx_gpiote_pin_t pin, nrf_gpiote_polarity_t action) {
	printf("Stopped 4 Ouputs.\n\n");
}

/***********************************************/

int main(void) {
	rtt_init();
	gpio_init();
	timer_init();

	// // initialize timers
	// error_code = app_timer_init();
	// APP_ERROR_CHECK(error_code);

	// // create testing timer
	// error_code = app_timer_create(&PUSH_TIMER, APP_TIMER_MODE_REPEATED, sim_push_button);
	// APP_ERROR_CHECK(error_code);

	// // start testing timer
	// error_code = app_timer_start(PUSH_TIMER, APP_TIMER_TICKS(5000), NULL);
	// APP_ERROR_CHECK(error_code);

	printf("HF Timer Enabled!\n\n");
	nrfx_timer_enable(&musca_start_timer);

	// printf("Button Simulator Enabled!\n");
	// error_code = app_timer_start(BSIM_TIMER, APP_TIMER_TICKS(TIMER_PERIOD_MS), NULL);

	// nrf_delay_ms(1000);
	// res_time = nrfx_timer_capture(&musca_start_timer, 1);
	// printf("Test - timer_capture_get: %d\n", res_time);

	while(true) {
		__WFI();
	}
}