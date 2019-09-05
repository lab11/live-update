// TODO: Divide by 10 for all values with comment of divide10 when floating point works
//#include "interface.h"
#include <string.h>
#include <stdio.h>

// USER SET
#define SCHEDULED_BASAL 10 // divide10 && daily scheduled rate?
#define TARGET 400 // divide10
#define THRESHOLD 100 // divide10
#define ISF 2 // TODO: allow auto-correction w sensitivity factor
#define MAX_BASAL_PUMP 20
#define DIA 240 // Technically 4 hours converted to 240 minutes

// DEFAULTS
#define DEFAULT_DIA 180 // Technically 3 hours converted to 180 minutes
#define MINUTES_TO_PEAK_FOR_DEFAULT_DIA 75
#define EXPECTED_MINUTES_PER_APPLICATION 30
#define MINUTES_PER_APPLICATION 5
#define MINUTES_FOR_LONG_DELTA 15

// MACROS
#define MIN(a, b) ((a)<(b)? (a) : (b))
#define ABS(a) ((a)<0? -1 * (a) : (a))
#define SQUARE(a) ((a) * (a))
#define REQUIRED_TEMP(bg) (SCHEDULED_BASAL + (bg - TARGET) / (ISF * EXPECTED_MINUTES_PER_APPLICATION))

int PEAK = MINUTES_TO_PEAK_FOR_DEFAULT_DIA * DIA / DEFAULT_DIA;
int CALC_MAX = MIN(MAX_BASAL_PUMP, 3 * SCHEDULED_BASAL);

int calculate_next_temp(int current_bg, int delta, int average_long_delta,
    int existing_basal, int expected_bg, int bg_impact) {
  int temp_basal = existing_basal;

  if (ABS(delta) > ABS(average_long_delta) * 2 || current_bg > TARGET + THRESHOLD) {
    expected_bg += average_long_delta - bg_impact;
  }

  if ((current_bg > TARGET + THRESHOLD && ABS(delta) > bg_impact / 2 && existing_basal > SCHEDULED_BASAL) ||
      (current_bg < TARGET - THRESHOLD && delta < bg_impact / 2 && existing_basal < SCHEDULED_BASAL)) {
    return MIN(CALC_MAX, existing_basal);
  }

  if (current_bg < TARGET - THRESHOLD) {
    temp_basal = 0;
  } else if (delta > 0 && expected_bg < TARGET) {
    temp_basal = SCHEDULED_BASAL;
  } else if (delta < 0 && expected_bg > TARGET) {
    temp_basal = SCHEDULED_BASAL;
  } else if (expected_bg > TARGET) {
    int required_temp = REQUIRED_TEMP(expected_bg);
    if (required_temp > existing_basal) {
      temp_basal = required_temp < 0? 0 : required_temp;
    } else if (current_bg < TARGET) {
      required_temp = REQUIRED_TEMP(current_bg + delta); // long_avg_delta?
      if (required_temp < existing_basal) {
        temp_basal = required_temp < 0? 0 : required_temp; // if >30m @ 0 required, extend zero temp to 30m
      }
    }
  }
  temp_basal = MIN(4 * existing_basal, temp_basal);
  return MIN(CALC_MAX, temp_basal);
}

int calculate_expected_bg(int insulin_intake[], int length, int beginning_index, int current_bg) {
  int minutes, insulin, net_iob = 0;
  for (int i = 1; i < length; i++) {
    insulin = insulin_intake[(beginning_index + i) % length];
    for (int j = 0; j < MINUTES_PER_APPLICATION; j++) {
      minutes = i * MINUTES_PER_APPLICATION - j;
      if (minutes > PEAK) {
        net_iob += (SQUARE(DIA - minutes) * insulin) / ((DIA - PEAK) * DIA);
      } else {
        net_iob += insulin - (SQUARE(minutes) * insulin) / (PEAK * DIA);
      }
    }
  }
  return current_bg - ISF * net_iob;
}

int calculate_bg_impact(int insulin_intake[], int length, int beginning_index) {
  int minutes, insulin, activity = 0;
  for (int i = 1; i < length; i++) {
    insulin = insulin_intake[(beginning_index + i) % length];
    for (int j = 0; j < MINUTES_PER_APPLICATION; j++) {
      minutes = i * MINUTES_PER_APPLICATION - j;
      if (minutes > PEAK) {
        activity += insulin * (2 * MINUTES_PER_APPLICATION * (DIA - minutes) - SQUARE(MINUTES_PER_APPLICATION)) / SQUARE(DIA);
      } else if (minutes + MINUTES_PER_APPLICATION <= PEAK) {
        activity += insulin * (2 * MINUTES_PER_APPLICATION * minutes + SQUARE(MINUTES_PER_APPLICATION)) / SQUARE(DIA);
      } else {
        activity += insulin - insulin * (SQUARE(minutes) + SQUARE(DIA - minutes - MINUTES_PER_APPLICATION)) / SQUARE(DIA);
      }
    }
  }
  return activity / MINUTES_PER_APPLICATION * ISF;
}

int get_next_bg(int i) {
  return (10 - i) * i * 10; // TODO: Replace with pin reading
}

void main(void) {
  int length = DIA / MINUTES_PER_APPLICATION, length_bg = MINUTES_FOR_LONG_DELTA / MINUTES_PER_APPLICATION;
  int insulin_intake[length], prev_bg[length_bg];
  int beginning_index = length - 1, existing_basal = SCHEDULED_BASAL, current_bg;
  for (int i = 0; i < length; i++) {
    insulin_intake[i] = 0;
    if (i < length_bg) {
      prev_bg[i] = TARGET;
    }
  }
  // TODO: Replace with timer functions
  for (int i = 1; i < 11; i++) {
    current_bg = get_next_bg(i);
    existing_basal = calculate_next_temp(current_bg,
      current_bg - prev_bg[length_bg - 1],
      (current_bg - prev_bg[0]) / length_bg,
      existing_basal,
      calculate_expected_bg(insulin_intake, length, beginning_index, current_bg),
      calculate_bg_impact(insulin_intake, length, beginning_index));
    printf("Blood glucose level: %d Temp basal rate: %d", current_bg, existing_basal);

    for (int j = 1; j < length_bg; j++) {
      prev_bg[j - 1] = prev_bg[j];
    }
    prev_bg[length_bg - 1] = current_bg;

    beginning_index = beginning_index == 0? length - 1 : beginning_index - 1;
    insulin_intake[beginning_index] = existing_basal;
  }
}
