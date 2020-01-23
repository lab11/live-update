//#include "sys/printk.h"
#include <stdio.h>
// USER SET
#define SCHEDULED_BASAL 1
#define TARGET_LOW 80
#define TARGET_HIGH 180
#define ISF 2 // TODO: allow auto-correction w sensitivity factor
#define MAX_BASAL_PUMP 2
#define DIA 180 // Technically 3 hours converted to 180 minutes
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
/*
float calculate_next_temp(float current_bg, float delta, float average_long_delta,
    float existing_basal, float net_iob, float bg_impact, float bolus_bg_impact) {
  float temp_basal = existing_basal;
  float expected_bg = current_bg - ISF * net_iob;
  // Advanced BG Deviation ------------
  if (ABS(delta) > ABS(average_long_delta) * 2 || current_bg > TARGET + THRESHOLD) {
    expected_bg += average_long_delta - bg_impact;
  }

  if ((current_bg > TARGET_HIGH && ABS(delta) > bg_impact * 2 && existing_basal > SCHEDULED_BASAL) ||
      (current_bg < TARGET_LOW && delta < bg_impact / 2 && existing_basal < SCHEDULED_BASAL)) {
    return MIN(CALC_MAX, existing_basal);
  }
  // ----------------------------------

  if (current_bg < TARGET_LOW ) {
    temp_basal = 0;
  } else if (delta > 0 && expected_bg < TARGET_LOW ) {
    temp_basal = SCHEDULED_BASAL;
  } else if (delta < 0 && expected_bg > TARGET_HIGH) {
    temp_basal = SCHEDULED_BASAL;
  } else if (expected_bg > TARGET_HIGH) {
    float required_temp = REQUIRED_TEMP(expected_bg);
    if (required_temp > existing_basal) { // TODO: perform check against max for bolus snooze && bolus_bg_impact + current_bg <= TARGET
      temp_basal = required_temp < 0? 0 : required_temp;
    } else if (current_bg < TARGET_LOW) {
      required_temp = REQUIRED_TEMP(expected_bg + bolus_bg_impact); // long_avg_delta?
      if (required_temp < existing_basal && expected_bg + bolus_bg_impact < TARGET_LOW) {
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
}*/

float calculate_next_temp (float bg, float current_temp_rate) {
  if (bg <= 10 || bg === 38) {
        if (current_temp_rate > SCHEDULED_BASAL) { // high temp is running
            rT.reason += ". Replacing high temp basal of "+currenttemp.rate+" with neutral temp of "+basal;
            rT.deliverAt = deliverAt;
            rT.temp = 'absolute';
            rT.duration = 30;
            rT.rate = basal;
            return rT;
        } else if ( currenttemp.rate === 0 && currenttemp.duration > 30 ) { //shorten long zero temps to 30m
            rT.reason += ". Shortening " + currenttemp.duration + "m long zero temp to 30m. ";
            rT.deliverAt = deliverAt;
            rT.temp = 'absolute';
            rT.duration = 30;
            rT.rate = 0;
            return rT;
        } else { //do nothing.
            rT.reason += ". Temp " + currenttemp.rate + " <= current basal " + basal + "U/hr; doing nothing. ";
            return rT;
        }
    }

    float target_bg = (TARGET_LOW + TARGET_HIGH) / 2.0; // if min and max are set, then set target to their average
    var sensitivityRatio;
    var high_temptarget_raises_sensitivity = profile.exercise_mode || profile.high_temptarget_raises_sensitivity;
    var normalTarget = 100; // evaluate high/low temptarget against 100, not scheduled target (which might change)
    if ( profile.half_basal_exercise_target ) {
        var halfBasalTarget = profile.half_basal_exercise_target;
    } else {
        halfBasalTarget = 160; // when temptarget is 160 mg/dL, run 50% basal (120 = 75%; 140 = 60%)
        // 80 mg/dL with low_temptarget_lowers_sensitivity would give 1.5x basal, but is limited to autosens_max (1.2x by default)
    }
    if ( high_temptarget_raises_sensitivity && profile.temptargetSet && target_bg > normalTarget
        || profile.low_temptarget_lowers_sensitivity && profile.temptargetSet && target_bg < normalTarget ) {
        // w/ target 100, temp target 110 = .89, 120 = 0.8, 140 = 0.67, 160 = .57, and 200 = .44
        // e.g.: Sensitivity ratio set to 0.8 based on temp target of 120; Adjusting basal from 1.65 to 1.35; ISF from 58.9 to 73.6
        //sensitivityRatio = 2/(2+(target_bg-normalTarget)/40);
        var c = halfBasalTarget - normalTarget;
        sensitivityRatio = c/(c+target_bg-normalTarget);
        // limit sensitivityRatio to profile.autosens_max (1.2x by default)
        sensitivityRatio = Math.min(sensitivityRatio, profile.autosens_max);
        sensitivityRatio = round(sensitivityRatio,2);
        process.stderr.write("Sensitivity ratio set to "+sensitivityRatio+" based on temp target of "+target_bg+"; ");
    } else if (typeof autosens_data !== 'undefined' && autosens_data) {
        sensitivityRatio = autosens_data.ratio;
        process.stderr.write("Autosens ratio: "+sensitivityRatio+"; ");
    }
    if (sensitivityRatio) {
        basal = profile.current_basal * sensitivityRatio;
        basal = round_basal(basal, profile);
        if (basal !== profile_current_basal) {
            process.stderr.write("Adjusting basal from "+profile_current_basal+" to "+basal+"; ");
        } else {
            process.stderr.write("Basal unchanged: "+basal+"; ");
        }
    }

    


}

float get_next_bg(int i) {
  int data [64] = {197, 10, 10, 10, 10, 10, 10, 154, 147, 145, 144, 144, 141, 132, 130, 124, 119, 110, 107, 112, 117, 125, 133, 147, 153, 150, 144, 138, 129, 123, 120, 126, 134, 135, 135, 105, 99, 104, 99, 107, 125, 149, 163, 172, 177, 179, 182, 183, 179, 177, 173, 167, 158, 154, 151, 146, 144, 138, 133, 122, 112, 103, 95, 98};
  return data[63 - i];
}

float get_next_bolus(int i) {
  return 0; // 70 - (11 - i) * i; // TODO: Replace with pin reading
}

int main(void) {
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
  for (int i = 0; i < 64; i++) {
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
    printf("Blood glucose level: %d.%d Temp basal rate: %d.%d\n", (int)current_bg, (int)(current_bg*1000000)%1000000, (int)existing_basal, (int)(existing_basal*1000000)%1000000);
    current_index = (current_index + 1) % length_bg;
    prev_bg[current_index] = current_bg;
    insulin_intake[beginning_index] = current_basal;
    bolus_intake[beginning_index] = get_next_bolus(i);
    existing_basal = current_basal;
  }
}
