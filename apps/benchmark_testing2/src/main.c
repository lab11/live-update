#include <string.h>
#include <kernel.h>
#include "drivers/gpio.h"
#include "sys/printk.h"

#define NUM_CYCLES 1000

#define PIN1 5
#define PIN2 6


struct device *gpio_dev;

struct k_timer test_timer1;
struct k_timer test_timer2;
struct k_timer test_timer3;
struct k_timer test_timer4;
struct k_timer test_timer5;


uint8_t sz8bit1 = 0xF;
uint8_t sz8bit0 = 0;

uint16_t sz16bit1 = 0xFF;
uint16_t sz16bit0 = 0;

uint32_t sz32bit1 = 0xFFFF;
uint32_t sz32bit0 = 0;

uint32_t sz64bit1[2] = {0xFFFF, 0xFFFF};
uint32_t sz64bit0[2] = {0, 0};

uint32_t sz128bit1[4] = {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF};
uint32_t sz128bit0[4] = {0, 0, 0, 0};

uint32_t sz256bit1[8] = {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF};
uint32_t sz256bit0[8] = {0, 0, 0, 0, 0, 0, 0};

uint32_t sz512bit1[16] = {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
						0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF};
uint32_t sz512bit0[16] = {0, 0, 0, 0, 0, 0, 0, 0, 
						0, 0, 0, 0, 0, 0, 0, 0};

uint32_t sz1024bit1[32] = {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
						0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
						0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
						0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF};
uint32_t sz1024bit0[32] = {0, 0, 0, 0, 0, 0, 0, 0, 
						0, 0, 0, 0, 0, 0, 0, 0,
						0, 0, 0, 0, 0, 0, 0, 0, 
						0, 0, 0, 0, 0, 0, 0, 0};

uint32_t sz2048bit1[64] = {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
						0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
						0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
						0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
						0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
						0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
						0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
						0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF};
uint32_t sz2048bit0[64] = {0, 0, 0, 0, 0, 0, 0, 0, 
						0, 0, 0, 0, 0, 0, 0, 0,
						0, 0, 0, 0, 0, 0, 0, 0, 
						0, 0, 0, 0, 0, 0, 0, 0,
						0, 0, 0, 0, 0, 0, 0, 0, 
						0, 0, 0, 0, 0, 0, 0, 0,
						0, 0, 0, 0, 0, 0, 0, 0, 
						0, 0, 0, 0, 0, 0, 0, 0};

volatile uint32_t *DWT_CONTROL = (uint32_t *)0xE0001000;
volatile uint32_t *DWT_CYCCNT = (uint32_t *)0xE0001004;
volatile uint32_t *DEMCR = (uint32_t *)0xE000EDFC;
volatile uint32_t *LAR = (uint32_t *)0xE0001FB0;

uint32_t in_mask = 1 << PIN1;
uint32_t port0;
uint32_t out_mask = 1 << PIN2;
uint32_t port0_out = 0;

uint32_t read_val;

uint32_t count1;
uint32_t count2;

// uint32_t up_bnd[14] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int ret;

typedef struct {
	uint8_t memcpy1B;
	uint16_t memcpy2B;
	uint32_t memcpy4B;
	uint32_t memcpy8B;
	uint32_t memcpy16B;
	uint32_t memcpy32B;
	uint32_t memcpy64B;
	uint32_t memcpy128B;
	uint32_t memcpy256B;
	uint32_t load;
	uint32_t write;
	uint32_t dev_get_binding;
	uint32_t ktimer_init;
	uint32_t ktimer_start;
	uint32_t pin_cfg;
	uint32_t pin_int_cfg;
	uint32_t port_get_raw;
	uint32_t port_set_masked_raw;
	uint32_t pin_read;
	uint32_t pin_write;
	uint32_t init_cb;
	uint32_t add_cb;
} bounds_t;

#define INIT_UP_BOUNDS			\
{								\
	.memcpy1B = 0,				\
	.memcpy2B = 0,				\
	.memcpy4B = 0,				\
	.memcpy8B = 0,				\
	.memcpy16B = 0,				\
	.memcpy32B = 0,				\
	.memcpy64B = 0,				\
	.memcpy128B = 0,			\
	.memcpy256B = 0,			\
	.load = 0,					\
	.write = 0,					\
	.dev_get_binding = 0,		\
	.ktimer_init = 0,			\
	.ktimer_start = 0,			\
	.pin_cfg = 0,				\
	.pin_int_cfg = 0,			\
	.port_get_raw = 0,			\
	.port_set_masked_raw = 0,	\
	.pin_read = 0,				\
	.pin_write = 0,				\
	.init_cb = 0,				\
	.add_cb = 0,				\
}

