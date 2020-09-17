//#include <drivers/uart.h>
//#include <hal/nrf_uarte.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/printk.h>
#include <time.h>
#include <zephyr.h>
#include <zephyr/types.h>
#include <update/live_update.h>

#define DATA_PAYLOAD_SIZE 32
#define READING_SIZE 3
#define MAX_READINGS_SIZE 200 * READING_SIZE
#define TREATMENT_SIZE 4
#define MAX_TREATMENTS_SIZE 10 * TREATMENT_SIZE

// User defined values in profile
#define TARGET_LOW 80
#define TARGET_HIGH 180
#define SENS 85
#define DIA 180 // Number of minutes for dia in hours

// Defaults
#define MAX_IOB 0.4
#define MAX_BASAL_PUMP 2
#define MAX_READINGS 200
#define MAX_BOLUS 10

//extern void uarte_nrfx_irq_callback_set(struct device *, uart_irq_callback_user_data_t, void *);

// Peak activation percentage occurs at 75 minutes for DIA of 180 or 3 hours.
static float PEAK = DIA * 75.0f / 180;

//static struct device *uart_dev;
//static u32_t rx_buf[DATA_PAYLOAD_SIZE / sizeof(u32_t)];
//static u8_t rx_bytes = 0;

enum Type {BG_Check, Temp_Basal, Correction_Bolus, Scheduled_Basal};

u32_t glucose_readings[MAX_READINGS_SIZE];
int num_glucose_readings;

u32_t iob_treatments[MAX_TREATMENTS_SIZE];
int num_iob_treatments;

float current_temp_basal_duration = 30.0;
float current_temp_basal_rate = 0.0;
float current_scheduled_basal_rate = 1.2;

static inline u32_t _get_type(u32_t *data) {
    return *(u32_t *)(data + 1);
}

static inline time_t _get_time(u32_t *data) {
    return *(time_t *)(data + 2);
}

static inline float _get_insulin(u32_t *data) {
    return *(float *)(data + 4);
}

static inline float _get_duration(u32_t *data) {
    return *(float *)(data + 5);
}

static inline float _get_rate(u32_t *data) {
    return *(float *)(data + 6);
}

static inline float _get_glucose(u32_t *data) {
    return *(float *)(data + 7);
}

static inline time_t _get_reading_time(int i) {
    return *(time_t *)(&glucose_readings[i*READING_SIZE]);
}

static inline float _get_reading_glucose(int i) {
    return *(float *)(&glucose_readings[i*READING_SIZE + 2]);
}

// Represents the absolute time as seconds from initialization.
static inline time_t _get_treatment_time(int i) {
    return *(time_t *)(&iob_treatments[i*TREATMENT_SIZE]);
}

// Represents minutes between the next treatment and the current.
static inline float _get_treatment_minutes(int i) {
    return *(float *)(&iob_treatments[i*TREATMENT_SIZE + 2]);
}

static inline float _get_treatment_insulin(int i) {
    return *(float *)(&iob_treatments[i*TREATMENT_SIZE + 3]);
}

void add_reading(time_t rawtime, float glucose) {
    *(time_t *)(&glucose_readings[num_glucose_readings * READING_SIZE]) = rawtime;
    *(float *)(&glucose_readings[num_glucose_readings * READING_SIZE + 2]) = glucose;
    num_glucose_readings++;
}

void add_treatment(time_t rawtime, float bolus_insulin) {
    *(time_t *)(&iob_treatments[num_iob_treatments * TREATMENT_SIZE]) = rawtime;
    *(float *)(&iob_treatments[num_iob_treatments * TREATMENT_SIZE + 3]) = bolus_insulin;

    if (num_iob_treatments == 0) {
        *(float *)(&iob_treatments[num_iob_treatments * TREATMENT_SIZE + 2]) = 0;
    } else {
        *(float *)(&iob_treatments[num_iob_treatments * TREATMENT_SIZE + 2]) =
            (rawtime - _get_treatment_time(num_iob_treatments - 1)) / 60;
    }
    num_iob_treatments++;
}

