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

#define MUSCA_ON	 			13		// gpio pin to start musca program		(output)
#define MUSCA_OFF 				14		// gpio pin to stop musca program		(output)
#define MUSCA_Bottle_IN 		2		// gpio pin to sim bottle presence		(output)
#define MUSCA_Bottle_UP			26		// gpio pin to sim bottle upright		(output)
#define MUSCA_Position_DOWN		27		// gpio pin to sim bottle upside0down	(output)

#define MUSCA_Bottle_HOLD		22		// gpio pin to receive HOLD signal		(input)
#define MUSCA_Motor_FORWARD		20		// gpio pin to receive Motor FORWARD	(input)
#define MUSCA_Water_ON			19		// gpio pin to receive Water ON signal	(input)
#define MUSCA_Motor_REVERSE		18		// gpio pin to receive Motor Reverse	(input)
#define MUSCA_Bottle_RELEASE	17		// gpio pin to receive RELEASE signal	(input)


#define TIMER_PERIOD_MS 23000			// desired timer period in milliseconds
#define NRF_BUTTON1 13					// Button 1 on nrf52
#define NRF_BUTTON2 14					// Button 2 on nrf52

ret_code_t error_code;          		// error checking - variously used 
nrfx_err_t nrfx_err_code;				// error checking for nrfx libraries

// APP_TIMER_DEF(PUSH_TIMER);				// timer id to call PLC Cycle function at regular intervals

uint32_t tx_time;						// time signal sent to musca
uint32_t rx_time;						// time signal received by nrf
uint32_t res_time;						// approximate response time

nrfx_gpiote_in_config_t musca_output_config = NRFX_GPIOTE_CONFIG_IN_SENSE_LOTOHI(1);

nrfx_gpiote_in_config_t musca_btn_cb_config = NRFX_GPIOTE_CONFIG_IN_SENSE_HITOLO(1);

nrfx_timer_t musca_bottle_timer = NRFX_TIMER_INSTANCE(1);					// Timer Instance
nrfx_timer_config_t timer_config = NRFX_TIMER_DEFAULT_CONFIG;		// Default Timer Configuration


/* NRFX Timer Callback Function */
static void musca_bottle_cb(nrf_timer_event_t evt_type, void* p_context);	

/* NRFX GPIOTE Callback Functions */
static void musca_out_cb(nrfx_gpiote_pin_t pin, nrf_gpiote_polarity_t action);

static void musca_btn_cb(nrfx_gpiote_pin_t pin, nrf_gpiote_polarity_t action);

/* App Timer Callback Function */
static void sim_sensor(void* p_context);

static void release_button(void * p_context);

APP_TIMER_DEF(BSIM_TIMER);				// Button Simulator Timer
APP_TIMER_DEF(SSIM_TIMER);				// Sensor Simulator Timer

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
	 * MUSCA_Bottle_HOLD
	 * MUSCA_Motor_FORWARD
	 * MUSCA_Water_ON
	 * MUSCA_Motor_REVERSE
	 */ 
	printf("Configuring Inputs...\n");

	nrfx_gpiote_init();

	// printf("Configure NRF_BUTTON1...\n");
	nrfx_gpiote_in_init(NRF_BUTTON1, 
				&musca_btn_cb_config, 
				(nrfx_gpiote_evt_handler_t) musca_btn_cb);

		// printf("Configure NRF_BUTTON2...\n");
	nrfx_gpiote_in_init(NRF_BUTTON2, 
				&musca_btn_cb_config, 
				(nrfx_gpiote_evt_handler_t) musca_btn_cb);

	// printf("Configure MUSCA_Bottle_HOLD...\n");
	nrfx_gpiote_in_init(MUSCA_Bottle_HOLD,
				&MUSCA_ouput_config,
				(nrfx_gpiote_evt_handler_t) musca_out_cb);

	// printf("Configure MUSCA_Motor_FORWARD...\n");
	nrfx_gpiote_in_init(MUSCA_Motor_FORWARD,
				&musca_output_config,
				(nrfx_gpiote_evt_handler_t) musca_out_cb);

	// printf("Configure MUSCA_Water_ON...\n");
	nrfx_gpiote_in_init(MUSCA_Water_ON,
				&musca_output_config,
				(nrfx_gpiote_evt_handler_t) musca_out_cb);
	
	// printf("Configure MUSCA_Motor_REVERSE...\n");
	nrfx_gpiote_in_init(MUSCA_Motor_REVERSE,
				&musca_output_config,
				(nrfx_gpiote_evt_handler_t) musca_out_cb);

	// printf("Configure MUSCA_Bottle_RELEASE...\n");
	nrfx_gpiote_in_init(MUSCA_Bottle_RELEASE,
				&musca_output_config,
				(nrfx_gpiote_evt_handler_t) musca_out_cb);

	// printf("Enabling inputs...\n");

	// printf("Enabling NRF_BUTTON1...\n");
	nrfx_gpiote_in_event_enable(NRF_BUTTON1, true);
	// printf("Enabling NRF_BUTTON2...\n");
	nrfx_gpiote_in_event_enable(NRF_BUTTON2, true);
	// printf("Enabling MUSCA_Bottle_HOLD...\n");
	nrfx_gpiote_in_event_enable(MUSCA_Bottle_HOLD, true);
	// printf("Enabling MUSCA_Motor_FORWARD...\n");
	nrfx_gpiote_in_event_enable(MUSCA_Motor_FORWARD, true);
	// printf("Enabling MUSCA_Water_ON...\n");
	nrfx_gpiote_in_event_enable(MUSCA_Water_ON, true);
	// printf("Enabling MUSCA_Motor_REVERSE...\n");
	nrfx_gpiote_in_event_enable(MUSCA_Motor_REVERSE, true);
	// printf("Enabling MUSCA_Bottle_RELEASE...\n");
	nrfx_gpiote_in_event_enable(MUSCA_Bottle_RELEASE, true);

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

	error_code = app_timer_create(&SSIM_TIMER, APP_TIMER_MODE_SINGLE_SHOT, sim_sensor);
	APP_ERROR_CHECK(error_code);

	printf("Initializing NRFX 16MHz High Frequency Timer...\n");

	timer_config.interrupt_priority = 7;
	timer_config.bit_width = NRF_TIMER_BIT_WIDTH_32;
	nrfx_timer_init(&musca_bottle_timer, 
					&timer_config, 
					(nrfx_timer_event_handler_t) musca_bottle_cb);

	/* Uncomment to trigger application periodically. */

	// uint32_t timer_period_ticks = nrfx_timer_ms_to_ticks(&musca_bottle_timer, TIMER_PERIOD_MS);
	// printf("Timer Period Set for %d ms...\n", TIMER_PERIOD_MS);

	// nrfx_timer_extended_compare(&musca_bottle_timer,
	// 							NRF_TIMER_CC_CHANNEL0,
	// 							timer_period_ticks,
	// 							NRF_TIMER_SHORT_COMPARE0_CLEAR_MASK,
	// 							true);

	printf("Timer Initialized!\n"); // Remember to enable the timer in main
}

