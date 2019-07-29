;/*
; * Copyright (c) 2018 ARM Limited
; *
; * Licensed under the Apache License, Version 2.0 (the "License");
; * you may not use this file except in compliance with the License.
; * You may obtain a copy of the License at
; *
; *     http://www.apache.org/licenses/LICENSE-2.0
; *
; * Unless required by applicable law or agreed to in writing, software
; * distributed under the License is distributed on an "AS IS" BASIS,
; * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
; * See the License for the specific language governing permissions and
; * limitations under the License.
; *
; *
; * This file is derivative of CMSIS V5.00 gcc_arm.ld
; */

/* Linker script to configure memory regions. */
/* This file will be run trough the pre-processor. */

MEMORY
{
  FLASH (rx)  : ORIGIN = 0x10000000, LENGTH = 0x00080000
  RAM   (rwx) : ORIGIN = 0x00000000, LENGTH = 0x00008000
}

ENTRY(main)

SECTIONS
{

    .pic_hdr :
    {
        /* Generate the PIC app header for the loader */
        _flash_base = .;
        LONG(0x0);                  /* uint32_t entry (set by static relocation) */
        LONG(_got);                 /* uint32_t got_start      */
        LONG(SIZEOF(.got));         /* uint32_t got_size       */
        LONG(_etext - _flash_base); /* uint32_t data_sym_start */
        LONG(_edata);               /* uint32_t data_size      */
        LONG(_bss);                 /* uint32_t bss_start      */
        LONG(_ebss - _bss);         /* uint32_t bss_size       */
        LONG(_rel - _flash_base);   /* uint32_t rel_start      */
        LONG(_erel - _rel);         /* uint32_t rel_size       */
    } > FLASH

    .text :
    {
        _text = .;
        *(.text*)

        KEEP(*(.init))
        KEEP(*(.fini))

        *crtbegin.o(.ctors)
        *crtbegin?.o(.ctors)
        *(EXCLUDE_FILE(*crtend?.o *crtend.o) .ctors)
        *(SORT(.ctors.*))
        *(.ctors)

        *crtbegin.o(.dtors)
        *crtbegin?.o(.dtors)
        *(EXCLUDE_FILE(*crtend?.o *crtend.o) .dtors)
        *(SORT(.dtors.*))
        *(.dtors)

        *(.rodata*)
        _etext = .;
    } > FLASH

    .got :
    {
        _got = .;
        *(.got*)
        _egot = .;
        /*
        _plt = .;
        *(.plt*)
        _eplt = .;
        */
    } > RAM AT > FLASH

    .plt :
    {
        _plt = .;
        *(.plt*)
        _eplt = .;
    } > RAM AT > FLASH

    .data :
    {
        _data = .;
        *(vtable)
        *(.data*)

        . = ALIGN(4);
        PROVIDE_HIDDEN (__preinit_array_start = .);
        KEEP(*(.preinit_array))
        PROVIDE_HIDDEN (__preinit_array_end = .);

        . = ALIGN(4);
        PROVIDE_HIDDEN (__init_array_start = .);
        KEEP(*(SORT(.init_array.*)))
        KEEP(*(.init_array))
        PROVIDE_HIDDEN (__init_array_end = .);

        . = ALIGN(4);
        PROVIDE_HIDDEN (__fini_array_start = .);
        KEEP(*(SORT(.fini_array.*)))
        KEEP(*(.fini_array))
        PROVIDE_HIDDEN (__fini_array_end = .);

        KEEP(*(.jcr*))
        . = ALIGN(4);
        _edata = .;
    } > RAM AT > FLASH

    .bss :
    {
        . = ALIGN(4);
        _bss = .;
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        _ebss = .;
    } > RAM

    /* Manually specify these sections so they go last */
    .interp :
    {
        *(.interp*)
    } > FLASH

    .dynsym :
    {
        *(.dynsym*)
    } > FLASH

    .dynstr :
    {
        *(.dynstr*)
    } > FLASH

    .hash :
    {
        *(.hash*)
    } > FLASH

    .ARM.exidx :
    {
        *(.ARM.exidx*)
    } > FLASH

    /* Relocations */
    .rel.dyn :
    {
        _rel = .;
        *(.rel*)
        _erel = .;
    } > FLASH
}