void calculate_deltas(float *deltas) {
    /*
     * delta = change in BG between glucose (most recent BG) and an average of BG
     *   value from between 2.5 and 7.5 minutes ago (usually just a single BG value
     *   from 5 minutes ago) 
     * glucose = most recent BG
     * short_avgdelta = average rate of change (per 5m) in BG values between
     *   glucose (most recent BG) and each BG reading from 2.5 to 17.5 minutes ago
     * long_avgdelta = average rate of change (per 5m) in BG values between glucose
     *   (most recent BG) and each BG reading from 17.5 to 42.5 minutes ago
    */

    float avg_bg_delta = 0.0, short_avg_delta = 0.0, long_avg_delta = 0.0;
    int short_num = 0, long_num = 0;
    float current_difference;

    for (int index = num_glucose_readings - 1; index >= 0; index--) {
        current_difference = _get_reading_time(num_glucose_readings - 1) - _get_reading_time(index);
        if (2.5 * 60 <= current_difference && current_difference <= 7.5 * 60) {
            avg_bg_delta += _get_reading_glucose(index);
            short_avg_delta += (_get_reading_glucose(num_glucose_readings - 1) - _get_reading_glucose(index));
            short_num++;
        } else if (17.5 * 60 <= current_difference && current_difference <= 42.5 * 60) {
            long_avg_delta += (_get_reading_glucose(num_glucose_readings - 1) - _get_reading_glucose(index));
            long_num++;
        }
    }

    deltas[0] = short_num > 0 ? avg_bg_delta / short_num : 0;
    deltas[1] = short_num > 0 ? short_avg_delta / short_num : 0;
    deltas[2] = long_num > 0 ? long_avg_delta / long_num : 0;
}

float get_activity_percentage(int minutes) {
    if (minutes <= PEAK) {
        return 2 * minutes / (DIA * PEAK);
    } else {
        return 2 * (DIA - minutes) / (DIA * (DIA - PEAK));
    }
}

float calculate_activity() {
    int total_activity = 0;
    int cumulative_minutes = 0;
    for (int i = num_iob_treatments - 1; i >= 0; i--) {
        if (cumulative_minutes >= DIA) break;

        cumulative_minutes += _get_treatment_minutes(i);
        total_activity += _get_treatment_insulin(i) * get_activity_percentage(cumulative_minutes);
    }
    return total_activity;
}

// sum for all insulin treatments in units of insulin remaining each minute
float calculate_iob() {
    float total_iob = 0;
    int cumulative_minutes = 0;
    for (int i = num_iob_treatments - 1; i >= 0; i--) {
        cumulative_minutes += _get_treatment_minutes(i);
        if (cumulative_minutes >= DIA) {
            break;
        } else if (cumulative_minutes <= PEAK) {
            total_iob += _get_treatment_insulin(i) * get_activity_percentage(cumulative_minutes) * cumulative_minutes * 0.5;
        } else {
            total_iob += _get_treatment_insulin(i) * (DIA - get_activity_percentage(cumulative_minutes) * (DIA - cumulative_minutes)) * 0.5;
        }
    }
    return total_iob;
}