uint32_t get_diff(uint32_t c1, uint32_t c2) {
	return c1 > c2 ? c2 - (~c1 + 1) : c2 - c1;
}

void test_timer_handler(struct k_timer *t) {
	printk("Timer1 Fired!!\n");
	// k_timer_stop(&test_timer1);
	return;
}

void test_timer1_handler(struct k_timer *t) {
	printk("Timer1 Fired!!\n");
	// k_timer_stop(&test_timer1);
	return;
}

void test_timer2_handler(struct k_timer *t) {
	printk("Timer2 Fired!!\n");
	// k_timer_stop(&test_timer1);
	return;
}

void test_timer3_handler(struct k_timer *t) {
	printk("Timer3 Fired!!\n");
	// k_timer_stop(&test_timer1);
	return;
}

void test_timer4_handler(struct k_timer *t) {
	printk("Timer4 Fired!!\n");
	// k_timer_stop(&test_timer1);
	return;
}

void test_timer5_handler(struct k_timer *t) {
	printk("Timer5 Fired!!\n");
	// k_timer_stop(&test_timer1);
	return;
}

struct gpio_callback gpio_cb;

void test_gpio_handler(struct device *port, 
										struct gpio_callback *cb, 
										uint32_t pins) {
	printk("GPIO PIN1 Fired!!\n");
	return;
}

