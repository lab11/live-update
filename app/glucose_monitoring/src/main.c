// TODO: Divide by 10 for all values with comment of divide10 when floating point works
#include "interface.h"

// USER SET
#define SCHEDULED_BASAL 10 // divide10 && daily scheduled rate?
#define TARGET 400 // divide10
#define THRESHOLD 300 // divide10
#define ISF 2 // TODO: allow auto-correction w sensitivity factor
#define MAX_BASAL_PUMP 20
#define DIA 240 // Technically 4 hours converted to 240 minutes

// DEFAULTS
#define DEFAULT_DIA 180 // Technically 3 hours converted to 180 minutes
#define MINUTES_TO_PEAK_FOR_DEFAULT_DIA 75
#define MINUTES_PER_APPLICATION 30

// MACROS
#define MIN(a, b) ((a)<(b)? (a) : (b))
#define REQUIRED_TEMP(bg) (SCHEDULED_BASAL + (bg - TARGET) / (ISF * MINUTES_PER_APPLICATION))

int calculate_next_temp(int current_bg, int delta, int existing_basal, int expected_bg) {
  printk("Val1: %d", current_bg);
  printk("Val2: %d", delta);
  printk("Val3: %d", existing_basal);
  printk("Val4: %d", expected_bg);
  /*if (current_bg < TARGET - THRESHOLD) {
    return 0;
  } else if (delta > 0 && expected_bg < TARGET) {
    return SCHEDULED_BASAL;
  } else if (delta < 0 && expected_bg > TARGET) {
    return SCHEDULED_BASAL;
  } else { //if (expected_bg > TARGET)
    int required_temp = REQUIRED_TEMP(expected_bg);
    if (required_temp > existing_basal) {
      return required_temp < 0? 0 : required_temp;
    } else if (current_bg < TARGET) {
      required_temp = REQUIRED_TEMP(current_bg + delta); // long_avg_delta?
      if (required_temp < existing_basal) {
        return required_temp < 0? 0 : required_temp; // if >30m @ 0 required, extend zero temp to 30m
      }
    }
  }*/
  return existing_basal;
}
/*
int calculate_net_iob(int insulin_intake[], int length, int beginning_index) {
  int peak = MINUTES_TO_PEAK_FOR_DEFAULT_DIA * DIA / DEFAULT_DIA;
  int minutes, insulin, net_iob = 0;
  for (int i = 1; i < length; i++) {
    minutes = i * MINUTES_PER_APPLICATION;
    insulin = insulin_intake[(beginning_index + i) % length];
    if (minutes > peak) {
      net_iob += ((DIA - minutes) * (DIA - minutes) * insulin) / ((DIA - peak) * DIA);
    } else {
      net_iob += insulin - (minutes * minutes * insulin) / (peak * DIA);
    }
  }
  return net_iob;
}
*/
int get_next_bg(int i) {
  return (10 - i) * i * 10; // TODO: Replace with pin reading
}

void main(void) {
  int length = DIA / MINUTES_PER_APPLICATION;
  int insulin_intake[length];
  int beginning_index = length - 1, prev_bg = 0, current_bg = 0, existing_basal = SCHEDULED_BASAL;
  for (int i = 0; i < length; i++) {
    insulin_intake[i] = 0;
  }
  // TODO: Replace with timer functions
  for (int i = 1; i < 11; i++) {
    current_bg = get_next_bg(i);
    existing_basal = calculate_next_temp(current_bg, current_bg - prev_bg, 3, 4); //calculate_net_iob(insulin_intake, length, beginning_index), , , 100
    existing_basal = MIN(existing_basal, MAX_BASAL_PUMP);
    printk("Input: %d; Output: %d.%d\n", current_bg/10, existing_basal/10, existing_basal%10);
    prev_bg = current_bg;
    // beginning_index = beginning_index == 0? length - 1 : beginning_index - 1;
    // insulin_intake[beginning_index] = existing_basal * MINUTES_PER_APPLICATION;
  }
}
