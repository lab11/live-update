/* User defined values in profile*/
#define TARGET_LOW 80
#define TARGET_HIGH 180
#define SENS 85
#define DIA 180 /* Number of minutes for dia in hours */

/* Defaults */
#define MAX_IOB 0
#define MAX_BASAL_PUMP 2

/* MACROS */
#define MIN(a, b) ((a)<(b)? (a) : (b))

/* Peak activation percentage occurs at 75 minutes for DIA of 180 or 3 hours. */
float PEAK = DIA * 75.0f / 180;

struct requested_temp {
  int duration;
  float rate;
};

struct treatment_list {
  int minutes; /* Represents minutes between the next treatment and the current. */
  float insulin;
  struct treatment_list * prev;
};

float get_activity_percentage (int minutes) {
  if (minutes <= PEAK) {
    return 2 * minutes / (DIA * PEAK);
  } else {
    return 2 * (DIA - minutes) / (DIA * (DIA - PEAK));
  }
}

/* sum for all insulin treatments in units of insulin used per minute */
float calculate_activity (struct treatment_list * iob_data) {
  int total_activity = 0;
  struct treatment_list * iob_data_used = iob_data;
  iob_data = iob_data -> prev; // Initial block is a sentinel
  int cummulative_minutes = iob_data -> minutes;
  while (DIA <= cummulative_minutes) {
    total_activity += (iob_data -> insulin) * get_activity_percentage (cummulative_minutes);
    iob_data_used = iob_data;
    iob_data = iob_data -> prev;
  }
  iob_data_used -> prev = NULL;
  return total_activity;
}

/* sum for all insulin treatments in units of insulin remaining each minute */
float calculate_iob (struct treatment_list * iob_data) {
  int cummulative_minutes = 0;
  int total_iob = 0;
  while (iob_data != NULL) {
    cummulative_minutes += iob_data -> minutes;
    if (cummulative_minutes <= PEAK) {
      total_iob += (iob_data -> insulin)
        * get_activity_percentage (cummulative_minutes)
        * cummulative_minutes * 0.5f;
    } else {
      total_iob += (iob_data -> insulin)
        * (DIA - get_activity_percentage (cummulative_minutes)
        * (DIA - cummulative_minutes)) * 0.5f;
    }

    iob_data = iob_data -> prev;
  }
  return total_iob;
}

