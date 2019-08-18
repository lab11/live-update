/*
 * Copyright (c) 2017-2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stdio.h>
#include <string.h>
#include <arm_cmse.h>

#include "secure_utilities.h"
#include "tfm_svc.h"
#include "region_defs.h"
#include "tfm_api.h"
#include "tfm_internal.h"
#include "tfm_memory_utils.h"
#ifdef TFM_PSA_API
#include <stdbool.h>
#include "tfm_svcalls.h"
#endif

/* This SVC handler is called when a secure partition requests access to a
 * buffer area
 */
extern int32_t tfm_core_set_buffer_area_handler(const uint32_t args[]);
#ifdef TFM_PSA_API
extern void tfm_psa_ipc_request_handler(const uint32_t svc_args[]);
#endif

struct tfm_fault_context_s {
    uint32_t R0;
    uint32_t R1;
    uint32_t R2;
    uint32_t R3;
    uint32_t R12;
    uint32_t LR;
    uint32_t ReturnAddress;
    uint32_t RETPSR;
} tfm_fault_context;

#if defined(__ARM_ARCH_8M_MAIN__)
/**
 * \brief Overwrites default Secure fault handler.
 */
void SecureFault_Handler(void)
{
    /* figure out context from which we landed in fault handler */
    uint32_t lr = __get_LR();
    uint32_t sp;

    if (lr & EXC_RETURN_SECURE_STACK) {
        if (lr & EXC_RETURN_STACK_PROCESS) {
            sp = __get_PSP();
        } else {
            sp = __get_MSP();
        }
    } else {
        if (lr & EXC_RETURN_STACK_PROCESS) {
            sp =  __TZ_get_PSP_NS();
        } else {
            sp = __TZ_get_MSP_NS();
        }
    }

    /* Only save the context if sp is valid */
    if ((sp >=  S_DATA_START &&
         sp <=  (S_DATA_LIMIT - sizeof(tfm_fault_context)) + 1) ||
        (sp >= NS_DATA_START &&
         sp <= (NS_DATA_LIMIT - sizeof(tfm_fault_context)) + 1)) {
        tfm_memcpy(&tfm_fault_context,
                   (const void *)sp,
                   sizeof(tfm_fault_context));
    }

    char buf[200];

    sprintf(buf, "in secure mode: %d", *((uint32_t *) 0xE000EDD0) != 0);
    LOG_MSG(buf);

    cmse_address_info_t i;
    i = cmse_TT((uint32_t *)0xa2284);
    sprintf(buf, "TT(0xa2284): value: %x\n\tsau_region=%d\n\tsau_region_valid=%d\n\tread_ok=%d\n\treadwrite_ok=%d\n\tns_read_ok=%d\n\tns_readwrite_ok=%d\n\tsecure=%d",
            i.value,
            i.flags.sau_region,
            i.flags.sau_region_valid,
            i.flags.read_ok,
            i.flags.readwrite_ok,
            i.flags.nonsecure_read_ok,
            i.flags.nonsecure_readwrite_ok,
            i.flags.secure
    );
    LOG_MSG(buf);

    i = cmse_TT((uint32_t *)0xa39d1);
    sprintf(buf, "TT(0xa39d1): value: %x\n\tsau_region=%d\n\tsau_region_valid=%d\n\tread_ok=%d\n\treadwrite_ok=%d\n\tns_read_ok=%d\n\tns_readwrite_ok=%d\n\tsecure=%d",
            i.value,
            i.flags.sau_region,
            i.flags.sau_region_valid,
            i.flags.read_ok,
            i.flags.readwrite_ok,
            i.flags.nonsecure_read_ok,
            i.flags.nonsecure_readwrite_ok,
            i.flags.secure
    );
    LOG_MSG(buf);



    LOG_MSG("Oops... Secure fault!!! You're not going anywhere!");
    LOG_MSG("---- SAU Registers ----");


    uint32_t sau_ctrl = *((uint32_t *)0xE000EDD4);
    sprintf(buf, "SAU_CTRL: %x", sau_ctrl);
    LOG_MSG(buf);
    sprintf(buf, "\tALLNS:  %d", (sau_ctrl >> 1) & 1);
    LOG_MSG(buf);
    sprintf(buf, "\tENABLE: %d", (sau_ctrl >> 0) & 1);
    LOG_MSG(buf);

    uint32_t sau_sfsr = *((uint32_t *)0xE000EDE4);
    sprintf(buf, "SFSR: %x", sau_sfsr);
    LOG_MSG(buf);
    sprintf(buf, "\tLSERR:     %d", (sau_sfsr >> 7) & 1);
    LOG_MSG(buf);
    sprintf(buf, "\tSFARVALID: %d", (sau_sfsr >> 6) & 1);
    LOG_MSG(buf);
    sprintf(buf, "\tLSPERR:    %d", (sau_sfsr >> 5) & 1);
    LOG_MSG(buf);
    sprintf(buf, "\tINVTRAN:   %d", (sau_sfsr >> 4) & 1);
    LOG_MSG(buf);
    sprintf(buf, "\tAUVIOL:    %d", (sau_sfsr >> 3) & 1);
    LOG_MSG(buf);
    sprintf(buf, "\tINVER:     %d", (sau_sfsr >> 2) & 1);
    LOG_MSG(buf);
    sprintf(buf, "\tINVIS:     %d", (sau_sfsr >> 1) & 1);
    LOG_MSG(buf);
    sprintf(buf, "\tINVEP:     %d", (sau_sfsr >> 0) & 1);
    LOG_MSG(buf);

    uint32_t sau_sfar = *((uint32_t *)0xE000EDE0);
    sprintf(buf, "SFAR: %x", sau_sfar);
    LOG_MSG(buf);

    uint32_t sau_type = *((uint32_t *)0xE000EDD4);
    sprintf(buf, "SAU_TYPE: %x", sau_type);
    LOG_MSG(buf);
    sprintf(buf, "\tSREGION: %d", (sau_type >> 0) & 0xff);
    LOG_MSG(buf);

    // Iterate through regions
    uint32_t *sau_rnr = (uint32_t *)0xE000EDD8; 
    uint32_t *sau_rbar = (uint32_t *)0xE000EDDC;
    uint32_t *sau_rlar = (uint32_t *)0xE000EDE0;

    for(int r = 0; r < (sau_type & 0xff); r++) {
        sprintf(buf, "\t-- Region %d --", r);
        LOG_MSG(buf);

        *sau_rnr = r;
        
        sprintf(buf, "\t\tSAU_RBAR: %x", *sau_rbar);
        LOG_MSG(buf);
        sprintf(buf, "\t\t\tBADDR: %x", *sau_rbar & 0xFFFFFFE0);   
        LOG_MSG(buf);

        sprintf(buf, "\t\tSAU_RLAR: %x", *sau_rlar);
        LOG_MSG(buf);
        sprintf(buf, "\t\t\tLADDR:  %x", (*sau_rlar & 0xFFFFFFE0) | 0x1F);   
        LOG_MSG(buf);
        sprintf(buf, "\t\t\tNSC:    %x", ((*sau_rlar) >> 1) & 1);
        LOG_MSG(buf);
        sprintf(buf, "\t\t\tENABLE: %x", ((*sau_rlar) >> 0) & 1);
        LOG_MSG(buf);
    }

    while (1) {
        ;
    }
}
#elif defined(__ARM_ARCH_8M_BASE__)
/**
 * \brief Overwrites default Hard fault handler.
 *
 * In case of a baseline implementation fault conditions that would generate a
 * SecureFault in a mainline implementation instead generate a Secure HardFault.
 */
