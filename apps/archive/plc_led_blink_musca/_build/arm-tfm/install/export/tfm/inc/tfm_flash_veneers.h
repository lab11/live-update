#ifndef __TFM_FLASH_VENEERS_H__
#define __TFM_FLASH_VENEERS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

uint8_t tfm_flash_init();

uint8_t tfm_flash_is_busy();
uint8_t tfm_flash_is_error();
uint8_t tfm_flash_read(uint32_t addr, uint32_t *buf, uint32_t len);
uint8_t tfm_flash_write(uint32_t addr, uint32_t *buf, uint32_t len);
uint8_t tfm_flash_write_step();

#ifdef __cplusplus
}
#endif

#endif /* __TFM_FLASH_VENEERS_H__ */

