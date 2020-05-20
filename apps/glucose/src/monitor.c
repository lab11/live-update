#include "monitor.h"
#include <stddef.h>
#include <stdlib.h>
#include "tfm_gpio_veneers.h"

/* User defined values in profile*/
#define TARGET_LOW 80
#define TARGET_HIGH 180
#define SENS 85
#define DIA 180 /* Number of minutes for dia in hours */

/* Defaults */
#define MAX_IOB 0
#define MAX_BASAL_PUMP 2
#define MAX_READINGS 200

/* MACROS */
#define MIN(a, b) ((a)<(b)? (a) : (b))

#define FLOATS_BIT 32

/* Peak activation percentage occurs at 75 minutes for DIA of 180 or 3 hours. */
float PEAK = DIA * 75.0f / 180;

struct glucose_readings {
  time_t time; /* Represents the absolute time as seconds from initialization. */
  float glucose;
};

struct treatment_list {
  time_t time; /* Represents the absolute time as seconds from initialization. */
  float minutes; /* Represents minutes between the next treatment and the current. */
  float insulin;
  struct treatment_list *prev;
};

struct requested_temp current_temp_basal;
struct requested_temp current_scheduled_basal;
struct treatment_list iob_data_absolute[MAX_READINGS];
int iob_data_size;
struct glucose_readings glucose_data_absolute[MAX_READINGS];
int glucose_size;

void calculate_deltas(float *);
void determine_basal(float, float, float, float, float, float, float);

void init_monitor(void) {
  current_scheduled_basal.duration = 0.0f;
  current_scheduled_basal.rate = 0.0f;
  current_temp_basal.duration = 0.0f;
  current_temp_basal.rate = 0.0f;

  iob_data_size = 0;
  glucose_size = 0;
}

void calculate_basal(float *duration, float *rate) {
  float glucose_deltas[3];
  calculate_deltas(glucose_deltas);
  determine_basal(glucose_data_absolute[glucose_size - 1].glucose, current_temp_basal.rate,
    current_temp_basal.duration, current_scheduled_basal.rate,
    glucose_deltas[0], glucose_deltas[1], glucose_deltas[2]);
  *duration = current_temp_basal.duration;
  *rate = current_temp_basal.rate;
}

void add_reading(time_t rawtime, float glucose) {
  // Append new glucose reading data
  glucose_data_absolute[glucose_size].time = rawtime;
  glucose_data_absolute[glucose_size].glucose = glucose;
  glucose_size++;
}

void set_current_basal(float rate) {
  current_scheduled_basal.duration = 0.0f; // NOT USED
  current_scheduled_basal.rate = rate;
}

void add_treatment(time_t rawtime, float bolus_insulin) {
  iob_data_absolute[iob_data_size].time = rawtime;
  iob_data_absolute[iob_data_size].insulin = bolus_insulin;
  iob_data_absolute[iob_data_size].prev = iob_data_absolute;
  if (iob_data_size > 0) {
    iob_data_absolute[iob_data_size].minutes = (rawtime - iob_data_absolute[iob_data_size - 1].time) / 60;
  } else {
    iob_data_absolute[iob_data_size].minutes = 0; // TODO: Check defaults
  }
  iob_data_size++;
}

void calculate_deltas(float *deltas) {
  /*
  delta = change in BG between glucose (most recent BG) and an average of BG value from between 2.5 and 7.5 minutes ago (usually just a single BG value from 5 minutes ago)
  glucose = most recent BG
  short_avgdelta = average rate of change (per 5m) in BG values between glucose (most recent BG) and each BG reading from 2.5 to 17.5 minutes ago
  long_avgdelta = average rate of change (per 5m) in BG values between glucose (most recent BG) and each BG reading from 17.5 to 42.5 minutes ago
  */
  float avg_bg_delta = 0.0f, short_avg_delta = 0.0f, long_avg_delta = 0.0f;
  int short_num = 0, long_num = 0;
  float current_difference;
  for (int index = glucose_size - 1; index >= 0; index--) {
    current_difference = glucose_data_absolute[glucose_size - 1].time - glucose_data_absolute[index].time;
    if (2.5f * 60 <= current_difference && current_difference <= 7.5f * 60) {
      avg_bg_delta += glucose_data_absolute[index].glucose;
      short_avg_delta += (glucose_data_absolute[glucose_size - 1].glucose) - (glucose_data_absolute[index].glucose);
      short_num++;
    } else if (17.5f * 60 <= current_difference && current_difference <= 42.5f * 60) {
      long_avg_delta += (glucose_data_absolute[glucose_size - 1].glucose) - (glucose_data_absolute[index].glucose);
      long_num++;
    }
  }
  deltas[0] = avg_bg_delta/short_num;
  deltas[1] = short_avg_delta/short_num;
  deltas[2] = long_avg_delta/long_num;
}