void main(void) {
	bounds_t up_bnd = (bounds_t) INIT_UP_BOUNDS;

	*DEMCR = *DEMCR | 0x01000000;
	*LAR = 0xC5ACCE55;
	*DWT_CYCCNT = 0;
	*DWT_CONTROL = *DWT_CONTROL | 1;

	for (int i = 0; i < NUM_CYCLES; i++) {

		// printk("\n\nTEST %d\n\n", i);

		/********************** memcpy **********************/

		uint8_t sz8;


		memcpy(&sz8, &sz8bit0, 1);	// zero copy destination

		count1 = *DWT_CYCCNT;

		memcpy(&sz8, &sz8bit1, 1);		// memcpy test

		count2 = *DWT_CYCCNT;

		// printk("CYCCNT1: %d\nCYCCNT2: %d\n", count1, count2);
		// printk("memcpy test %d cycle count: %d\n", i, get_diff(count1, count2));

		up_bnd.memcpy1B = up_bnd.memcpy1B < get_diff(count1, count2) ? get_diff(count1, count2) : up_bnd.memcpy1B;

		// printk("\nMEMCPY1B UPPER BOUND: %d\n", up_bnd.memcpy1B);


		uint16_t sz16;


		memcpy(&sz16, &sz16bit0, 2);	// zero copy destination

		count1 = *DWT_CYCCNT;

		memcpy(&sz16, &sz16bit1, 2);		// memcpy test

		count2 = *DWT_CYCCNT;

		// printk("CYCCNT1: %d\nCYCCNT2: %d\n", count1, count2);
		// printk("memcpy test %d cycle count: %d\n", i, get_diff(count1, count2));

		up_bnd.memcpy2B = up_bnd.memcpy2B < get_diff(count1, count2) ? get_diff(count1, count2) : up_bnd.memcpy2B;

		// printk("\nMEMCPY2B UPPER BOUND: %d\n", up_bnd.memcpy2B);


		uint32_t sz32;


		memcpy(&sz32, &sz32bit0, 4);	// zero copy destination

		count1 = *DWT_CYCCNT;

		memcpy(&sz32, &sz32bit1, 4);		// memcpy test

		count2 = *DWT_CYCCNT;

		// printk("CYCCNT1: %d\nCYCCNT2: %d\n", count1, count2);
		// printk("memcpy test %d cycle count: %d\n", i, get_diff(count1, count2));

		up_bnd.memcpy4B = up_bnd.memcpy4B < get_diff(count1, count2) ? get_diff(count1, count2) : up_bnd.memcpy4B;

		// printk("\nMEMCPY4B UPPER BOUND: %d\n", up_bnd.memcpy4B);


		uint32_t sz64[2];


		memcpy(sz64, sz64bit0, 8);	// zero copy destination

		count1 = *DWT_CYCCNT;

		memcpy(sz64, sz64bit1, 8);		// memcpy test

		count2 = *DWT_CYCCNT;

		// printk("CYCCNT1: %d\nCYCCNT2: %d\n", count1, count2);
		// printk("memcpy test %d cycle count: %d\n", i, get_diff(count1, count2));

		up_bnd.memcpy8B = up_bnd.memcpy8B < get_diff(count1, count2) ? get_diff(count1, count2) : up_bnd.memcpy8B;

		// printk("\nMEMCPY8B UPPER BOUND: %d\n", up_bnd.memcpy8B);


		uint32_t sz128[4];


		memcpy(sz128, sz128bit0, 16);	// zero copy destination

		count1 = *DWT_CYCCNT;

		memcpy(sz128, sz128bit1, 16);		// memcpy test

		count2 = *DWT_CYCCNT;

		// printk("CYCCNT1: %d\nCYCCNT2: %d\n", count1, count2);
		// printk("memcpy test %d cycle count: %d\n", i, get_diff(count1, count2));

		up_bnd.memcpy16B = up_bnd.memcpy16B < get_diff(count1, count2) ? get_diff(count1, count2) : up_bnd.memcpy16B;

		// printk("\nMEMCPY16B UPPER BOUND: %d\n", up_bnd.memcpy16B);


		uint32_t sz256[8];


		memcpy(sz256, sz256bit0, 32);	// zero copy destination

		count1 = *DWT_CYCCNT;

		memcpy(sz256, sz256bit1, 32);		// memcpy test

		count2 = *DWT_CYCCNT;

		// printk("CYCCNT1: %d\nCYCCNT2: %d\n", count1, count2);
		// printk("memcpy test %d cycle count: %d\n", i, get_diff(count1, count2));

		up_bnd.memcpy32B = up_bnd.memcpy32B < get_diff(count1, count2) ? get_diff(count1, count2) : up_bnd.memcpy32B;

		// printk("\nMEMCPY32B UPPER BOUND: %d\n", up_bnd.memcpy32B);

		
		uint32_t sz512[16];


		memcpy(sz512, sz512bit0, 64);	// zero copy destination

		count1 = *DWT_CYCCNT;

		memcpy(sz512, sz512bit1, 64);		// memcpy test

		count2 = *DWT_CYCCNT;

		// printk("CYCCNT1: %d\nCYCCNT2: %d\n", count1, count2);
		// printk("memcpy test %d cycle count: %d\n", i, get_diff(count1, count2));

		up_bnd.memcpy64B = up_bnd.memcpy64B < get_diff(count1, count2) ? get_diff(count1, count2) : up_bnd.memcpy64B;

		// printk("\nMEMCPY64B UPPER BOUND: %d\n", up_bnd.memcpy64B);


		uint32_t sz1024[32];


		memcpy(sz1024, sz1024bit0, 128);	// zero copy destination

		count1 = *DWT_CYCCNT;

		memcpy(sz1024, sz1024bit1, 128);		// memcpy test

		count2 = *DWT_CYCCNT;

		// printk("CYCCNT1: %d\nCYCCNT2: %d\n", count1, count2);
		// printk("memcpy test %d cycle count: %d\n", i, get_diff(count1, count2));

		up_bnd.memcpy128B = up_bnd.memcpy128B < get_diff(count1, count2) ? get_diff(count1, count2) : up_bnd.memcpy128B;

		// printk("\nMEMCPY128B UPPER BOUND: %d\n", up_bnd.memcpy128B);

		
		uint32_t sz2048[64];


		memcpy(sz2048, sz2048bit0, 256);	// zero copy destination

		count1 = *DWT_CYCCNT;

		memcpy(sz2048, sz2048bit1, 256);		// memcpy test

		count2 = *DWT_CYCCNT;

		// printk("CYCCNT1: %d\nCYCCNT2: %d\n", count1, count2);
		// printk("memcpy test %d cycle count: %d\n", i, get_diff(count1, count2));

		up_bnd.memcpy256B = up_bnd.memcpy256B < get_diff(count1, count2) ? get_diff(count1, count2) : up_bnd.memcpy256B;

		// printk("\nMEMCPY256B UPPER BOUND: %d\n", up_bnd.memcpy256B);

		/********************** load **********************/
		

		uint32_t val;

		count1 = *DWT_CYCCNT;

		val = *LAR;								// load test

		count2 = *DWT_CYCCNT;

		// printk("CYCCNT1: %d\nCYCCNT2: %d\n", count1, count2);
		// printk("load test %d cycle count: %d\n", i, get_diff(count1, count2));

		up_bnd.load = up_bnd.load < get_diff(count1, count2) ? get_diff(count1, count2) : up_bnd.load;

		// printk("\nLOAD UPPER BOUND: %d\n", up_bnd.load);

		/********************* write *********************/
		

		// uint32_t val;

		count1 = *DWT_CYCCNT;

		val = 0xFFFF;							// write test

		count2 = *DWT_CYCCNT;

		// printk("CYCCNT1: %d\nCYCCNT2: %d\n", count1, count2);
		// printk("write test %d cycle count: %d\n", i, get_diff(count1, count2));

		up_bnd.write = up_bnd.write < get_diff(count1, count2) ? get_diff(count1, count2) : up_bnd.write;

		// printk("\nWRITE UPPER BOUND: %d\n", up_bnd.write);

		/********************* device_get_binding *********************/
		

		// gpio_dev = 0;
		count1 = *DWT_CYCCNT;

		gpio_dev = device_get_binding("GPIO_0"); // device_get_binding test

		count2 = *DWT_CYCCNT;

		// printk("CYCCNT1: %d\nCYCCNT2: %d\n", count1, count2);
		// printk("device_get_binding test %d cycle count: %d\n", i, get_diff(count1, count2));

		up_bnd.dev_get_binding = up_bnd.dev_get_binding < get_diff(count1, count2) ? get_diff(count1, count2) : up_bnd.dev_get_binding;

		// printk("\nDEVICE_GET_BINDING UPPER BOUND: %d\n", up_bnd.dev_get_binding);

		/******************** k_timer_init ********************/
		
		struct k_timer test_timer;

		switch(i) {
			case 0:
				count1 = *DWT_CYCCNT;
				k_timer_init(&test_timer1, test_timer1_handler, NULL);
				count2 = *DWT_CYCCNT;
				break;
			case 1:
				count1 = *DWT_CYCCNT;
				k_timer_init(&test_timer2, test_timer2_handler, NULL);
				count2 = *DWT_CYCCNT;
				break;
			case 2:
				count1 = *DWT_CYCCNT;
				k_timer_init(&test_timer3, test_timer3_handler, NULL);
				count2 = *DWT_CYCCNT;
				break;
			case 3:
				count1 = *DWT_CYCCNT;
				k_timer_init(&test_timer4, test_timer4_handler, NULL);
				count2 = *DWT_CYCCNT;
				break;
			case 4:
				count1 = *DWT_CYCCNT;
				k_timer_init(&test_timer5, test_timer5_handler, NULL);
				count2 = *DWT_CYCCNT;
				break;
			default :
				count1 = *DWT_CYCCNT;
				k_timer_init(&test_timer, test_timer_handler, NULL);
				count2 = *DWT_CYCCNT;
		}
		// printk("CYCCNT1: %d\nCYCCNT2: %d\n", count1, count2);
		// printk("k_timer_init test %d cycle count: %d\n", i, get_diff(count1, count2));

		up_bnd.ktimer_init = up_bnd.ktimer_init < get_diff(count1, count2) ? get_diff(count1, count2) : up_bnd.ktimer_init;

		// printk("\nK_TIMER_INIT UPPER BOUND: %d\n", up_bnd.ktimer_init);

		/******************** k_timer_start ********************/
		
		switch (i % 5) {
			case 0:
				count1 = *DWT_CYCCNT;
				k_timer_start(&test_timer1, K_SECONDS(1), K_SECONDS(0));
				count2 = *DWT_CYCCNT;
				break;
			case 1:
				count1 = *DWT_CYCCNT;
				k_timer_start(&test_timer2, K_SECONDS(1), K_SECONDS(0));
				count2 = *DWT_CYCCNT;
				break;
			case 2:
				count1 = *DWT_CYCCNT;
				k_timer_start(&test_timer3, K_SECONDS(1), K_SECONDS(0));
				count2 = *DWT_CYCCNT;
				break;
			case 3:
				count1 = *DWT_CYCCNT;
				k_timer_start(&test_timer4, K_SECONDS(1), K_SECONDS(0));
				count2 = *DWT_CYCCNT;
				break;
			case 4:
				count1 = *DWT_CYCCNT;
				k_timer_start(&test_timer5, K_SECONDS(1), K_SECONDS(0));
				count2 = *DWT_CYCCNT;
				break;
		}

		// printk("CYCCNT1: %d\nCYCCNT2: %d\n", count1, count2);
		// printk("k_timer_start test %d cycle count: %d\n", i, get_diff(count1, count2));

		up_bnd.ktimer_start = up_bnd.ktimer_start < get_diff(count1, count2) ? get_diff(count1, count2) : up_bnd.ktimer_start;

		// printk("\nK_TIMER_START UPPER BOUND: %d\n", up_bnd.ktimer_start);


		/********************* gpio_pin_config *********************/
		

		count1 = *DWT_CYCCNT;

		ret = gpio_pin_configure(gpio_dev, PIN2, GPIO_INPUT);

		count2 = *DWT_CYCCNT;

		// printk("CYCCNT1: %d\nCYCCNT2: %d\n", count1, count2);
		// printk("gpio_pin_config test %d cycle count: %d\n", i, get_diff(count1, count2));

		up_bnd.pin_cfg = up_bnd.pin_cfg < get_diff(count1, count2) ? get_diff(count1, count2) : up_bnd.pin_cfg;

		// printk("\nGPIO_PIN_CONFIGURE UPPER BOUND: %d\n", up_bnd.pin_cfg);

		ret = gpio_pin_configure(gpio_dev, PIN1, GPIO_OUTPUT);

		/********************* gpio_pin_interrupt_config *********************/
		

		count1 = *DWT_CYCCNT;

		ret = gpio_pin_interrupt_configure(gpio_dev, PIN1, GPIO_INT_EDGE_TO_ACTIVE);

		count2 = *DWT_CYCCNT;

		// printk("CYCCNT1: %d\nCYCCNT2: %d\n", count1, count2);
		// printk("gpio_pin_interrupt_config test %d cycle count: %d\n", i, get_diff(count1, count2));

		up_bnd.pin_int_cfg = up_bnd.pin_int_cfg < get_diff(count1, count2) ? get_diff(count1, count2) : up_bnd.pin_int_cfg;

		// printk("\nGPIO_PIN_INTERRUPT_CONFIGURE UPPER BOUND: %d\n", up_bnd.pin_int_cfg);

		/********************* gpio_port_get_raw *********************/
		

		count1 = *DWT_CYCCNT;

		ret = gpio_port_get_raw(gpio_dev, &port0);

		count2 = *DWT_CYCCNT;

		// printk("CYCCNT1: %d\nCYCCNT2: %d\n", count1, count2);
		// printk("gpio_port_get_raw test %d cycle count: %d\n", i, get_diff(count1, count2));

		up_bnd.port_get_raw = up_bnd.port_get_raw < get_diff(count1, count2) ? get_diff(count1, count2) : up_bnd.port_get_raw;

		// printk("\nGPIO_PORT_GET_RAW UPPER BOUND: %d\n", up_bnd.port_get_raw);


		/********************* gpio_port_set_masked_raw *********************/
		

		port0_out ^= 1 << PIN2;

		count1 = *DWT_CYCCNT;

		ret = gpio_port_set_masked_raw(gpio_dev, out_mask, GPIO_INT_EDGE_TO_ACTIVE);

		count2 = *DWT_CYCCNT;

		// printk("CYCCNT1: %d\nCYCCNT2: %d\n", count1, count2);
		// printk("gpio_port_set_masked_raw test %d cycle count: %d\n", i, get_diff(count1, count2));

		up_bnd.port_set_masked_raw = up_bnd.port_set_masked_raw < get_diff(count1, count2) ? get_diff(count1, count2) : up_bnd.port_set_masked_raw;

		// printk("\nGPIO_PORT_SET_MASKED_RAW UPPER BOUND: %d\n", up_bnd.port_set_masked_raw);


		/********************* gpio_pin_read *********************/
		

		count1 = *DWT_CYCCNT;

		ret = gpio_pin_read(gpio_dev, PIN1, &read_val);

		count2 = *DWT_CYCCNT;

		// printk("CYCCNT1: %d\nCYCCNT2: %d\n", count1, count2);
		// printk("gpio_pin_config test %d cycle count: %d\n", i, get_diff(count1, count2));

		up_bnd.pin_read = up_bnd.pin_read < get_diff(count1, count2) ? get_diff(count1, count2) : up_bnd.pin_read;

		// printk("\nGPIO_PIN_READ UPPER BOUND: %d\n", up_bnd.pin_read);

		/********************* gpio_pin_write *********************/
		

		count1 = *DWT_CYCCNT;

		ret = gpio_pin_write(gpio_dev, PIN2, i % 2);

		count2 = *DWT_CYCCNT;

		// printk("CYCCNT1: %d\nCYCCNT2: %d\n", count1, count2);
		// printk("gpio_pin_config test %d cycle count: %d\n", i, get_diff(count1, count2));

		up_bnd.pin_write = up_bnd.pin_write < get_diff(count1, count2) ? get_diff(count1, count2) : up_bnd.pin_write;

		// printk("\nGPIO_PIN_WRITE UPPER BOUND: %d\n", up_bnd.pin_write);


		/********************* gpio_init_callback *********************/
		

		count1 = *DWT_CYCCNT;

		gpio_init_callback(&gpio_cb, test_gpio_handler, in_mask);

		count2 = *DWT_CYCCNT;

		// printk("CYCCNT1: %d\nCYCCNT2: %d\n", count1, count2);
		// printk("gpio_pin_config test %d cycle count: %d\n", i, get_diff(count1, count2));

		up_bnd.init_cb = up_bnd.init_cb < get_diff(count1, count2) ? get_diff(count1, count2) : up_bnd.init_cb;

		// printk("\nGPIO_INIT_CALLBACK UPPER BOUND: %d\n", up_bnd.init_cb);


		/********************* gpio_add_callback *********************/
		

		count1 = *DWT_CYCCNT;

		ret = gpio_add_callback(gpio_dev, &gpio_cb);

		count2 = *DWT_CYCCNT;

		// printk("CYCCNT1: %d\nCYCCNT2: %d\n", count1, count2);
		// printk("gpio_pin_config test %d cycle count: %d\n", i, get_diff(count1, count2));

		up_bnd.add_cb = up_bnd.add_cb < get_diff(count1, count2) ? get_diff(count1, count2) : up_bnd.add_cb;

		// printk("\nGPIO_ADD_CALLBACK UPPER BOUND: %d\n", up_bnd.add_cb);

	}

	printk("MEMCPY1B UPPER BOUND: %d\n", up_bnd.memcpy1B);
	printk("MEMCPY2B UPPER BOUND: %d\n", up_bnd.memcpy2B);
	printk("MEMCPY4B UPPER BOUND: %d\n", up_bnd.memcpy4B);
	printk("MEMCPY8B UPPER BOUND: %d\n", up_bnd.memcpy8B);
	printk("MEMCPY16B UPPER BOUND: %d\n", up_bnd.memcpy16B);
	printk("MEMCPY32B UPPER BOUND: %d\n", up_bnd.memcpy32B);
	printk("MEMCPY64B UPPER BOUND: %d\n", up_bnd.memcpy64B);
	printk("MEMCPY128B UPPER BOUND: %d\n", up_bnd.memcpy128B);
	printk("MEMCPY256B UPPER BOUND: %d\n", up_bnd.memcpy256B);
	printk("LOAD UPPER BOUND: %d\n", up_bnd.load);
	printk("WRITE UPPER BOUND: %d\n", up_bnd.write);
	printk("DEVICE_GET_BINDING UPPER BOUND: %d\n", up_bnd.dev_get_binding);
	printk("K_TIMER_INIT UPPER BOUND: %d\n", up_bnd.ktimer_init);
	printk("K_TIMER_START UPPER BOUND: %d\n", up_bnd.ktimer_start);
	printk("GPIO_PIN_CONFIGURE UPPER BOUND: %d\n", up_bnd.pin_cfg);
	printk("GPIO_PIN_INTERRUPT_CONFIGURE UPPER BOUND: %d\n", up_bnd.pin_int_cfg);
	printk("GPIO_PORT_GET_RAW UPPER BOUND: %d\n", up_bnd.port_get_raw);
	printk("GPIO_PORT_SET_MASKED_RAW UPPER BOUND: %d\n", up_bnd.port_set_masked_raw);
	printk("GPIO_PIN_READ UPPER BOUND: %d\n", up_bnd.pin_read);
	printk("GPIO_PIN_WRITE UPPER BOUND: %d\n", up_bnd.pin_write);
	printk("GPIO_INIT_CALLBACK UPPER BOUND: %d\n", up_bnd.init_cb);
	printk("GPIO_ADD_CALLBACK UPPER BOUND: %d\n", up_bnd.add_cb);
}