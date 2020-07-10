#include <stdio.h>
#include "sys/printk.h"
#include <zephyr.h>
#include <time.h>
#include <stddef.h>
#include <stdlib.h>

// --- data.h ---

#define SIZE 250
#define BG_Check 0
#define Temp_Basal 1
#define Correction_Bolus 2
#define Scheduled_Basal 3
struct Data {
time_t time; int type; float insulin, duration, rate, glucose;};
struct Data data[SIZE] = {
{0,Scheduled_Basal,0,0,0.425,0},
{9000,Scheduled_Basal,0,0,0.35,0},
{12324,BG_Check,0,0,0,80},
{12335,Temp_Basal,0,30,0,0},
{12624,BG_Check,0,0,0,80},
{12924,BG_Check,0,0,0,79},
{13224,BG_Check,0,0,0,77},
{13524,BG_Check,0,0,0,80},
{13539,Temp_Basal,0,30,0,0},
{13825,BG_Check,0,0,0,82},
{13907,Temp_Basal,0,0,0,0},
{14124,BG_Check,0,0,0,83},
{14424,BG_Check,0,0,0,82},
{14724,BG_Check,0,0,0,82},
{14735,Temp_Basal,0,30,0,0},
{14936,BG_Check,0,0,0,93},
{15026,BG_Check,0,0,0,86},
{15035,Temp_Basal,0,0,0,0},
{15324,BG_Check,0,0,0,83},
{15624,BG_Check,0,0,0,82},
{15924,BG_Check,0,0,0,82},
{15935,Temp_Basal,0,30,0,0},
{16224,BG_Check,0,0,0,83},
{16236,Temp_Basal,0,0,0,0},
{16524,BG_Check,0,0,0,85},
{16824,BG_Check,0,0,0,88},
{17124,BG_Check,0,0,0,90},
{17424,BG_Check,0,0,0,90},
{17724,BG_Check,0,0,0,92},
{18024,BG_Check,0,0,0,97},
{18324,BG_Check,0,0,0,104},
{18338,Temp_Basal,0,30,1.2,0},
{18624,BG_Check,0,0,0,108},
{18924,BG_Check,0,0,0,110},
{19224,BG_Check,0,0,0,110},
{19524,BG_Check,0,0,0,112},
{19537,Temp_Basal,0,30,1.15,0},
{19824,BG_Check,0,0,0,113},
{19837,Temp_Basal,0,30,1,0},
{20124,BG_Check,0,0,0,116},
{20137,Temp_Basal,0,30,0.825,0},
{20424,BG_Check,0,0,0,120},
{20437,Temp_Basal,0,30,0.8,0},
{20724,BG_Check,0,0,0,124},
{20738,Temp_Basal,0,30,1.1,0},
{21024,BG_Check,0,0,0,129},
{21037,Temp_Basal,0,30,1.15,0},
{21324,BG_Check,0,0,0,134},
{21338,Temp_Basal,0,30,1.2,0},
{21600,Scheduled_Basal,0,0,0.4,0},
{21624,BG_Check,0,0,0,139},
{21924,BG_Check,0,0,0,143},
{22224,BG_Check,0,0,0,146},
{22469,Temp_Basal,0,30,1.1,0},
{22524,BG_Check,0,0,0,147},
{22770,Temp_Basal,0,30,0.625,0},
{22824,BG_Check,0,0,0,147},
{23070,Temp_Basal,0,30,0.525,0},
{23124,BG_Check,0,0,0,146},
{23370,Temp_Basal,0,30,0.45,0},
{23424,BG_Check,0,0,0,145},
{23670,Temp_Basal,0,30,0.2,0},
{23724,BG_Check,0,0,0,143},
{23969,Temp_Basal,0,30,0.15,0},
{24024,BG_Check,0,0,0,142},
{24270,Temp_Basal,0,30,0.225,0},
{24324,BG_Check,0,0,0,141},
{24556,Temp_Basal,0,30,0.15,0},
{24624,BG_Check,0,0,0,137},
{24870,Temp_Basal,0,30,0,0},
{24924,BG_Check,0,0,0,133},
{25224,BG_Check,0,0,0,128},
{25524,BG_Check,0,0,0,123},
{25824,BG_Check,0,0,0,118},
{26069,Temp_Basal,0,30,0,0},
{26124,BG_Check,0,0,0,113},
{26424,BG_Check,0,0,0,109},
{26724,BG_Check,0,0,0,106},
{27024,BG_Check,0,0,0,102},
{27269,Temp_Basal,0,30,0,0},
{27324,BG_Check,0,0,0,99},
{27624,BG_Check,0,0,0,96},
{27924,BG_Check,0,0,0,91},
{28224,BG_Check,0,0,0,88},
{28469,Temp_Basal,0,30,0,0},
{28524,BG_Check,0,0,0,86},
{28824,BG_Check,0,0,0,84},
{29124,BG_Check,0,0,0,83},
{29424,BG_Check,0,0,0,82},
{29669,Temp_Basal,0,0,0,0},
{29724,BG_Check,0,0,0,84},
{30024,BG_Check,0,0,0,83},
{30324,BG_Check,0,0,0,83},
{30624,BG_Check,0,0,0,83},
{30924,BG_Check,0,0,0,81},
{31224,BG_Check,0,0,0,80},
{31524,BG_Check,0,0,0,81},
{31824,BG_Check,0,0,0,82},
{32124,BG_Check,0,0,0,82},
{32400,Scheduled_Basal,0,0,0.35,0},
{32424,BG_Check,0,0,0,81},
{32724,BG_Check,0,0,0,79},
{32969,Temp_Basal,0,30,0,0},
{33023,BG_Check,0,0,0,78},
{33324,BG_Check,0,0,0,78},
{33569,Temp_Basal,0,0,0,0},
{33623,BG_Check,0,0,0,77},
{33924,BG_Check,0,0,0,76},
{34223,BG_Check,0,0,0,76},
{34523,BG_Check,0,0,0,77},
{34824,BG_Check,0,0,0,78},
{35123,BG_Check,0,0,0,77},
{35423,BG_Check,0,0,0,75},
{35669,Temp_Basal,0,30,0,0},
{35724,BG_Check,0,0,0,76},
{35970,Temp_Basal,0,0,0,0},
{36023,BG_Check,0,0,0,75},
{36323,BG_Check,0,0,0,76},
{36623,BG_Check,0,0,0,76},
{36923,BG_Check,0,0,0,75},
{37169,Temp_Basal,0,30,0,0},
{37223,BG_Check,0,0,0,74},
{37523,BG_Check,0,0,0,73},
{37823,BG_Check,0,0,0,73},
{38123,BG_Check,0,0,0,77},
{38369,Temp_Basal,0,0,0,0},
{38423,BG_Check,0,0,0,79},
{38723,BG_Check,0,0,0,75},
{39023,BG_Check,0,0,0,76},
{39270,Temp_Basal,0,30,0,0},
{39323,BG_Check,0,0,0,78},
{39569,Temp_Basal,0,0,0,0},
{39623,BG_Check,0,0,0,76},
{39924,BG_Check,0,0,0,74},
{40183,Temp_Basal,0,30,0,0},
{40223,BG_Check,0,0,0,75},
{40483,Temp_Basal,0,0,0,0},
{40523,BG_Check,0,0,0,78},
{40823,BG_Check,0,0,0,80},
{41123,BG_Check,0,0,0,80},
{41424,BG_Check,0,0,0,81},
{41723,BG_Check,0,0,0,82},
{42023,BG_Check,0,0,0,82},
{42323,BG_Check,0,0,0,81},
{42624,BG_Check,0,0,0,81},
{42869,Temp_Basal,0,30,0,0},
{42923,BG_Check,0,0,0,81},
{43170,Temp_Basal,0,0,0,0},
{43223,BG_Check,0,0,0,81},
{43523,BG_Check,0,0,0,81},
{43823,BG_Check,0,0,0,81},
{44070,Temp_Basal,0,30,0.075,0},
{44123,BG_Check,0,0,0,81},
{44370,Temp_Basal,0,0,0,0},
{44423,BG_Check,0,0,0,83},
{44622,BG_Check,0,0,0,86},
{44723,BG_Check,0,0,0,83},
{44734,Correction_Bolus,2.1,0,0,0},
{45025,BG_Check,0,0,0,83},
{45270,Temp_Basal,0,30,0.1,0},
{45323,BG_Check,0,0,0,81},
{45570,Temp_Basal,0,30,0.05,0},
{45623,BG_Check,0,0,0,82},
{45870,Temp_Basal,0,0,0,0},
{45923,BG_Check,0,0,0,82},
{46223,BG_Check,0,0,0,81},
{46522,BG_Check,0,0,0,92},
{46822,BG_Check,0,0,0,101},
{47070,Temp_Basal,0,30,1.2,0},
{47122,BG_Check,0,0,0,112},
{47422,BG_Check,0,0,0,124},
{47722,BG_Check,0,0,0,132},
{48022,BG_Check,0,0,0,136},
{48270,Temp_Basal,0,30,1.2,0},
{48322,BG_Check,0,0,0,145},
{48622,BG_Check,0,0,0,155},
{48922,BG_Check,0,0,0,163},
{49222,BG_Check,0,0,0,170},
{49470,Temp_Basal,0,30,1.2,0},
{49522,BG_Check,0,0,0,168},
{49822,BG_Check,0,0,0,174},
{50122,BG_Check,0,0,0,167},
{50370,Temp_Basal,0,0,0,0},
{50400,Scheduled_Basal,0,0,0.4,0},
{50422,BG_Check,0,0,0,168},
{50670,Temp_Basal,0,30,0,0},
{50722,BG_Check,0,0,0,167},
{51022,BG_Check,0,0,0,162},
{51322,BG_Check,0,0,0,157},
{51622,BG_Check,0,0,0,152},
{51870,Temp_Basal,0,30,0,0},
{51922,BG_Check,0,0,0,148},
{52222,BG_Check,0,0,0,143},
{52823,BG_Check,0,0,0,130},
{53071,Temp_Basal,0,30,0,0},
{53123,BG_Check,0,0,0,122},
{53423,BG_Check,0,0,0,116},
{53722,BG_Check,0,0,0,109},
{54022,BG_Check,0,0,0,103},
{54272,Temp_Basal,0,30,0,0},
{54274,Temp_Basal,0,30,0,0},
{54322,BG_Check,0,0,0,95},
{54571,Temp_Basal,0,30,0,0},
{54622,BG_Check,0,0,0,88},
{54922,BG_Check,0,0,0,82},
{55222,BG_Check,0,0,0,75},
{55522,BG_Check,0,0,0,72},
{55708,BG_Check,0,0,0,63},
{55774,Temp_Basal,0,30,0,0},
{55822,BG_Check,0,0,0,69},
{56122,BG_Check,0,0,0,68},
{56422,BG_Check,0,0,0,72},
{56509,Correction_Bolus,0.475,0,0,0},
{56722,BG_Check,0,0,0,79},
{56971,Temp_Basal,0,0,0,0},
{57022,BG_Check,0,0,0,90},
{57322,BG_Check,0,0,0,103},
{57336,Correction_Bolus,1,0,0,0},
{57622,BG_Check,0,0,0,117},
{57869,Temp_Basal,0,30,1.2,0},
{57922,BG_Check,0,0,0,130},
{58222,BG_Check,0,0,0,142},
{58523,BG_Check,0,0,0,150},
{58823,BG_Check,0,0,0,153},
{59123,BG_Check,0,0,0,158},
{59131,Temp_Basal,0,30,1.2,0},
{59423,BG_Check,0,0,0,157},
{59723,BG_Check,0,0,0,151},
{60023,BG_Check,0,0,0,147},
{60031,Temp_Basal,0,30,0.875,0},
{60323,BG_Check,0,0,0,146},
{60332,Temp_Basal,0,30,0.15,0},
{60623,BG_Check,0,0,0,142},
{60632,Temp_Basal,0,30,0.125,0},
{60877,Temp_Basal,0,30,0,0},
{60922,BG_Check,0,0,0,136},
{61222,BG_Check,0,0,0,129},
{61522,BG_Check,0,0,0,120},
{61822,BG_Check,0,0,0,114},
{62122,BG_Check,0,0,0,104},
{62132,Temp_Basal,0,30,0,0},
{62422,BG_Check,0,0,0,88},
{62723,BG_Check,0,0,0,75},
{63022,BG_Check,0,0,0,66},
{63277,Temp_Basal,0,30,0,0},
{63322,BG_Check,0,0,0,59},
{63460,BG_Check,0,0,0,60},
{63622,BG_Check,0,0,0,54},
{63922,BG_Check,0,0,0,60},
{64222,BG_Check,0,0,0,58},
};

