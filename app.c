/*
 * Copyright (c) 2017-2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

//#include "blink.h"
#include "cmsis_os2.h"
#include "Driver_USART.h"
#include "tfm_api.h"
#include "target_cfg.h"
#include "tfm_ns_svc.h"
#include "tfm_ns_lock.h"
#include "update.h"

// github.com/lab11/pic-explorations/movable_blink/code.c
typedef struct {
    unsigned int* entry_loc;        /* Entry point for user application */
    unsigned int* init_data_loc;    /* Data initialization information in flash */
    unsigned int init_data_size;    /* Size of initialization information */
    unsigned int got_start_offset;  /* Offset to start of GOT */
    unsigned int got_end_offset;    /* Offset to end of GOT */
    unsigned int plt_start_offset;  /* Offset to start of PLT */
    unsigned int plt_end_offset;    /* Offset to end of PLT */
    unsigned int bss_start_offset;  /* Offset to start of BSS */
    unsigned int bss_end_offset;    /* Offset to end of BSS */
    unsigned int rel_start_offset;  /* Offset to start of relocation table */
    unsigned int rel_end_offset;    /* Offset to end of relocation table */
} Load_Info;

extern void  main(void);
extern unsigned int* _etext;
extern unsigned int* _edata;
extern unsigned int* _got;
extern unsigned int* _egot;
extern unsigned int* _plt;
extern unsigned int* _eplt;
extern unsigned int* _bss;
extern unsigned int* _ebss;
extern unsigned int* _rel;
extern unsigned int* _erel;

// Load Info is used by the runtime in order to load the application
//  Note that locations in the text section assume .text starts at 0x10000000
//  and are therefore updated
__attribute__ ((section(".load_info"), used))
Load_Info app_info = {
    .entry_loc          = (unsigned int*)((unsigned int)main - 0x10000000),
    .init_data_loc      = (unsigned int*)((unsigned int)(&_etext) - 0x10000000),
    .init_data_size     = (unsigned int)(&_edata),
    .got_start_offset   = (unsigned int)(&_got),
    .got_end_offset     = (unsigned int)(&_egot),
    .plt_start_offset   = (unsigned int)(&_plt),
    .plt_end_offset     = (unsigned int)(&_eplt),
    .bss_start_offset   = (unsigned int)(&_bss),
    .bss_end_offset     = (unsigned int)(&_ebss),
    .rel_start_offset   = ((unsigned int)(&_rel) - 0x10000000),
    .rel_end_offset     = ((unsigned int)(&_erel) - 0x10000000),
};

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
//FILE __stdout;
/* Redirects armclang printf to NS_DRIVER_STDIO */
//int fputc(int ch, FILE *f) {
    /* Send byte to NS_DRIVER_STDIO */
//    (void)NS_DRIVER_STDIO.Send((const unsigned char *)&ch, 1);
    /* Return character written */
//    return ch;
//}
/* redirects gcc printf to NS_DRIVER_STDIO */
//int _write(int fd, char * str, int len)
//{
//    (void)NS_DRIVER_STDIO.Send(str, len);

//    return len;
//}

static uint64_t blink_stack[(1024U) / (sizeof(uint64_t))];
static const osThreadAttr_t blink_attr = {
    .name = "blink",
    .stack_size = 1024U,
    .stack_mem = blink_stack,
    .priority = osPriorityHigh,
};

/*
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
*/

static osStatus_t   status;
static osThreadId_t thread_id;

/*
struct update_args {
    osThreadId_t blink_id;
    uint64_t *blink_stack;
} ua;
*/

__attribute__((noreturn))
void busyloop() {
    while(1);
}

#ifndef __GNUC__
__attribute__((noreturn))
#endif
void main(void)
{
    NS_DRIVER_STDIO.Initialize(update_uart_cb);
    NS_DRIVER_STDIO.Control(ARM_USART_MODE_ASYNCHRONOUS, 115200);

    int x = 1;
    while(x);

    NS_DRIVER_STDIO.Send("yoo", 3);

    while(1);

    status = osKernelInitialize();

    /* Initialize the TFM NS lock */
    tfm_ns_lock_init();
    // thread_id = osThreadNew(blink, NULL, &blink_attr);
    thread_id = osThreadNew(busyloop, NULL, &blink_attr);
    //ua.blink_id = thread_id;
    //ua.blink_stack = (uint64_t *) &blink_stack;
    //osThreadNew(update_low_priority, &ua, &update_lp_attr);
    //osThreadNew(update_high_priority, NULL, &update_hp_attr);
    status = osKernelStart();

    /* Reached only in case of error */
    while(1);
}
