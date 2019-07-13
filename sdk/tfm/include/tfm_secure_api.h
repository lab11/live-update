/*
 * Copyright (c) 2017-2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __TFM_SECURE_API_H__
#define __TFM_SECURE_API_H__

#include <arm_cmse.h>
#include "tfm_svc.h"
#include "secure_utilities.h"
#include "tfm_core.h"
#include "tfm_api.h"
#include "tfm_boot_status.h"

/*!
 * \def __tfm_secure_gateway_attributes__
 *
 * \brief Attributes for secure gateway functions
 */
#define __tfm_secure_gateway_attributes__ \
        __attribute__((cmse_nonsecure_entry, noinline, section("SFN")))

/* Hide specific errors if not debugging */
#ifdef TFM_CORE_DEBUG
#define TFM_ERROR_STATUS(status) (status)
#else
#define TFM_ERROR_STATUS(status) (TFM_PARTITION_BUSY)
#endif

#define TFM_SFN_API_LEGACY 0
#define TFM_SFN_API_IOVEC 1

#ifndef TFM_LVL
#error TFM_LVL is not defined!
#endif

enum tfm_buffer_share_region_e {
    TFM_BUFFER_SHARE_DISABLE,
    TFM_BUFFER_SHARE_NS_CODE,
    TFM_BUFFER_SHARE_SCRATCH,
    TFM_BUFFER_SHARE_PRIV, /* only for TCB in level 2, all in level 1 */
    TFM_BUFFER_SHARE_DEFAULT,
};

enum tfm_ns_region_e {
    TFM_NS_REGION_CODE = 0,
    TFM_NS_REGION_DATA,
    TFM_NS_REGION_VENEER,
    TFM_NS_REGION_PERIPH_1,
    TFM_NS_REGION_PERIPH_2,
    TFM_NS_SECONDARY_IMAGE_REGION,
};

enum tfm_memory_access_e {
    TFM_MEMORY_ACCESS_RO = 1,
    TFM_MEMORY_ACCESS_RW = 2,
};

#endif /* __TFM_SECURE_API_H__ */
