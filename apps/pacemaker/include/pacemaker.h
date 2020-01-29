
void observe(char* event);

void lri_observe(char* event);
void aei_observe(char* event);
void lri_timer_stop_cb(struct k_timer *t);
void aei_timer_stop_cb(struct k_timer *t);
void aei_timer_expire_cb(struct k_timer *t);
void vrp_observe(char* event);
void uri_observe(char* event);
void avi_observe(char* event);
void uri_timer_expire_cb(struct k_timer *t);
void avi_timer_expire_cb(struct k_timer *t);
void vrp_timer_expire_cb(struct k_timer *t);
void notify_fsms_ventricle();