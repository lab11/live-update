/*
 * Copyright (c) 2013-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file
 * @brief Linker command/script file
 *
 * Linker script for the Cortex-M platforms.
 */

#define _LINKER
#define _ASMLANGUAGE

#include <autoconf.h>
#include <linker/sections.h>
#include <generated_dts_board.h>

#include <linker/linker-defs.h>
#include <linker/linker-tool.h>

#define CONFIG_APP_SECTIONS 1

#define _APPRAM_DATA_SECTION_NAME      .app_data
#define _APPRAM_BSS_SECTION_NAME       .app_bss
#define _APPFLASH_RODATA_SECTION_NAME  .app_rodata
#define _APPFLASH_TEXT_SECTION_NAME    .app_text

#define APPFLASH_ADDR  (CONFIG_FLASH_BASE_ADDRESS + CONFIG_APP_FLASH_OFFSET)
#define APPFLASH_SIZE  (CONFIG_APP_FLASH_SIZE)

#define APPRAM_ADDR   (CONFIG_SRAM_BASE_ADDRESS + CONFIG_APP_SRAM_OFFSET)
#define APPRAM_SIZE   (CONFIG_APP_SRAM_SIZE)

MEMORY
    {
    APPRAM                  (wx) : ORIGIN = APPRAM_ADDR, LENGTH = APPRAM_SIZE
    APPFLASH                (rx) : ORIGIN = APPFLASH_ADDR , LENGTH = APPFLASH_SIZE
    }

#include <arch/arm/cortex_m/scripts/linker.ld>
