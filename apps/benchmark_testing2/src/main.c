#include <string.h>
#include <kernel.h>
#include "drivers/gpio.h"
#include "sys/printk.h"

#define NUM_CYCLES 100

#define PIN1 5
#define PIN2 6


struct device *gpio_dev;
struct k_timer test_timer;

uint8_t sz8bit1 = 0xF;
uint8_t sz8bit0 = 0;
uint16_t sz16bit1 = 0xFF;
uint16_t sz16bit0 = 0;
uint32_t sz32bit1 = 0xFFFF;
uint32_t sz32bit0 = 0;

uint32_t sz128bit1[4] = {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF};
uint32_t sz128bit0[4] = {0, 0, 0, 0};

uint32_t sz512bit1[16] = {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
						0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF};
uint32_t sz512bit0[16] = {0, 0, 0, 0, 0, 0, 0, 0, 
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

uint32_t up_bnd[14] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int ret;

uint32_t get_diff(uint32_t c1, uint32_t c2) {
	return c1 > c2 ? c2 - (~c1 + 1) : c2 - c1;
}

void test_timer_handler(struct k_timer *t) {
	printk("Timer Fired!!\n");
	// k_timer_stop(&test_timer);
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
	*DEMCR = *DEMCR | 0x01000000;
	*LAR = 0xC5ACCE55;
	*DWT_CYCCNT = 0;
	*DWT_CONTROL = *DWT_CONTROL | 1;

	for (int i = 0; i < NUM_CYCLES; i++) {

		printk("\n\nTEST %d\n\n", i);

		/********************** memcpy **********************/
		
		uint32_t sz2048[64];


		// memcpy(sz2048, sz2048bit0, 256);

		count1 = *DWT_CYCCNT;
		// count1 = k_cycle_get_32();

		memcpy(sz2048, sz2048bit1, 256);		// memcpy test

		count2 = *DWT_CYCCNT;
		// count2 = k_cycle_get_32();
		// printk("CYCCNT1: %d\nCYCCNT2: %d\n", count1, count2);
		// printk("bits1: %x\nbits0 %x\n", *sz2048bit1, *sz2048);
		// printk("memcpy test %d cycle count: %d\n", i, get_diff(count1, count2));

		up_bnd[0] = up_bnd[0] < get_diff(count1, count2) ? get_diff(count1, count2) : up_bnd[0];

		// printk("\nMEMCPY UPPER BOUND: %d\n", up_bnd[0]);

		/********************** load **********************/
		

		uint32_t val;

		count1 = *DWT_CYCCNT;

		val = *LAR;								// load test

		count2 = *DWT_CYCCNT;

		// printk("CYCCNT1: %d\nCYCCNT2: %d\n", count1, count2);
		// printk("load test %d cycle count: %d\n", i, get_diff(count1, count2));

		up_bnd[1] = up_bnd[1] < get_diff(count1, count2) ? get_diff(count1, count2) : up_bnd[1];

		// printk("\nLOAD UPPER BOUND: %d\n", up_bnd[1]);

		/********************* write *********************/
		

		// uint32_t val;

		count1 = *DWT_CYCCNT;

		val = 0xFFFF;							// write test

		count2 = *DWT_CYCCNT;

		// printk("CYCCNT1: %d\nCYCCNT2: %d\n", count1, count2);
		// printk("write test %d cycle count: %d\n", i, get_diff(count1, count2));

		up_bnd[2] = up_bnd[2] < get_diff(count1, count2) ? get_diff(count1, count2) : up_bnd[2];

		// printk("\nWRITE UPPER BOUND: %d\n", up_bnd[2]);

		/********************* device_get_binding *********************/
		

		// gpio_dev = 0;
		count1 = *DWT_CYCCNT;

		gpio_dev = device_get_binding("GPIO_0"); // device_get_binding test

		count2 = *DWT_CYCCNT;

		// printk("CYCCNT1: %d\nCYCCNT2: %d\n", count1, count2);
		// printk("device_get_binding test %d cycle count: %d\n", i, get_diff(count1, count2));

		up_bnd[3] = up_bnd[3] < get_diff(count1, count2) ? get_diff(count1, count2) : up_bnd[3];

		// printk("\nDEVICE_GET_BINDING UPPER BOUND: %d\n", up_bnd[3]);

		/******************** k_timer_init ********************/
		

		count1 = *DWT_CYCCNT;

		k_timer_init(&test_timer, test_timer_handler, NULL);

		count2 = *DWT_CYCCNT;

		// printk("CYCCNT1: %d\nCYCCNT2: %d\n", count1, count2);
		// printk("k_timer_init test %d cycle count: %d\n", i, get_diff(count1, count2));

		up_bnd[4] = up_bnd[4] < get_diff(count1, count2) ? get_diff(count1, count2) : up_bnd[4];

		// printk("\nK_TIMER_INIT UPPER BOUND: %d\n", up_bnd[4]);

		/******************** k_timer_start ********************/
		

		count1 = *DWT_CYCCNT;

		// k_timer_start(&test_timer, K_SECONDS(10), K_SECONDS(0));

		count2 = *DWT_CYCCNT;

		// printk("CYCCNT1: %d\nCYCCNT2: %d\n", count1, count2);
		// printk("k_timer_start test %d cycle count: %d\n", i, get_diff(count1, count2));

		up_bnd[5] = up_bnd[5] < get_diff(count1, count2) ? get_diff(count1, count2) : up_bnd[5];

		// printk("\nK_TIMER_START UPPER BOUND: %d\n", up_bnd[5]);


		/********************* gpio_pin_config *********************/
		

		count1 = *DWT_CYCCNT;

		ret = gpio_pin_configure(gpio_dev, PIN2, GPIO_INPUT);

		count2 = *DWT_CYCCNT;

		// printk("CYCCNT1: %d\nCYCCNT2: %d\n", count1, count2);
		// printk("gpio_pin_config test %d cycle count: %d\n", i, get_diff(count1, count2));

		up_bnd[6] = up_bnd[6] < get_diff(count1, count2) ? get_diff(count1, count2) : up_bnd[6];

		// printk("\nGPIO_PIN_CONFIGURE UPPER BOUND: %d\n", up_bnd[6]);

		ret = gpio_pin_configure(gpio_dev, PIN1, GPIO_OUTPUT);

		/********************* gpio_pin_interrupt_config *********************/
		

		count1 = *DWT_CYCCNT;

		ret = gpio_pin_interrupt_configure(gpio_dev, PIN1, GPIO_INT_EDGE_TO_ACTIVE);

		count2 = *DWT_CYCCNT;

		// printk("CYCCNT1: %d\nCYCCNT2: %d\n", count1, count2);
		// printk("gpio_pin_interrupt_config test %d cycle count: %d\n", i, get_diff(count1, count2));

		up_bnd[7] = up_bnd[7] < get_diff(count1, count2) ? get_diff(count1, count2) : up_bnd[7];

		// printk("\nGPIO_PIN_INTERRUPT_CONFIGURE UPPER BOUND: %d\n", up_bnd[7]);

		/********************* gpio_port_get_raw *********************/
		

		count1 = *DWT_CYCCNT;

		ret = gpio_port_get_raw(gpio_dev, &port0);

		count2 = *DWT_CYCCNT;

		// printk("CYCCNT1: %d\nCYCCNT2: %d\n", count1, count2);
		// printk("gpio_port_get_raw test %d cycle count: %d\n", i, get_diff(count1, count2));

		up_bnd[8] = up_bnd[8] < get_diff(count1, count2) ? get_diff(count1, count2) : up_bnd[8];

		// printk("\nGPIO_PIN_INTERRUPT_CONFIGURE UPPER BOUND: %d\n", up_bnd[8]);


		/********************* gpio_port_set_masked_raw *********************/
		

		port0_out ^= 1 << PIN2;

		count1 = *DWT_CYCCNT;

		ret = gpio_port_set_masked_raw(gpio_dev, out_mask, GPIO_INT_EDGE_TO_ACTIVE);

		count2 = *DWT_CYCCNT;

		// printk("CYCCNT1: %d\nCYCCNT2: %d\n", count1, count2);
		// printk("gpio_port_set_masked_raw test %d cycle count: %d\n", i, get_diff(count1, count2));

		up_bnd[9] = up_bnd[9] < get_diff(count1, count2) ? get_diff(count1, count2) : up_bnd[9];

		// printk("\nGPIO_PORT_SET_MASKED_RAW UPPER BOUND: %d\n", up_bnd[9]);


		/********************* gpio_pin_read *********************/
		

		count1 = *DWT_CYCCNT;

		ret = gpio_pin_read(gpio_dev, PIN1, &read_val);

		count2 = *DWT_CYCCNT;

		// printk("CYCCNT1: %d\nCYCCNT2: %d\n", count1, count2);
		// printk("gpio_pin_config test %d cycle count: %d\n", i, get_diff(count1, count2));

		up_bnd[10] = up_bnd[10] < get_diff(count1, count2) ? get_diff(count1, count2) : up_bnd[10];

		// printk("\nGPIO_PIN_READ UPPER BOUND: %d\n", up_bnd[10]);

		/********************* gpio_pin_write *********************/
		

		count1 = *DWT_CYCCNT;

		ret = gpio_pin_write(gpio_dev, PIN2, i % 2);

		count2 = *DWT_CYCCNT;

		// printk("CYCCNT1: %d\nCYCCNT2: %d\n", count1, count2);
		// printk("gpio_pin_config test %d cycle count: %d\n", i, get_diff(count1, count2));

		up_bnd[11] = up_bnd[11] < get_diff(count1, count2) ? get_diff(count1, count2) : up_bnd[11];

		// printk("\nGPIO_PIN_WRITE UPPER BOUND: %d\n", up_bnd[11]);


		/********************* gpio_init_callback *********************/
		

		count1 = *DWT_CYCCNT;

		gpio_init_callback(&gpio_cb, test_gpio_handler, in_mask);

		count2 = *DWT_CYCCNT;

		// printk("CYCCNT1: %d\nCYCCNT2: %d\n", count1, count2);
		// printk("gpio_pin_config test %d cycle count: %d\n", i, get_diff(count1, count2));

		up_bnd[12] = up_bnd[12] < get_diff(count1, count2) ? get_diff(count1, count2) : up_bnd[12];

		// printk("\nGPIO_INIT_CALLBACK UPPER BOUND: %d\n", up_bnd[12]);


		/********************* gpio_add_callback *********************/
		

		count1 = *DWT_CYCCNT;

		ret = gpio_add_callback(gpio_dev, &gpio_cb);

		count2 = *DWT_CYCCNT;

		// printk("CYCCNT1: %d\nCYCCNT2: %d\n", count1, count2);
		// printk("gpio_pin_config test %d cycle count: %d\n", i, get_diff(count1, count2));

		up_bnd[13] = up_bnd[13] < get_diff(count1, count2) ? get_diff(count1, count2) : up_bnd[13];

		// printk("\nGPIO_ADD_CALLBACK UPPER BOUND: %d\n", up_bnd[13]);

	}

	printk("\nMEMCPY UPPER BOUND: %d\n", up_bnd[0]);
	printk("\nLOAD UPPER BOUND: %d\n", up_bnd[1]);
	printk("\nWRITE UPPER BOUND: %d\n", up_bnd[2]);
	printk("\nDEVICE_GET_BINDING UPPER BOUND: %d\n", up_bnd[3]);
	printk("\nK_TIMER_INIT UPPER BOUND: %d\n", up_bnd[4]);
	printk("\nK_TIMER_START UPPER BOUND: %d\n", up_bnd[5]);
	printk("\nGPIO_PIN_CONFIGURE UPPER BOUND: %d\n", up_bnd[6]);
	printk("\nGPIO_PIN_INTERRUPT_CONFIGURE UPPER BOUND: %d\n", up_bnd[7]);
	printk("\nGPIO_PIN_INTERRUPT_CONFIGURE UPPER BOUND: %d\n", up_bnd[8]);
	printk("\nGPIO_PORT_SET_MASKED_RAW UPPER BOUND: %d\n", up_bnd[9]);
	printk("\nGPIO_PIN_READ UPPER BOUND: %d\n", up_bnd[10]);
	printk("\nGPIO_PIN_WRITE UPPER BOUND: %d\n", up_bnd[11]);
	printk("\nGPIO_INIT_CALLBACK UPPER BOUND: %d\n", up_bnd[12]);
	printk("\nGPIO_ADD_CALLBACK UPPER BOUND: %d\n", up_bnd[13]);
}