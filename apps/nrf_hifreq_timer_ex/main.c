#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_pwr_mgmt.h"
#include "nrf_serial.h"
#include "nrf_drv_clock.h"
#include "nrf_delay.h"
#include "nrfx_timer.h"

#define TIMER_PERIOD_US 5000000

ret_code_t error_code; 			// Error Checking

nrfx_timer_t timer_instance = NRFX_TIMER_INSTANCE(1);					// Timer Instance
nrfx_timer_config_t timer_configuration = NRFX_TIMER_DEFAULT_CONFIG;	// Default Timer Configuration

uint32_t timer_period_ticks;
uint32_t time_cnt = 0;
uint32_t time_prv = 0;
uint32_t time_elps = 0;

void timer_callback(void) {
	printf("Timer Fired!\n");
	time_cnt = nrfx_timer_capture(&timer_instance, 3);
	printf("CC3 State: %d\n", time_cnt);
}


int main(void) {
	error_code = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(error_code);

    NRF_LOG_DEFAULT_BACKENDS_INIT();
    printf("Log initialized!\n");

    nrfx_timer_init(&timer_instance, &timer_configuration, (nrfx_timer_event_handler_t) timer_callback);
    printf("Timer Instance Initialized!\n");

    // timer_period_ticks = nrfx_timer_us_to_ticks(&timer_instance, TIMER_PERIOD_US);
    // printf("Timer Period set for %d ticks...\n", timer_period_ticks);

    // nrfx_timer_extended_compare(&timer_instance, 
    // 							NRF_TIMER_CC_CHANNEL0, 
    // 							timer_period_ticks, 
    // 							NRF_TIMER_SHORT_COMPARE0_CLEAR_MASK, 
    // 							true);
    // printf("Timer Set in Extended Compare Mode!\n");

    nrfx_timer_enable(&timer_instance);

    printf("CC0: %d\n", nrfx_timer_capture(&timer_instance, 0));
    printf("CC1: %d\n", nrfx_timer_capture(&timer_instance, 1));
    printf("Timer Enabled!\n");
 //    time_cnt = nrfx_timer_capture(&timer_instance, 1);

	// printf("Time Counter: %d\n", time_cnt);

    while(1) {
        time_prv = nrfx_timer_capture(&timer_instance, 1);
    	nrf_delay_ms(4000);
    	time_cnt = nrfx_timer_capture(&timer_instance, 2);

        time_elps = (time_cnt < time_prv)? (time_cnt + (1 + ~time_prv)) : (time_cnt - time_prv);

        printf("Time Since CC0: %f\n", (float) (time_cnt - nrfx_timer_capture_get(&timer_instance, 0)) / 16000000);
    	printf("Time Elapsed: %f\n", (float) (time_elps) / 16000000);
    }
}