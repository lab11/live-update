#include <time.h>

void init_monitor(void);
float calculate_basal(time_t, float);
void set_current_basal(float);
void add_treatment(time_t, float);
