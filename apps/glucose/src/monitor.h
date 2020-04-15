#include <time.h>

struct requested_temp {
  float duration;
  float rate;
};

void init_monitor(void);
void calculate_basal(time_t, float, float *, float *);
void set_current_basal(float);
void add_treatment(time_t, float);
