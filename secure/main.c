/*
 * secure/main.c
 *
 * Main secure-side software. Currently boots out of the bootloader and hands
 * off to the non-secure application
 */

#include "secure_utilities.h"
#include "tfm_api.h"
#include "tfm_core.h"
#include "tfm_internal.h"
#include "tfm_spm_hal.h"
#include "uart_stdout.h"

#undef cmse_nsfptr_create
#define cmse_nsfptr_create(p) ((intptr_t) (p) & ~1)

#define REGION(a, b, c) a##b##c
#define REGION_NAME(a, b, c) REGION(a, b, c)
#define REGION_DECLARE(a, b, c) extern uint32_t REGION_NAME(a, b, c)

REGION_DECLARE(Image$$, TFM_UNPRIV_SCRATCH, $$ZI$$Base);
REGION_DECLARE(Image$$, TFM_UNPRIV_SCRATCH, $$ZI$$Limit);

void configure_ns_code(void) {
    SCB_NS->VTOR = tfm_spm_hal_get_ns_VTOR();
    uint32_t ns_msp = tfm_spm_hal_get_ns_MSP();
    __TZ_set_MSP_NS(ns_msp);

    uint32_t entry_ptr = tfm_spm_hal_get_ns_entry_point();
    ns_entry = (nsfptr_t) cmse_nsfptr_create(entry_ptr);
}

int32_t tfm_core_init(void) {

    enable_fault_handlers();

    system_reset_cfg();

    tfm_spm_hal_init_debug();

    __enable_irq();

    stdio_init();

    tfm_core_validate_boot_data();
    
    tfm_spm_hal_init_isolation_hw();

    configure_ns_code();

    nvic_interrupt_target_state_cfg();
    nvic_interrupt_enable();

    tfm_scratch_area = (uint8_t *)&REGION_NAME(Image$$, TFM_UNPRIV_SCRATCH, $$ZI$$Base);
    tfm_scratch_area_size = (uint32_t)&REGION_NAME(Image$$, TFM_UNPRIV_SCRATCH, $$ZI$$Base);
    return 0;
}

static int32_t tfm_core_set_secure_exception_priorities(void) {
    uint32_t VECTKEY;
    SCB_Type *scb = SCB;
    uint32_t AIRCR;

    /* Set PRIS flag is AIRCR */
    AIRCR = scb-> AIRCR;
    VECTKEY = (~AIRCR & SCB_AIRCR_VECTKEYSTAT_Msk);
    scb->AIRCR = SCB_AIRCR_PRIS_Msk |
        VECTKEY |
        (AIRCR & ~SCB_AIRCR_VECTKEY_Msk);

    /* FixMe: Explicitly set secure fault and Secure SVC priority to highest */

    /*
     * Set secure PendSV priority to the lowest in SECURE state.
     *
     * IMPORTANT NOTE:
     *
     * Although the priority of the secure PendSV must be the lowest possible
     * among other interrupts in the Secure state, it must be ensured that
     * PendSV is not preempted nor masked by Non-Secure interrupts to ensure
     * the integrity of the Secure operation. When AIRCR.PRIS is set, the
     * Non-Secure execution can act on FAULTMASK_NS, PRIMASK_NS or BASEPRI_NS
     * register to boost its priority number up to the value 0x80. For this
     * reason, set the priority of the PendSV interrupt to the next priority
     * level configurable on the platform, just below 0x80.
     */

    NVIC_SetPriority(PendSV_IRQn, (1 << (__NVIC_PRIO_BITS -1 )) - 1);

    return TFM_SUCCESS;
}

int main() {
    tfm_core_init();

    tfm_spm_hal_setup_isolation_hw();
    //tfm_spm_partition_set_state(TFM_SP_CORE_ID, SPM_PARTITION_STATE_RUNNING);

    extern uint32_t Image$$ARM_LIB_STACK$$ZI$$Base[];
    uint32_t psp_stack_bottom = (uint32_t)Image$$ARM_LIB_STACK$$ZI$$Base;

    __set_PSPLIM(psp_stack_bottom);

    tfm_core_set_secure_exception_priorities();

    //tfm_spm_partition_set_state(TFM_SP_CORE_ID, SPM_PARTITION_STATE_CLOSE);
    //tfm_spm_partition_set_state(TFM_SP_NON_SECURE_ID, SPM_PARTITION_STATE_RUNNING);

    LOG_MSG("Hi!");
    jump_to_ns_code();
}
