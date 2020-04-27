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

// #include "app_timer.h"
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

#define BUTTON_SIM 25					// gpio pin that simulates button press
#define MUSCA_INPUT 24		 			// gpio pin to read input from musca 
#define LED1 17							// specify LED pin
#define TIMER_PERIOD_MS 5000			// desired timer period in milliseconds

ret_code_t error_code;          		// error checking - variously used 
nrfx_err_t nrfx_err_code;				// error checking for nrfx libraries

// APP_TIMER_DEF(PUSH_TIMER);				// timer id to call PLC Cycle function at regular intervals

uint32_t tx_time;						// time signal sent to musca
uint32_t rx_time;						// time signal received by nrf
uint32_t res_time;						// approximate response time

nrfx_gpiote_in_config_t input_config = NRFX_GPIOTE_CONFIG_IN_SENSE_LOTOHI(1);

nrfx_timer_t button_timer = NRFX_TIMER_INSTANCE(1);					// Timer Instance
nrfx_timer_config_t timer_config = NRFX_TIMER_DEFAULT_CONFIG;		// Default Timer Configuration

static void sim_push_button(void);
static void musca_input_callback(void);

/********************* INIT *********************/

void rtt_init(void) {
    error_code = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(error_code);

    NRF_LOG_DEFAULT_BACKENDS_INIT();
    printf("Log initialized!\n");

}

void gpio_init(void) {
	printf("Initializing GPIO and GPIOTE peripherals...\n");

	nrfx_gpiote_init();
	nrfx_gpiote_in_init(MUSCA_INPUT, 
				&input_config, 
				(nrfx_gpiote_evt_handler_t) musca_input_callback);

	nrfx_gpiote_in_event_enable(MUSCA_INPUT, true);

	nrf_gpio_cfg_output(BUTTON_SIM);
	nrf_gpio_cfg_output(LED1);

	nrf_gpio_pin_set(LED1);
	nrf_gpio_pin_set(BUTTON_SIM);

	printf("GPIO and GPIOTE initialized!\n");
}

void timer_init(void) {
	printf("Initializing NRFX 16MHz High Frequency Timer...\n");

	timer_config.interrupt_priority = 5;
	nrfx_timer_init(&button_timer, 
					&timer_config, 
					(nrfx_timer_event_handler_t) sim_push_button);

	uint32_t timer_period_ticks = nrfx_timer_ms_to_ticks(&button_timer, TIMER_PERIOD_MS);
	printf("Timer Period Set for %d ms...\n", TIMER_PERIOD_MS);

	nrfx_timer_extended_compare(&button_timer,
								NRF_TIMER_CC_CHANNEL0,
								timer_period_ticks,
								NRF_TIMER_SHORT_COMPARE0_CLEAR_MASK,
								true);

	printf("Timer Initialized!\n"); // Remember to enable the timer in main
}

/************************************************/

float timer_16MHz_ticks_to_ms(uint32_t ticks) {
	return ((float) ticks) / 16000;
}

static void sim_push_button(void) {
	printf("Simulate Button Press...\n");
	nrf_gpio_pin_clear(BUTTON_SIM);

	nrf_delay_ms(50);

	tx_time = nrfx_timer_capture(&button_timer, 1);
	nrf_gpio_pin_set(BUTTON_SIM);
}

static void musca_input_callback(void) {
	rx_time = nrfx_timer_capture(&button_timer, 1);
	res_time = rx_time - tx_time;

	printf("Tx Time: %d\n", tx_time);
	printf("Rx Time: %d\n", rx_time);
	printf("Response Time: %d ticks\n", res_time);
	printf("Response Time: %f ms\n\n", timer_16MHz_ticks_to_ms(res_time));
}

/************************************************/

int main(void) {
	rtt_init();
	gpio_init();
	timer_init();

	// initialize clock
	// error_code = nrf_drv_clock_init();
	// APP_ERROR_CHECK(error_code);
	// nrf_drv_clock_lfclk_request(NULL);

	// // initialize timers
	// error_code = app_timer_init();
	// APP_ERROR_CHECK(error_code);

	// // create testing timer
	// error_code = app_timer_create(&PUSH_TIMER, APP_TIMER_MODE_REPEATED, sim_push_button);
	// APP_ERROR_CHECK(error_code);

	// // start testing timer
	// error_code = app_timer_start(PUSH_TIMER, APP_TIMER_TICKS(5000), NULL);
	// APP_ERROR_CHECK(error_code);

	printf("Timer Enabled!\n");
	nrfx_timer_enable(&button_timer);

	// nrf_delay_ms(1000);
	// res_time = nrfx_timer_capture(&button_timer, 1);
	// printf("Test - timer_capture_get: %d\n", res_time);

	while(true) {
		__WFI();
	}
}