/*
 * Copyright (c) 2017-2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

typedef struct {
    unsigned int* entry_loc;        /* Entry point for user application */
    unsigned int* init_data_loc;    /* Data initialization information in flash */ unsigned int init_data_size;    /* Size of initialization information */
    unsigned int got_start_offset;  /* Offset to start of GOT */
    unsigned int got_end_offset;    /* Offset to end of GOT */
    unsigned int plt_start_offset;  /* Offset to start of PLT */
    unsigned int plt_end_offset;    /* Offset to end of PLT */
    unsigned int bss_start_offset;  /* Offset to start of BSS */
    unsigned int bss_end_offset;    /* Offset to end of BSS */
    unsigned int rel_start_offset;  /* Offset to start of relocation table */
    unsigned int rel_end_offset;    /* Offset to end of relocation table */
} Load_Info; 
extern unsigned char *_apps;
extern unsigned char *_eapps;

#ifndef __GNUC__
__attribute__((noreturn))
#endif
void main(void)
{
    Load_Info *code_info = (Load_Info *)&_apps;

    int y = 1;
    while(y);

    unsigned int *flash_location = (unsigned int *)&_apps;
    unsigned int *sram_location = (unsigned int *)0x20018000; // use upper half of MUSCA NS on chip RAM
    // copy data into data section
    //  Data start location assumes .text starts at address 0, and needs to be
    //  updated to the actual location in flash
    unsigned int *init_data_src = (unsigned int *)((unsigned int)(*code_info).init_data_loc + (unsigned int)flash_location);
    unsigned int *init_data_end = (unsigned int *)((unsigned int)init_data_src + (*code_info).init_data_size);
    unsigned int *data_dst = sram_location;
    while (init_data_src < init_data_end) {
        *data_dst++ = *init_data_src++;
    }
    // zero out bss section
    unsigned int *bss_start = (unsigned int *)((unsigned int)sram_location + (*code_info).bss_start_offset);
    unsigned int bss_size = (*code_info).bss_end_offset - (*code_info).bss_start_offset;
    unsigned int *bss_end   = (unsigned int *)((unsigned int)bss_start + bss_size);
    while (bss_start < bss_end) {
        *bss_start++ = 0;
    }
    
    unsigned int flash_link_location = 0x10000000; // PIC app linked to flash at this location

    // apply relocations for data section, including GOT
    unsigned int *rel_start = (unsigned int *)((unsigned int)(*code_info).rel_start_offset + (unsigned int)flash_location);
    unsigned int *rel_end = (unsigned int *)((unsigned int)(*code_info).rel_end_offset + (unsigned int)flash_location);

    while (rel_start < rel_end) {
        unsigned int offset = *rel_start;

        unsigned int *target = 0x0;
        if (offset >= flash_link_location) { // resolve target relative to code SRAM
            target = (unsigned int *)((offset - flash_link_location) + (unsigned int)flash_location);
        } else { // resolve target relative to data SRAM
            target = (unsigned int *)((unsigned int)sram_location + offset);
        }

        // for some reason, the relocation table also includes Load_Info; just don't
        if ((unsigned int) target >= (unsigned int) code_info && (unsigned int) target < ((unsigned int) code_info + sizeof(Load_Info))) {
            rel_start += 2;
            continue;
        }

        unsigned int fixed_val = 0;
        if (*target >= flash_link_location) { // fixup offset relative to code SRAM
            fixed_val = (*target - flash_link_location) + (unsigned int)flash_location;
        } else { // fixup offset relative to internal data SRAM
            fixed_val = *target + (unsigned int)sram_location;
        }
        *target = fixed_val;

        rel_start += 2; // skip info, assume they're all R_ARM_RELATIVE entries (0x17)
    }

    /*
    // fixup GOT
    unsigned int *got_start = (unsigned int *)((unsigned int)sram_location + (*code_info).got_start_offset);
    unsigned int got_size = (*code_info).got_end_offset - (*code_info).got_start_offset;
    unsigned int *got_end   = (unsigned int *)((unsigned int)got_start + got_size);

    while (got_start < got_end) {
        unsigned int fixed_up_val = 0;
        if (*got_start >= flash_link_location) { // fixup offset relative to code SRAM location
            fixed_up_val = ((unsigned int)*got_start - flash_link_location) + (unsigned int)flash_location;
        } else { // fixup offset relative to internal data SRAM location
            fixed_up_val = *got_start + (unsigned int)sram_location;
        }
        *got_start++ = fixed_up_val;
    }
    */

    /* Context Switch */
    // set base register (r9)
    //  Needs to point to the beginning of the GOT section, which is at the
    //  start of the SRAM for the data section.
    __asm(" ldr     r9,=0x20018000      \n"
          );

    // Jump into application
    //  Entry location assumes .text starts at address 0, and needs to be
    //  updated to the actual location in flash
    unsigned int jmp = ((unsigned int)(*code_info).entry_loc + (unsigned int)flash_location);
    void (*fn)(void) = (void (*)(void))(((unsigned int)jmp) | 1); // |1 is very important!! Must stay in thumb mode

    fn();

    /* Do nothing */
    while(1);
}