float get_activity_percentage (int minutes) {
  if (minutes <= PEAK) {
    return 2 * minutes / (DIA * PEAK);
  } else {
    return 2 * (DIA - minutes) / (DIA * (DIA - PEAK));
  }
}

/* sum for all insulin treatments in units of insulin used per minute */
float calculate_activity () {
  int total_activity = 0;
  int cummulative_minutes = 0;
  for (int i = iob_data_size - 1; i >= 0; i--) {
    if (cummulative_minutes >= DIA) { break; }
    cummulative_minutes += iob_data_absolute[i].minutes;
    total_activity += (iob_data_absolute[i].insulin) * get_activity_percentage (cummulative_minutes);
  }
  return total_activity;
}

/* sum for all insulin treatments in units of insulin remaining each minute */
float calculate_iob () {
  float total_iob = 0;
  int cummulative_minutes = 0;
  for (int i = iob_data_size - 1; i >= 0; i--) {  //(current_index != (starting_treatment - 1 + MAX_READINGS) % MAX_READINGS && cummulative_minutes < DIA) {
    cummulative_minutes += iob_data_absolute[i].minutes;
    if (cummulative_minutes >= DIA) { break; }
    else if (cummulative_minutes <= PEAK) {
      total_iob += (iob_data_absolute[i].insulin)
        * get_activity_percentage (cummulative_minutes)
        * cummulative_minutes * 0.5f;
    } else {
      total_iob += (iob_data_absolute[i].insulin)
        * (DIA - get_activity_percentage (cummulative_minutes)
        * (DIA - cummulative_minutes)) * 0.5f;
    }
  }
  return total_iob;
}

