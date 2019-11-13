#include "sys/printk.h"

// USER SET
#define SCHEDULED_BASAL 1 // divide1000000 && daily scheduled rate?
#define TARGET 4 // divide1000000
#define THRESHOLD 1 // divide1000000
#define ISF 2 // TODO: allow auto-correction w sensitivity factor
#define MAX_BASAL_PUMP 20 //divide1000000
#define DIA 240 // Technically 4 hours converted to 240 minutes
#define MAX_IOB_DIFFERENCE 0

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

float PEAK = MINUTES_TO_PEAK_FOR_DEFAULT_DIA * DIA / DEFAULT_DIA;
float CALC_MAX = MIN(MAX_BASAL_PUMP, 3 * SCHEDULED_BASAL);

float calculate_next_temp(float current_bg, float delta, float average_long_delta,
    float existing_basal, float net_iob, float bg_impact, float bolus_bg_impact) {
  float temp_basal = existing_basal;
  float expected_bg = current_bg - ISF * net_iob;
  // Advanced BG Deviation ------------
  if (ABS(delta) > ABS(average_long_delta) * 2 || current_bg > TARGET + THRESHOLD) {
    expected_bg += average_long_delta - bg_impact;
  }

  if ((current_bg > TARGET + THRESHOLD && ABS(delta) > bg_impact * 2 && existing_basal > SCHEDULED_BASAL) ||
      (current_bg < TARGET - THRESHOLD && delta < bg_impact / 2 && existing_basal < SCHEDULED_BASAL)) {
    return MIN(CALC_MAX, existing_basal);
  }
  // ----------------------------------

  if (current_bg < TARGET - THRESHOLD ) {
    temp_basal = 0;
  } else if (delta > 0 && expected_bg < TARGET ) {
    temp_basal = SCHEDULED_BASAL;
  } else if (delta < 0 && expected_bg > TARGET) {
    temp_basal = SCHEDULED_BASAL;
  } else if (expected_bg > TARGET) {
    float required_temp = REQUIRED_TEMP(expected_bg);
    if (required_temp > existing_basal) { // TODO: perform check against max for bolus snooze && bolus_bg_impact + current_bg <= TARGET
      temp_basal = required_temp < 0? 0 : required_temp;
    } else if (current_bg < TARGET) {
      required_temp = REQUIRED_TEMP(expected_bg + bolus_bg_impact); // long_avg_delta?
      if (required_temp < existing_basal && expected_bg + bolus_bg_impact < TARGET) {
        temp_basal = required_temp < 0? 0 : required_temp; // if >30m @ 0 required, extend zero temp to 30m
      }
    }
  }
  temp_basal = MIN(4 * existing_basal, temp_basal);
  return MIN(CALC_MAX, temp_basal);
}

float calculate_iob(float insulin_intake[], int length, int beginning_index, float dia, float peak) {
  int minutes, insulin, net_iob = 0;
  for (int i = 1; i < length; i++) {
    insulin = insulin_intake[(beginning_index + i) % length];
    for (int j = 0; j < MINUTES_PER_APPLICATION; j++) {
      minutes = i * MINUTES_PER_APPLICATION - j;
      if (minutes > peak) {
        net_iob += (SQUARE(dia - minutes) * insulin) / ((dia - peak) * dia);
      } else {
        net_iob += insulin - (SQUARE(minutes) * insulin) / (peak * dia);
      }
    }
  }
  return net_iob;
}

float calculate_bg_impact(float insulin_intake[], int length, int beginning_index, float dia, float peak) {
  int minutes, insulin, activity = 0;
  for (int i = 1; i < length; i++) {
    insulin = insulin_intake[(beginning_index + i) % length];
    for (int j = 0; j < MINUTES_PER_APPLICATION; j++) {
      minutes = i * MINUTES_PER_APPLICATION - j;
      if (minutes > peak) {
        activity += insulin * (2 * MINUTES_PER_APPLICATION * (dia - minutes) - SQUARE(MINUTES_PER_APPLICATION)) / SQUARE(dia);
      } else if (minutes + MINUTES_PER_APPLICATION <= peak) {
        activity += insulin * (2 * MINUTES_PER_APPLICATION * minutes + SQUARE(MINUTES_PER_APPLICATION)) / SQUARE(dia);
      } else {
        activity += insulin - insulin * (SQUARE(minutes) + SQUARE(dia - minutes - MINUTES_PER_APPLICATION)) / SQUARE(dia);
      }
    }
  }
  return activity * ISF;
}

float get_next_bg(int i) {
  return 70 - (11 - i) * i; // TODO: Replace with pin reading
}

float get_next_bolus(int i) {
  return 70 - (11 - i) * i; // TODO: Replace with pin reading
}

void main(void) {
  int length = DIA / MINUTES_PER_APPLICATION, length_bg = MINUTES_FOR_LONG_DELTA / MINUTES_PER_APPLICATION;
  float insulin_intake[length], bolus_intake[length], prev_bg[length_bg];
  int beginning_index = length - 1, current_index = length_bg - 1;
  float current_basal, existing_basal = SCHEDULED_BASAL, current_bg;
  for (int i = 0; i < length; i++) {
    insulin_intake[i] = 0;
    bolus_intake[i] = 0;
    if (i < length_bg) {
      prev_bg[i] = TARGET;
    }
  }
  // TODO: Replace with timer functions
  for (int i = 1; i < 11; i++) {
    current_bg = get_next_bg(i);
    current_basal = calculate_next_temp(current_bg,
      current_bg - prev_bg[current_index],
      (current_bg - prev_bg[(current_index + 1) % length_bg]) / length_bg,
      existing_basal,
      calculate_iob(insulin_intake, length, beginning_index, DIA, PEAK),
      calculate_bg_impact(insulin_intake, length, beginning_index, DIA, PEAK),
      calculate_bg_impact(bolus_intake, length, beginning_index, DIA/2, PEAK/2));
    beginning_index = beginning_index == 0? length - 1 : beginning_index - 1;
    insulin_intake[beginning_index] = existing_basal;
    if (current_basal > existing_basal &&
      calculate_iob(insulin_intake, length, beginning_index, DIA, PEAK) > calculate_iob(bolus_intake, length, beginning_index, DIA/2, PEAK/2) + MAX_IOB_DIFFERENCE) {
      current_basal = existing_basal;
    }
    printk("Blood glucose level: %d.%d Temp basal rate: %d.%d\n", (int)current_bg, (int)(current_bg*1000000)%1000000, (int)existing_basal, (int)(existing_basal*1000000)%1000000);
    current_index = (current_index + 1) % length_bg;
    prev_bg[current_index] = current_bg;
    insulin_intake[beginning_index] = current_basal;
    bolus_intake[beginning_index] = get_next_bolus(i);
    existing_basal = current_basal;
  }
}