/***********************************************/

uint32_t* pin_context;

/******** Callback and Helper Functions ********/

float timer_16MHz_ticks_to_ms(uint32_t ticks) {
	return ((float) ticks) / 16000;
}

static void musca_bottle_cb(nrf_timer_event_t evt_type, void* p_context) {
	printf("Simulating Bottle Presence...\n");
	error_code = app_timer_start(BSIM_TIMER, APP_TIMER_TICKS(50), p_context);
	APP_ERROR_CHECK(error_code);

	tx_time = nrfx_timer_capture(&musca_bottle_timer, 1);
	nrf_gpio_pin_clear(MUSCA_Bottle_IN);
}

static void sim_sensor(void* p_context) {
	error_code = app_timer_start(BSIM_TIMER, APP_TIMER_TICKS(50), p_context);
	APP_ERROR_CHECK(error_code);

	tx_time = nrfx_timer_capture(&musca_bottle_timer, 1);
	nrf_gpio_pin_clear(*p_context);b
}

static void release_button(void* p_context) {
	nrf_gpio_pin_set(*p_context);
}

static void musca_out_cb(nrfx_gpiote_pin_t pin, nrf_gpiote_polarity_t action) {
	rx_time = nrfx_timer_capture(&musca_bottle_timer, 2);
	res_time = (rx_time < tx_time) ? (rx_time + (1 + ~tx_time)) : (rx_time - tx_time);

	// printf("Tx Time: %d\n", tx_time);
	// printf("Rx Time: %d\n", rx_time);
	// printf("Response Time: %d ticks\n", res_time);
	switch(pin) {
		case MUSCA_Bottle_HOLD :
			printf("Bottle_HOLD Res Time: %f ms\n", timer_16MHz_ticks_to_ms(res_time));
			tx_time = nrfx_timer_capture(&musca_bottle_timer, 1);
			*pin_context = MUSCA_Bottle_HOLD;
			error_code = app_timer_start(BSIM_TIMER, APP_TIMER_TICKS(50), (void*) pin_context);
			APP_ERROR_CHECK(error_code);
			break;
		case MUSCA_Motor_FORWARD :
			printf("Motor_FORWARD Res Time: %f ms\n", timer_16MHz_ticks_to_ms(res_time));
			// tx_time = nrfx_timer_capture(&musca_bottle_timer, 1);
			break;
		case MUSCA_Water_ON :
			printf("Water_ON Res Time: %f ms\n", timer_16MHz_ticks_to_ms(res_time));
			tx_time = nrfx_timer_capture(&musca_bottle_timer, 1);
			*pin_context = MUSCA_Water_ON;
			error_code = app_timer_start(BSIM_TIMER, APP_TIMER_TICKS(50), (void*) pin_context);
			APP_ERROR_CHECK(error_code);
			break;
		case MUSCA_Motor_REVERSE :
			printf("Motor REVERSE Res Time: %f ms\n", timer_16MHz_ticks_to_ms(res_time));
			break;
		case MUSCA_Bottle_RELEASE :
			printf("Bottle RELEASE Res Time: %f ms\n\n", timer_16MHz_ticks_to_ms(res_time));
			break;
	}
	return;
}

static void musca_btn_cb(nrfx_gpiote_pin_t pin, nrf_gpiote_polarity_t action) {
	switch(pin) {

		case NRF_BUTTON1 : // Stop Button
			printf("Stopped 4 Ouputs.\n\n");
			break;

		case NRF_BUTTON2 : // Start Case
			printf("Starting 4 Ouputs Now...\n");
			error_code = app_timer_start(BSIM_TIMER, APP_TIMER_TICKS(50), NULL);
			APP_ERROR_CHECK(error_code);

			tx_time = nrfx_timer_capture(&musca_bottle_timer, 1);
			nrf_gpio_pin_clear(MUSCA_ON);
			break;
	}
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
	nrfx_timer_enable(&musca_bottle_timer);

	// printf("Button Simulator Enabled!\n");
	// error_code = app_timer_start(BSIM_TIMER, APP_TIMER_TICKS(TIMER_PERIOD_MS), NULL);

	// nrf_delay_ms(1000);
	// res_time = nrfx_timer_capture(&musca_bottle_timer, 1);
	// printf("Test - timer_capture_get: %d\n", res_time);

	while(true) {
		__WFI();
	}
}