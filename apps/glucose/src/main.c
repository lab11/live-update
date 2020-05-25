#include <stdio.h>
#include "monitor.c"
#include "data.h"
#include "sys/printk.h"
#include <zephyr.h>

int main(void) {
	init_monitor();
	float duration, rate;
	float diff = 0;
	float total_num = 0;
	for (int i = 0; i < SIZE; i++) {
		if (data[i].type == BG_Check) {
			add_reading(data[i].time, data[i].glucose);
		} else if (data[i].type == Temp_Basal) {
			calculate_basal(&duration, &rate);
			printk("Calculated duration and rate: %d.%d & %d.%d ", (int)duration, (int)(duration*1000)%1000, (int)rate, (int)(rate*1000)%1000);
			printk("Versus real duration and rate: %d.%d & %d.%d\n", (int)(data[i].duration), (int)(data[i].duration*1000)%1000, (int)(data[i].rate), (int)(data[i].rate*1000)%1000);
			diff += rate - data[i].rate > 0? rate - data[i].rate : data[i].rate - rate;
			total_num++;
		} else if (data[i].type == Correction_Bolus) {
			add_treatment(data[i].time, data[i].insulin);
		} else if (data[i].type == Scheduled_Basal) {
			set_current_basal(data[i].rate);
		}
	}
	printk("avg diff %d.%d\n", (int)(diff / total_num), (int)((diff / total_num) * 1000 % 1000));
	return 0;
}
