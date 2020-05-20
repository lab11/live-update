#include <time.h>

struct requested_temp {
  float duration;
  float rate;
};

void init_monitor(void);
void add_reading(time_t, float);
void set_current_basal(float);
void add_treatment(time_t, float);
void calculate_basal(float *, float *);
