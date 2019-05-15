#ifndef __UPDATE_H__
#define __UPDATE_H__

#ifdef __cplusplus
extern "C" {
#endif

/*
 * \brief USART callback for receiving commands
 */
void update_uart_cb(uint32_t event);

/**
 * \brief Update control task
 */
void update_low_priority(void *arg);

/**
 * \brief High priority update control task
 */
void update_high_priority(void *arg);

#ifdef __cplusplus
}
#endif

#endif /* __UPDATE_H__ */
