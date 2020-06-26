#ifndef __TFM_GPIO_VENEERS_H__
#define __TFM_GPIO_VENEERS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

// Return 0 on success, non-zero on failure
uint32_t tfm_gpio_read_all(void);				// added by Ryan
uint8_t tfm_gpio_write_all(uint32_t out);		// added by Ryan
uint8_t tfm_gpio_enable_outputs(uint32_t pins);	// added by Ryan
uint8_t tfm_gpio_read(uint8_t pin);				// added by Ryan

uint8_t tfm_gpio_enable_output(uint8_t pin);
uint8_t tfm_gpio_set(uint8_t pin);
uint8_t tfm_gpio_clear(uint8_t pin);
uint8_t tfm_gpio_disable(uint8_t pin);
uint8_t tfm_gpio_read(uint8_t pin);

/*
 * Configuration for gpio interrupts
 * ---------------------------------
 * type = 1 --> interrupt on edge
 *     polarity = 0 --> falling edge
 *     polarity = 1 --> rising edge
 * type = 0 --> interrupt on level
 *     polarity = 0 --> low level
 *     polarity = 1 --> high level
 *
 * cb is the desired callback on interrupt
 */
typedef struct {
    uint8_t type;
    uint8_t polarity;
    void (*cb) (void);
} gpio_int_config;

uint8_t tfm_gpio_interrupt_enable(uint8_t pin, gpio_int_config *cfg);
uint8_t tfm_gpio_interrupt_disable(uint8_t pin);

#ifdef __cplusplus
}
#endif

#endif /* __TFM_GPIO_VENEERS_H__ */

