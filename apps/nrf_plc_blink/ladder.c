#include "ladder.h"

#include "app_error.h"
#include "app_button.h"
#include "nrf_gpio.h"

ret_code_t error_code;		

void Config_GPIO(void) {
	nrf_gpio_cfg_input(BUTTON1, NRF_GPIO_PIN_PULLUP);
	nrf_gpio_cfg_output(LED1);

	nrf_gpio_pin_clear(LED1);
}

BOOL Read_U_b_Xbutton(void) {
	return (BOOL) !nrf_gpio_pin_read(BUTTON1);
}

BOOL Read_U_b_Yled(void){
	nrf_gpio_cfg_input(LED1, NRF_GPIO_PIN_NOPULL);
	return (BOOL) !nrf_gpio_pin_read(LED1);
}

void Write_U_b_Yled(BOOL v) {
	nrf_gpio_cfg_output(LED1);
	if (v == 0) {
		v = 1;
	} else {
		v = 0;
	}
	nrf_gpio_pin_write(LED1, v);
}