void HardFault_Handler(void)
{
    /* In a baseline implementation there is no way, to find out whether this is
     * a hard fault triggered directly, or another fault that has been
     * escalated.
     */
    LOG_MSG("--> HardFault");
    while (1) {
        ;
    }
}

void BusFault_Handler(void)
{
    LOG_MSG("--> BusFault");
    while (1) {
        ;
    }
}
#else
#error "Unsupported ARM Architecture."
#endif

#if defined(__ARM_ARCH_8M_MAIN__)
__attribute__((naked)) void SVC_Handler(void)
{
    __ASM volatile(
    "TST     lr, #4\n"  /* Check store SP in thread mode to r0 */
    "IT      EQ\n"
    "BXEQ    lr\n"
    "MRS     r0, PSP\n"
    "MOV     r1, lr\n"
    "BL      SVCHandler_main\n"
    "BX      r0\n"
    );
}
#elif defined(__ARM_ARCH_8M_BASE__)
__attribute__((naked)) void SVC_Handler(void)
{
    __ASM volatile(
    ".syntax unified\n"
    "MOVS    r0, #4\n"  /* Check store SP in thread mode to r0 */
    "MOV     r1, lr\n"
    "TST     r0, r1\n"
    "BEQ     handler\n"
    "MRS     r0, PSP\n"  /* Coming from thread mode */
    "B sp_stored\n"
    "handler:\n"
    "BX      lr\n"  /* Coming from handler mode */
    "sp_stored:\n"
    "MOV     r1, lr\n"
    "BL      SVCHandler_main\n"
    "BX      r0\n"
    );
}
#else
#error "Unsupported ARM Architecture."
#endif