// All constants according to oref design
void determine_basal(float blood_glucose, float current_basal, float current_duration, float scheduled_basal,
                     float delta, float short_avgdelta, float long_avgdelta) {

    if (blood_glucose <= 80 || blood_glucose == 38 ) {
        if (current_basal > scheduled_basal) { // high temp is running or shorten long zero temps to 30m
            current_temp_basal_duration = 30.0;
            current_temp_basal_rate = 0.0;
            //printk("  176 ");
        } else {
            //printk("  179 ");
        }
        return;
    }

    // if target_high and target_low are set, then set target to their average
    float target_bg = (TARGET_LOW + TARGET_HIGH) / 2.0f;
    float minDelta = delta < short_avgdelta ? delta : short_avgdelta;
    float minAvgDelta = short_avgdelta < long_avgdelta ? short_avgdelta : long_avgdelta;
    //float maxDelta = long_avgdelta > short_avgdelta + delta - minDelta ? long_avgdelta : short_avgdelta + delta - minDelta;

    // TODO: compare currenttemp to lastTemp and cancel temp if they don't match

    //calculate BG impact: the amount BG "should" be rising or falling based on insulin activity alone
    float activity = calculate_activity();
    float bgi = - activity * SENS * 5;
    // project deviations for 30 minutes
    float deviation = 30 / 5 * (minDelta - bgi);
    // don't overreact to a big negative delta: use minAvgDelta if deviation is negative
    if (deviation < 0) {
        deviation = (30 / 5) * (minAvgDelta - bgi) ;
        // and if deviation is still negative, use long_avgdelta
        if (deviation < 0) {
            deviation = (30 / 5) * (long_avgdelta - bgi) ;
        }
    }

    float iob = calculate_iob();
    // calculate the naive (bolus calculator math) eventual BG based on net IOB and sensitivity
    float naive_eventual_bg = blood_glucose - iob * SENS;
    // and adjust it for the deviation above
    float eventual_bg = naive_eventual_bg + deviation;

    // TODO: raise target for noisy / raw CGM data

    float expectedDelta = bgi + (target_bg - eventual_bg) * 10 / DIA;

    // min_bg of 90 -> threshold of 65, 100 -> 70 110 -> 75, and 130 -> 85
    float threshold = TARGET_LOW - 0.5f * (TARGET_LOW - 40);

    // TODO: add carb impact

    if (blood_glucose < threshold) {
        current_temp_basal_duration = 0;
        current_temp_basal_rate = 0;
        //printk("  224 ");
        return;
    }

    if (eventual_bg < target_bg) { // if eventual BG is below target:
        // if 5m or 30m avg BG is rising faster than expected delta
        if (minDelta > expectedDelta && minDelta > 0) {
            // if naive_eventual_bg < 40, set a 30m zero temp (oref0-pump-loop will let any longer SMB zero temp run)
            if (naive_eventual_bg < 40) {
                current_temp_basal_duration = 30;
                current_temp_basal_rate = 0;
                //printk("  235 ");
                return;
            }
            if (current_duration > 15) {
                current_temp_basal_duration = 0;
                current_temp_basal_rate = 0;
                //printk("  241 ");
                return;
            } else {
                current_temp_basal_duration = 30;
                current_temp_basal_rate = scheduled_basal;
                //printk("  246 ");
                return;
            }
        }

        // calculate 30m low-temp required to get projected BG up to target
        // multiply by 2 to low-temp faster for increased hypo safety
        float insulinReq = 2 * ((eventual_bg - target_bg) / SENS < 0 ? (eventual_bg - target_bg) / SENS : 0);
        // calculate naiveInsulinReq based on naive_eventual_bg
        float naiveInsulinReq = ((naive_eventual_bg - target_bg) / SENS < 0 ? (naive_eventual_bg - target_bg) / SENS : 0);
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
        float minInsulinReq = insulinReq < naiveInsulinReq ? insulinReq : naiveInsulinReq;
        if (insulinScheduled < minInsulinReq - scheduled_basal * 0.3f) {
            current_temp_basal_duration = 30;
            current_temp_basal_rate = rate;
            //printk("  271 ");
            return;
        }
        // calculate a long enough zero temp to eventually correct back up to target
        if (rate <= 0) {
            current_temp_basal_duration = 30;
            current_temp_basal_rate = 0;
            return;
        }
        current_temp_basal_duration = 30;
        current_temp_basal_rate = rate;
        //printk("  301 ");
        return;
    }

    // eventual BG is at/above target
    // if iob is over max, just cancel any temps
    if (iob > MAX_IOB) {
        if (current_duration > 15 && current_basal == scheduled_basal) {
            //printk("  309 ");
            return;
        } else {
            current_temp_basal_duration = 30;
            current_temp_basal_rate = scheduled_basal;
            //printk("  314 ");
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
        //float rate = MAX_BASAL_PUMP < (scheduled_basal / 2 + insulinReq) ? MAX_BASAL_PUMP : (scheduled_basal / 2 + insulinReq);
        float temp_rate = MAX_BASAL_PUMP < (scheduled_basal / 2 + insulinReq) ? MAX_BASAL_PUMP : (scheduled_basal / 2 + insulinReq);
        float rate = temp_rate + (short_avgdelta > 1 ? 0.2 : -0.2);
        if (long_avgdelta + bgi <= 8) {
            current_temp_basal_duration = 30;
            current_temp_basal_rate = rate / 2;
            return;
        }

        float insulinScheduled = current_duration * (current_basal - scheduled_basal) / 60;
        if (insulinScheduled >= insulinReq * 2 || current_duration <= 5 || rate < current_basal) {
            // if current temp would deliver >2x more than the required insulin, lower the rate
            current_temp_basal_duration = 30;
            current_temp_basal_rate = rate;
            //printk("insulinReq: %f, blood_glucose: %f, eventual_bg: %f, target_bg: %f, iob: %f\n", insulinReq, blood_glucose, eventual_bg, target_bg, iob);
            //printk("  334 ");
            return;
        } else {
            // if required temp <~ existing temp basal
            current_temp_basal_duration = 30;
            current_temp_basal_rate = scheduled_basal;
            //printk("  338 ");
            return;
        }
    }
}

void calculate_basal() {
    float glucose_deltas[3];

    calculate_deltas(glucose_deltas);
    determine_basal(
        _get_reading_glucose(num_glucose_readings - 1),
        current_temp_basal_rate,
        current_temp_basal_duration,
        current_scheduled_basal_rate,
        glucose_deltas[0], glucose_deltas[1], glucose_deltas[2]   
    );
}

// --- main.c ---
int n = 0;

void handle_data(struct k_timer *t) {
    u32_t *rx_buf = lu_get_data(n);

    if (_get_type(rx_buf) == BG_Check) {
        add_reading(_get_time(rx_buf), _get_glucose(rx_buf));

    } else if (_get_type(rx_buf) == Correction_Bolus) {
        add_treatment(_get_time(rx_buf), _get_insulin(rx_buf));

    } else if (_get_type(rx_buf) == Temp_Basal) {
        calculate_basal();

        // calculated duration and rate
        printk("U Calculated duration %d.%d and rate %d.%d\n",
                (int) current_temp_basal_duration, (int) (current_temp_basal_duration * 1000) % 1000,
                (int) current_temp_basal_rate, (int) (current_temp_basal_rate * 1000) % 1000
        );
        // expected duration and rate
        printk("U Expected duration %d.%d and rate %d.%d\n",
                (int) _get_duration(rx_buf), (int) (_get_duration(rx_buf) * 1000) % 1000,
                (int) _get_rate(rx_buf), (int) (_get_rate(rx_buf) * 1000) % 1000
        );
    }

    n++;
}

/*
void glucose_uart_rx_cb(void *x) {
    uart_irq_update(uart_dev);

    while (uart_irq_rx_ready(uart_dev)) {
        int len = uart_fifo_read(uart_dev, ((unsigned char *)rx_buf) + rx_bytes, 4);
        if (len == 4 && rx_buf[rx_bytes/4] == 0xfdfdfdfd) {
            rx_buf[0] = 0xfdfdfdfd;
            rx_buf[rx_bytes/4] = 0x0;
            rx_bytes = 4;
        } else {
            rx_bytes += len;
        }
    }

    if (rx_bytes == DATA_PAYLOAD_SIZE) {
        if (rx_buf[0] == 0xfdfdfdfd) { // payload successfully received
            //printk("payload received, magic matches!\n");
            handle_data();
        } else { // issue in reception
            //printk("payload received, magic MISMATCH (%x)\n", rx_buf[0]);
        }
	    rx_bytes = 0;
    }
}
*/

struct k_timer timer;

int main(void) {
    //uart_dev = device_get_binding("UART_0");
    //uarte_nrfx_irq_callback_set(uart_dev, glucose_uart_rx_cb, NULL);
    //uart_irq_rx_enable(uart_dev);
    //printk("app uart dev: %p\n", uart_dev);

    k_timer_init(&timer, handle_data, NULL);
    k_timer_start(&timer, K_MSEC(1000), K_MSEC(1000));
    return 0;
}