// All constants according to oref design
struct requested_temp * determine_basal(float blood_glucose,
  float current_basal, float current_duration, float scheduled_basal,
  struct treatment_list * iob_data,
  float delta, float short_avgdelta, float long_avgdelta) {
  struct requested_temp * rT = (struct requested_temp *) malloc (sizeof (requested_temp));
    if (blood_glucose <= 10 || blood_glucose === 38 ) {
        if (current_basal > scheduled_basal || current_basal == 0 && current_duration > 30 ) {
          // high temp is running or shorten long zero temps to 30m
            rT -> duration = 30;
            rT -> rate = 0;
            return rT;
        } else { //do nothing.
          return NULL;
        }
    }

    // if target_high and target_low are set, then set target to their average
    float target_bg = (TARGET_LOW + TARGET_HIGH) / 2.0f;
    float minDelta = MIN (delta, short_avgdelta);
    float minAvgDelta = MIN (short_avgdelta , long_avgdelta);
    float maxDelta = long_avgdelta > short_avgdelta + delta - minDelta ? long_avgdelta : short_avgdelta + delta - minDelta;

    // TODO: compare currenttemp to lastTemp and cancel temp if they don't match

    //calculate BG impact: the amount BG "should" be rising or falling based on insulin activity alone
    float activity = calculate_activity (iob_data);
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

    float iob = calculate_iob (iob_data);
    // calculate the naive (bolus calculator math) eventual BG based on net IOB and sensitivity
    float naive_eventualBG = blood_glucose - iob * SENS;
    // and adjust it for the deviation above
    float eventualBG = naive_eventualBG + deviation;

    // TODO: raise target for noisy / raw CGM data

    float expectedDelta = bgi + (target_bg - eventual_bg) * 5 / 120;

    // min_bg of 90 -> threshold of 65, 100 -> 70 110 -> 75, and 130 -> 85
    float threshold = TARGET_LOW - 0.5f * (TARGET_LOW - 40);

    // TODO: add carb impact

    if ( blood_glucose < threshold) {
      rT -> duration = 0;
      rT -> rate = 0;
      return rT;
    }

    if (eventualBG < TARGET_LOW) { // if eventual BG is below target:
        // if 5m or 30m avg BG is rising faster than expected delta
        if ( minDelta > expectedDelta && minDelta > 0 ) {
            // if naive_eventualBG < 40, set a 30m zero temp (oref0-pump-loop will let any longer SMB zero temp run)
            if (naive_eventualBG < 40) {
              rT -> duration = 30;
              rT -> rate = 0;
              return rT;
            }
            if (current_duration > 15 && current_basal == scheduled_basal) {
              return NULL;
            } else {
              rT -> duration = 30;
              rT -> rate = scheduled_basal;
              return rT;
            }
        }

        // calculate 30m low-temp required to get projected BG up to target
        // multiply by 2 to low-temp faster for increased hypo safety
        float insulinReq = 2 * MIN (0, (eventualBG - target_bg) / SENS);
        // calculate naiveInsulinReq based on naive_eventualBG
        float naiveInsulinReq = MIN (0, (naive_eventualBG - target_bg) / SENS);
        if (minDelta < 0 && minDelta > expectedDelta) {
            // if we're barely falling, newinsulinReq should be barely negative
            insulinReq *= (minDelta / expectedDelta);
        }
        // rate required to deliver insulinReq less insulin over 30m:
        float rate = basal + (2 * insulinReq);

        // 5 / 120 is the inverse of how many 5 minute periods in 2h
        float insulinScheduled = current_duration * (current_basal - scheduled_basal) / 60;
        // if current temp would deliver a lot (30% of basal) less than the required insulin,
        // by both normal and naive calculations, then raise the rate
        float minInsulinReq = MIN (insulinReq, naiveInsulinReq);
        if (insulinScheduled < minInsulinReq - scheduled_basal * 0.3f) {
          rT -> duration = 30;
          rT -> rate = rate;
          return rT;
        }
        // calculate a long enough zero temp to eventually correct back up to target
        if ( rate <= 0 ) {
            float durationReq = round(60 * (target_bg - naive_eventualBG / SENS) / current_basal);
            if (durationReq < 0) {
                durationReq = 0;
            // don't set a temp longer than 120 minutes
            } else {
                durationReq = round(durationReq/30)*30;
                durationReq = MIN (120, 0 > durationReq? 0 : durationReq);
            }
            if (durationReq > 0) {
              rT -> duration = durationReq;
              rT -> rate = rate;
              return rT;
            }
        }
        rT -> duration = 30;
        rT -> rate = rate;
        return rT;
    }

    // eventual BG is at/above target
    // if iob is over max, just cancel any temps
    if (iob_data.iob > MAX_IOB) {
        if (current_duration > 15 && current_basal == scheduled_basal) {
            return NULL;
        } else {
          rT -> duration = 30;
          rT -> rate = scheduled_basal;
          return rT;
        }
    } else { // otherwise, calculate 30m high-temp required to get projected BG down to target

        // insulinReq is the additional insulin required to get minPredBG down to target_bg
        insulinReq = (MIN (minPredBG, eventualBG) - target_bg) / SENS;
        // if that would put us over max_iob, then reduce accordingly
        if (insulinReq > MAX_IOB - iob) {
            insulinReq = MAX_IOB - iob;
        }

        // rate required to deliver insulinReq more insulin over 30m:
        rate = MIN (MAX_BASAL_PUMP, scheduled_basal + (2 * insulinReq));

        insulinScheduled = current_duration * (current_basal - scheduled_basal) / 60;
        if (insulinScheduled >= insulinReq * 2 || current_duration <= 5 || rate > current_basal) {
          // if current temp would deliver >2x more than the required insulin, lower the rate
          rT -> duration = 30;
          rT -> rate = rate;
          return rT;
        } else {
          // if required temp <~ existing temp basal
          return NULL;
        }
    }

};