// monitor.h

struct requested_temp {
  float duration;
  float rate;
};

void init_monitor(void);
void add_reading(time_t, float);
void add_treatment(time_t, float);
void calculate_basal(float *, float *);

// --- monitor.c ---

/* User defined values in profile*/
#define TARGET_LOW 80
#define TARGET_HIGH 180
#define SENS 85
#define DIA 180 /* Number of minutes for dia in hours */

/* Defaults */
#define MAX_IOB 0.4
#define MAX_BASAL_PUMP 2
#define MAX_READINGS 200
#define MAX_BOLUS 10

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
struct treatment_list iob_data_absolute[MAX_BOLUS];
int iob_data_size;
struct glucose_readings glucose_data_absolute[MAX_READINGS];
int glucose_size;

void calculate_deltas(float *);
void determine_basal(float, float, float, float, float, float, float);

void init_monitor(void) {
  current_scheduled_basal.duration = 30.0f; // NOT USED
  current_scheduled_basal.rate = 1.2f;
  current_temp_basal.duration = 30.0f;
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
  if (short_num > 0) {
    deltas[0] = avg_bg_delta/short_num;
    deltas[1] = short_avg_delta/short_num;
  } else {
    deltas[0] = 0;
    deltas[1] = 0;
  }
  if (long_num > 0) {
    deltas[2] = long_avg_delta/long_num;
  } else {
    deltas[2] = 0;
  }

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
  for (int i = iob_data_size - 1; i >= 0; i--) {
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
    if (blood_glucose <= 80 || blood_glucose == 38 ) {
        if (current_basal > scheduled_basal) {
          // high temp is running or shorten long zero temps to 30m
            current_temp_basal.duration = 30;
            current_temp_basal.rate = 0;
            printk("  176 ");
            return;
        } else {
        printk("  179 ");
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

    float expectedDelta = bgi + (target_bg - eventual_bg) * 10 / DIA;

    // min_bg of 90 -> threshold of 65, 100 -> 70 110 -> 75, and 130 -> 85
    float threshold = TARGET_LOW - 0.5f * (TARGET_LOW - 40);

    // TODO: add carb impact

    if ( blood_glucose < threshold) {
      current_temp_basal.duration = 0;
      current_temp_basal.rate = 0;
      printk("  224 ");
      return;
    }

    if (eventual_bg < target_bg) { // if eventual BG is below target:
        // if 5m or 30m avg BG is rising faster than expected delta
        if ( minDelta > expectedDelta && minDelta > 0 ) {
            // if naive_eventual_bg < 40, set a 30m zero temp (oref0-pump-loop will let any longer SMB zero temp run)
            if (naive_eventual_bg < 40) {
              current_temp_basal.duration = 30;
              current_temp_basal.rate = 0;
              printk("  235 ");
              return;
            }
            if (current_duration > 15) {
              current_temp_basal.duration = 0;
              current_temp_basal.rate = 0;
              printk("  241 ");
              return;
            } else {
              current_temp_basal.duration = 30;
              current_temp_basal.rate = scheduled_basal;
              printk("  246 ");
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
          printk("  271 ");
          return;
        }
        // calculate a long enough zero temp to eventually correct back up to target
        if ( rate <= 0 ) {
            current_temp_basal.duration = 30;
            current_temp_basal.rate = 0;
            return;
        }
        current_temp_basal.duration = 30;
        current_temp_basal.rate = rate;
        printk("  301 ");
        return;
    }

    // eventual BG is at/above target
    // if iob is over max, just cancel any temps
    if (iob > MAX_IOB) {
        if (current_duration > 15 && current_basal == scheduled_basal) {
        printk("  309 ");
            return;
        } else {
          current_temp_basal.duration = 30;
          current_temp_basal.rate = scheduled_basal;
          printk("  314 ");
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
        float rate = MIN (MAX_BASAL_PUMP, scheduled_basal / 2 + insulinReq);
        /*float rate = MIN (MAX_BASAL_PUMP, scheduled_basal / 2 + insulinReq) + (short_avgdelta > 1? 0.2 : -0.2);
        if (long_avgdelta + bgi <= 8) {
          current_temp_basal.duration = 30;
          current_temp_basal.rate = rate / 2;
          return;
        }*/

        float insulinScheduled = current_duration * (current_basal - scheduled_basal) / 60;
        if (insulinScheduled >= insulinReq * 2 || current_duration <= 5 || rate < current_basal) {
          // if current temp would deliver >2x more than the required insulin, lower the rate
          current_temp_basal.duration = 30;
          current_temp_basal.rate = rate;
          printk("insulinReq: %f, blood_glucose: %f, eventual_bg: %f, target_bg: %f, iob: %f\n", insulinReq, blood_glucose, eventual_bg, target_bg, iob);
          printk("  334 ");
          return;
        } else {
          // if required temp <~ existing temp basal
          current_temp_basal.duration = 30;
          current_temp_basal.rate = scheduled_basal;
          printk("  338 ");
          return;
        }
    }

}

// --- main.c ---

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
