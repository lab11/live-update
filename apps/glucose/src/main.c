#include "monitor.h"
#include "data.h"
#include "sys/printk.h"
#include <zephyr.h>

void main(void) {
	init_monitor();
	float duration, rate;
	int off_zero = 0, total_num = 0;
	float percentage_diff = 0;
	for (int i = 0; i < SIZE; i++) {
		if (data[i].type == BG_Check) {
			add_reading(data[i].time, data[i].glucose);
		} else if (data[i].type == Temp_Basal) {
			calculate_basal(&duration, &rate);
			printk("Calculated duration and rate: %d.%d & %d.%d ", (int)duration, (int)(duration*1000000)%1000000, (int)rate, (int)(rate*1000000)%1000000);
			printk("Versus real duration and rate: %d.%d & %d.%d\n", (int)(data[i].duration), (int)(data[i].duration*1000000)%1000000, (int)(data[i].rate), (int)(data[i].rate*1000000)%1000000);
			if (data[i].rate == 0 && rate != 0) {
				off_zero++;
			} else if (data[i].rate != 0) {
				percentage_diff += (rate - data[i].rate < 0? data[i].rate - rate : rate - data[i].rate) / data[i].rate;
			}
			total_num++;
		} else if (data[i].type == Correction_Bolus) {
			add_treatment(data[i].time, data[i].insulin);
		} else if (data[i].type == Scheduled_Basal) {
			set_current_basal(data[i].rate);
		}
	}
	percentage_diff /= total_num;
	printk("Average percentage diff: %d percent \n", (int)(percentage_diff * 100));
	printk("Off zero: %d times out of %d \n", off_zero, total_num);
}
