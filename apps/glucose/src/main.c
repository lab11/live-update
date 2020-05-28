#include <stdio.h>
#include "monitor.h"
#include "data.h"
#include "sys/printk.h"
#include <zephyr.h>

struct k_timer timer;
int index = 0;

void handle_call(int i) {
  if (data[i].type == BG_Check) {
    add_reading(data[i].time, data[i].glucose);
		printk("Add reading");
  } else if (data[i].type == Temp_Basal) {
    float duration, rate;
    calculate_basal(&duration, &rate);
    printk("Calculated duration and rate: %d.%d & %d.%d ", (int)duration, (int)(duration*1000)%1000, (int)rate, (int)(rate*1000)%1000);
    printk("Versus real duration and rate: %d.%d & %d.%d\n", (int)(data[i].duration), (int)(data[i].duration*1000)%1000, (int)(data[i].rate), (int)(data[i].rate*1000)%1000);
  } else if (data[i].type == Correction_Bolus) {
    add_treatment(data[i].time, data[i].insulin);
		printk("Add treatment");
  }
}

void timer_handler(struct k_timer *t) {
  if (index < SIZE) {
    handle_call(index);
    index++;
  }
}

int main(void) {
	init_monitor();
  k_timer_init(&timer, timer_handler, NULL);
  k_timer_start(&timer, K_SECONDS(1), K_SECONDS(1)); // data[index].time - data[index - 1].time
	return 0;
}
