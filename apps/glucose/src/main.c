#include "sys/printk.h"
#include <zephyr.h>
#include <drivers/uart.h>
#include "monitor.h"
​
#define BUF_MAXSIZE	256
#define SLEEP_TIME	500
​
static struct device *uart1_dev;
static u8_t rx_buf[BUF_MAXSIZE];
static u8_t tx_buf[BUF_MAXSIZE];
​
static void uart1_isr(struct device *x)
{
	int len = uart_fifo_read(uart1_dev, rx_buf, BUF_MAXSIZE);
	ARG_UNUSED(x);
	// read in glucose, bolus, scheduled_basal
	char type = rx_buf[0];
	int value = 0;
	int curr;
	for (int i = 1; i < len && i < 32; i++) {
		curr = rx_buf[i];
		value = value << 1 + curr;
	}
	time_t rawtime;
	time(&rawtime);
	if (type == 'A') {
		// Read in scheduled_basal
		set_current_basal((float) value);
		printf("Set scheduled basal at %f\n", (float) value);
	} else if (type == 'B') {
		// Read in bolus
		add_treatment(rawtime, (float) value);
		printf("Set bolus to be %f\n", (float) value);
	} else {
		// Read in glucose
		printf("Calculating temp basal: %f\n", calculate_basal(rawtime, (float) value));
	}
}
​
static void uart1_init(void)
{
	uart1_dev = device_get_binding("UART_1");
​
	uart_irq_callback_set(uart1_dev, uart1_isr);
	uart_irq_rx_enable(uart1_dev);
​
	printk("uart1_init done\n");
}
​
void main(void) {
	uart1_init();
	init_monitor();
	// Move to nrf52x?
	float *tx;
	// Test setting scheduled basal
	tx_buf[0] = 0x41;
	tx = (float *) (&tx_buf[1]);
	*tx = 15f;
	uart_fifo_fill(uart1_dev, tx_buf, sizeof(u32_t));
	// Test setting bolus
	tx_buf[0] = 0x42;
	tx = (float *) (&tx_buf[1]);
	*tx = 1.85f;
	uart_fifo_fill(uart1_dev, tx_buf, sizeof(u32_t));
	// Test calculating basal
	tx_buf[0] = 0x43;
	tx = (float *) (&tx_buf[1]);
	*tx = 128f;
	uart_fifo_fill(uart1_dev, tx_buf, sizeof(u32_t));
}
