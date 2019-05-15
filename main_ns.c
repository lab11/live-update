/*
 * Copyright (c) 2017-2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "tfm_api.h"
#include "cmsis_os2.h"
#include "tfm_integ_test.h"
#include "tfm_ns_svc.h"
#include "tfm_ns_lock.h"
#include "blink.h"
#include "update.h"
#include "target_cfg.h"
#include "Driver_USART.h"

/* For UART the CMSIS driver is used */
extern ARM_DRIVER_USART NS_DRIVER_STDIO;

/**
 * \brief Modified table template for user defined SVC functions
 *
 * \details RTX has a weak definition of osRtxUserSVC, which
 *          is overridden here
 */
#if (defined(__ARMCC_VERSION) && (__ARMCC_VERSION == 6110004))
/* Workaround needed for a bug in Armclang 6.11, more details at:
 * http://www.keil.com/support/docs/4089.htm
 */
__attribute__((section(".gnu.linkonce")))
#endif
extern void * const osRtxUserSVC[1+USER_SVC_COUNT];
       void * const osRtxUserSVC[1+USER_SVC_COUNT] = {
  (void *)USER_SVC_COUNT,

#define X(SVC_ENUM, SVC_HANDLER) (void*)SVC_HANDLER,

    /* SVC API for Services */
#ifdef TFM_NS_CLIENT_IDENTIFICATION
    LIST_SVC_NSPM
#endif

#undef X

/*
 * (void *)user_function1,
 *  ...
 */
};

/* Struct FILE is implemented in stdio.h. Used to redirect printf to
 * NS_DRIVER_STDIO
 */
FILE __stdout;
/* Redirects armclang printf to NS_DRIVER_STDIO */
int fputc(int ch, FILE *f) {
    /* Send byte to NS_DRIVER_STDIO */
    (void)NS_DRIVER_STDIO.Send((const unsigned char *)&ch, 1);
    /* Return character written */
    return ch;
}
/* redirects gcc printf to NS_DRIVER_STDIO */
int _write(int fd, char * str, int len)
{
    (void)NS_DRIVER_STDIO.Send(str, len);

    return len;
}

static uint64_t blink_stack[(1024U) / (sizeof(uint64_t))];
static const osThreadAttr_t blink_attr = {
    .name = "blink",
    .stack_size = 1024U,
    .stack_mem = blink_stack,
    .priority = osPriorityHigh,
};

static uint64_t update_lp_stack[(3U * 1024U) / (sizeof(uint64_t))];
static uint64_t update_hp_stack[(3U * 1024U) / (sizeof(uint64_t))];

static const osThreadAttr_t update_lp_attr = {
    .name = "update-low-priority",
    .stack_size = sizeof(update_lp_stack),
    .stack_mem = update_lp_stack,
};

static const osThreadAttr_t update_hp_attr = {
    .name = "update-high-priority",
    .stack_size = sizeof(update_hp_stack),
    .stack_mem = update_hp_stack,
    .priority = osPriorityRealtime,
};

static osStatus_t   status;
static osThreadId_t thread_id;

struct update_args {
    osThreadId_t blink_id;
    uint64_t *blink_stack;
} ua;

#ifndef __GNUC__
__attribute__((noreturn))
#endif
int main(void)
{
    NS_DRIVER_STDIO.Initialize(update_uart_cb);
    NS_DRIVER_STDIO.Control(ARM_USART_MODE_ASYNCHRONOUS, 115200);

    status = osKernelInitialize();

    /* Initialize the TFM NS lock */
    tfm_ns_lock_init();
    thread_id = osThreadNew(blink, NULL, &blink_attr);
    ua.blink_id = thread_id;
    ua.blink_stack = (uint64_t *) &blink_stack;
    osThreadNew(update_low_priority, &ua, &update_lp_attr);
    osThreadNew(update_high_priority, NULL, &update_hp_attr);
    status = osKernelStart();

    /* Reached only in case of error */
    while(1);
}
