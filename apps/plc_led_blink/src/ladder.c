#include "ladder.h"

void init_plc(void) {
	tfm_gpio_enable_output(LED);
	tfm_gpio_set(LED);

	printk("plc initialized\n");
}

BOOL Read_U_b_Xbutton(void) {
	return (BOOL) tfm_gpio_read(BUTTON);
}

BOOL Read_U_b_Yled(void) {
	return (BOOL) tfm_gpio_read(LED);
}

void Write_U_b_Yled(BOOL v) {
	if (v == 0) {
		tfm_gpio_clear(LED);
	} else {
		tfm_gpio_set(LED);
	}
}