// All constants according to oref design
void determine_basal(float blood_glucose,
  float current_basal, float current_duration, float scheduled_basal,
  float delta, float short_avgdelta, float long_avgdelta) {
    if (blood_glucose <= 10 || blood_glucose == 38 ) {
        if (current_basal > scheduled_basal || (current_basal == 0 && current_duration > 30) ) {
          // high temp is running or shorten long zero temps to 30m
            current_temp_basal.duration = 30;
            current_temp_basal.rate = 0;
            return;
        } else { //do nothing.
          return;
        }
    }

    // if target_high and target_low are set, then set target to their average
    float target_bg = (TARGET_LOW + TARGET_HIGH) / 2.0f;
    float minDelta = MIN (delta, short_avgdelta);
    float minAvgDelta = MIN (short_avgdelta , long_avgdelta);
    //float maxDelta = long_avgdelta > short_avgdelta + delta - minDelta ? long_avgdelta : short_avgdelta + delta - minDelta;

    // TODO: compare currenttemp to lastTemp and cancel temp if they don't match

    //calculate BG impact: the amount BG "should" be rising or falling based on insulin activity alone
    float activity = calculate_activity ();
    float bgi = - activity * SENS * 5;
    // project deviations for 30 minutes
    float deviation = 30 / 5 * ( minDelta - bgi ) ;
    // don't overreact to a big negative delta: use minAvgDelta if deviation is negative
    if (deviation < 0) {
        deviation = (30 / 5) * ( minAvgDelta - bgi ) ;
        // and if deviation is still negative, use long_avgdelta
        if (deviation < 0) {
            deviation = (30 / 5) * ( long_avgdelta - bgi ) ;
        }
    }

    float iob = calculate_iob ();
    // calculate the naive (bolus calculator math) eventual BG based on net IOB and sensitivity
    float naive_eventual_bg = blood_glucose - iob * SENS;
    // and adjust it for the deviation above
    float eventual_bg = naive_eventual_bg + deviation;

    // TODO: raise target for noisy / raw CGM data

    float expectedDelta = bgi + (target_bg - eventual_bg) * 5 / 120;

    // min_bg of 90 -> threshold of 65, 100 -> 70 110 -> 75, and 130 -> 85
    float threshold = TARGET_LOW - 0.5f * (TARGET_LOW - 40);

    // TODO: add carb impact

    if ( blood_glucose < threshold) {
      current_temp_basal.duration = 0;
      current_temp_basal.rate = 0;
      return;
    }

    if (eventual_bg < TARGET_LOW) { // if eventual BG is below target:
        // if 5m or 30m avg BG is rising faster than expected delta
        if ( minDelta > expectedDelta && minDelta > 0 ) {
            // if naive_eventual_bg < 40, set a 30m zero temp (oref0-pump-loop will let any longer SMB zero temp run)
            if (naive_eventual_bg < 40) {
              current_temp_basal.duration = 30;
              current_temp_basal.rate = 0;
              return;
            }
            if (current_duration > 15 && current_basal == scheduled_basal) {
              return;
            } else {
              current_temp_basal.duration = 30;
              current_temp_basal.rate = scheduled_basal;
              return;
            }
        }

        // calculate 30m low-temp required to get projected BG up to target
        // multiply by 2 to low-temp faster for increased hypo safety
        float insulinReq = 2 * MIN (0, (eventual_bg - target_bg) / SENS);
        // calculate naiveInsulinReq based on naive_eventual_bg
        float naiveInsulinReq = MIN (0, (naive_eventual_bg - target_bg) / SENS);
        if (minDelta < 0 && minDelta > expectedDelta) {
            // if we're barely falling, newinsulinReq should be barely negative
            insulinReq *= (minDelta / expectedDelta);
        }
        // rate required to deliver insulinReq less insulin over 30m:
        float rate = current_basal + (2 * insulinReq);

        // 5 / 120 is the inverse of how many 5 minute periods in 2h
        float insulinScheduled = current_duration * (current_basal - scheduled_basal) / 60;
        // if current temp would deliver a lot (30% of basal) less than the required insulin,
        // by both normal and naive calculations, then raise the rate
        float minInsulinReq = MIN (insulinReq, naiveInsulinReq);
        if (insulinScheduled < minInsulinReq - scheduled_basal * 0.3f) {
          current_temp_basal.duration = 30;
          current_temp_basal.rate = rate;
          return;
        }
        // calculate a long enough zero temp to eventually correct back up to target
        if ( rate <= 0 ) {
            int durationReq = ((int) (60.0f * (target_bg - naive_eventual_bg / SENS) / current_basal) + 0.5f);
            if (durationReq < 0) {
                durationReq = 0;
            // don't set a temp longer than 120 minutes
            } else {
                durationReq = ((int) (durationReq / 30.0f + 0.5f)) * 30;
                durationReq = MIN (120, 0 > durationReq? 0 : durationReq);
            }
            if (durationReq > 0) {
              current_temp_basal.duration = durationReq;
              current_temp_basal.rate = rate;
              return;
            }
        }
        current_temp_basal.duration = 30;
        current_temp_basal.rate = rate;
        return;
    }

    // eventual BG is at/above target
    // if iob is over max, just cancel any temps
    if (iob > MAX_IOB) {
        if (current_duration > 15 && current_basal == scheduled_basal) {
            return;
        } else {
          current_temp_basal.duration = 30;
          current_temp_basal.rate = scheduled_basal;
          return;
        }
    } else { // otherwise, calculate 30m high-temp required to get projected BG down to target
        //int minPredBG = (int) (MAX(39, minIOBPredBG) + 0.5f);
        // insulinReq is the additional insulin required to get minPredBG down to target_bg
        float insulinReq = (eventual_bg - target_bg) / SENS; // MIN (minPredBG, eventual_bg)
        // if that would put us over max_iob, then reduce accordingly
        if (insulinReq > MAX_IOB - iob) {
            insulinReq = MAX_IOB - iob;
        }

        // rate required to deliver insulinReq more insulin over 30m:
        float rate = MIN (MAX_BASAL_PUMP, scheduled_basal + (2 * insulinReq));

        float insulinScheduled = current_duration * (current_basal - scheduled_basal) / 60;
        if (insulinScheduled >= insulinReq * 2 || current_duration <= 5 || rate > current_basal) {
          // if current temp would deliver >2x more than the required insulin, lower the rate
          current_temp_basal.duration = 30;
          current_temp_basal.rate = rate;
          return;
        } else {
          // if required temp <~ existing temp basal
          return;
        }
    }

}