uint32_t SVCHandler_main(uint32_t *svc_args, uint32_t lr)
{
    uint8_t svc_number;
    /*
     * Stack contains:
     * r0, r1, r2, r3, r12, r14 (lr), the return address and xPSR
     * First argument (r0) is svc_args[0]
     */
    if (lr & EXC_RETURN_SECURE_STACK) {
        /* SV called directly from secure context. Check instruction for
         * svc_number
         */
        svc_number = ((uint8_t *)svc_args[6])[-2];
    } else {
        /* Secure SV executing with NS return.
         * NS cannot directly trigger S SVC so this should not happen
         * FixMe: check for security implications
         */
        return lr;
    }
    switch (svc_number) {
    case TFM_SVC_SFN_REQUEST:
        //lr = tfm_core_partition_request_svc_handler(svc_args, lr);
        break;
    case TFM_SVC_SFN_RETURN:
        //lr = tfm_core_partition_return_handler(lr);
        break;
    case TFM_SVC_VALIDATE_SECURE_CALLER:
        //tfm_core_validate_secure_caller_handler(svc_args);
        break;
    case TFM_SVC_GET_CALLER_CLIENT_ID:
        //tfm_core_get_caller_client_id_handler(svc_args);
        break;
    case TFM_SVC_SPM_REQUEST:
        tfm_core_spm_request_handler((struct tfm_exc_stack_t *)svc_args);
        break;
    case TFM_SVC_MEMORY_CHECK:
        //tfm_core_memory_permission_check_handler(svc_args);
        break;
    case TFM_SVC_SET_SHARE_AREA:
        //tfm_core_set_buffer_area_handler(svc_args);
        break;
#ifdef TFM_PSA_API
    case TFM_SVC_IPC_REQUEST:
        tfm_psa_ipc_request_handler(svc_args);
        break;
#endif
    case TFM_SVC_PRINT:
        printf("\e[1;34m[Sec Thread] %s\e[0m\r\n", (char *)svc_args[0]);
        break;
    case TFM_SVC_GET_BOOT_DATA:
        tfm_core_get_boot_data_handler(svc_args);
        break;
#ifdef TFM_PSA_API
    case TFM_SVC_SCHEDULE:
    case TFM_SVC_EXIT_THRD:
    case TFM_SVC_PSA_FRAMEWORK_VERSION:
    case TFM_SVC_PSA_VERSION:
    case TFM_SVC_PSA_CONNECT:
    case TFM_SVC_PSA_CALL:
    case TFM_SVC_PSA_CLOSE:
    case TFM_SVC_PSA_WAIT:
    case TFM_SVC_PSA_GET:
    case TFM_SVC_PSA_SET_RHANDLE:
    case TFM_SVC_PSA_READ:
    case TFM_SVC_PSA_SKIP:
    case TFM_SVC_PSA_WRITE:
    case TFM_SVC_PSA_REPLY:
    case TFM_SVC_PSA_NOTIFY:
    case TFM_SVC_PSA_CLEAR:
    case TFM_SVC_PSA_EOI:
        svc_args[0] = SVC_Handler_IPC(svc_number, svc_args, lr);
        break;
#endif
    default:
        LOG_MSG("Unknown SVC number requested!");
        break;
    }

    return lr;
}

void tfm_access_violation_handler(void)
{
    while (1) {
        ;
    }
}
