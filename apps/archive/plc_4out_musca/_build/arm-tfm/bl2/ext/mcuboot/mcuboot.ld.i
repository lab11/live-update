;
MEMORY
{
  FLASH (rx) : ORIGIN = ((0x10200000)), LENGTH = ((0x20000))
  CODE_RAM (rx) : ORIGIN = ((0x10000000)), LENGTH = ((0x20000))
  RAM (rwx) : ORIGIN = (((0x30000000) + (0x0))), LENGTH = ((0x00020000))
}
__heap_size__ = 0x0001000;
__msp_stack_size__ = 0x0001000;
GROUP(libgcc.a libc.a libm.a libnosys.a)
ENTRY(Reset_Handler)
SECTIONS
{
    .startup :
    {
        KEEP(*(.vectors))
        __Vectors_End = .;
        __Vectors_Size = __Vectors_End - __Vectors;
        __end__ = .;
        KEEP(*(.init))
        KEEP(*(.fini))
        *startup_cmsdk_musca_bl2.*
    } > FLASH
    .text :
    {
        KEEP(*(.vectors))
        __Vectors_End = .;
        __Vectors_Size = __Vectors_End - __Vectors;
        __end__ = .;
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
        KEEP(*(.eh_frame*))
        . = ALIGN(4);
    } > CODE_RAM AT > FLASH
    Image$$ER_CODE_SRAM$$Base = ADDR(.text);
    Image$$ER_CODE_SRAM$$Limit = .;
    Image$$ER_CODE_SRAM$$Length = Image$$ER_CODE_SRAM$$Limit - Image$$ER_CODE_SRAM$$Base;
    Load$$ER_CODE_SRAM$$Base = LOADADDR(.text);
    Load$$ER_CODE_SRAM$$Length = Image$$ER_CODE_SRAM$$Length;
    Load$$ER_CODE_SRAM$$Limit = Load$$ER_CODE_SRAM$$Base + Load$$ER_CODE_SRAM$$Length;
    .ARM.extab :
    {
        *(.ARM.extab* .gnu.linkonce.armextab.*)
    } > FLASH
    __exidx_start = .;
    .ARM.exidx :
    {
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
    } > FLASH
    __exidx_end = .;
    .copy.table :
    {
        . = ALIGN(4);
        __copy_table_start__ = .;
        LONG (__etext)
        LONG (__data_start__)
        LONG (__data_end__ - __data_start__)
        LONG (Load$$ER_CODE_SRAM$$Base)
        LONG (Image$$ER_CODE_SRAM$$Base)
        LONG (Image$$ER_CODE_SRAM$$Length)
        LONG (DEFINED(__etext2) ? __etext2 : 0)
        LONG (DEFINED(__data2_start__) ? __data2_start__ : 0)
        LONG (DEFINED(__data2_start__) ? __data2_end__ - __data2_start__ : 0)
        __copy_table_end__ = .;
    } > FLASH
    .zero.table :
    {
        . = ALIGN(4);
        __zero_table_start__ = .;
        LONG (__bss_start__)
        LONG (__bss_end__ - __bss_start__)
        LONG (DEFINED(__bss2_start__) ? __bss2_start__ : 0)
        LONG (DEFINED(__bss2_start__) ? __bss2_end__ - __bss2_start__ : 0)
        __zero_table_end__ = .;
    } > FLASH
    __etext = .;
    .tfm_bl2_shared_data : ALIGN(32)
    {
        . += 0x400;
    } > RAM
    Image$$SHARED_DATA$$RW$$Base = ADDR(.tfm_bl2_shared_data);
    Image$$SHARED_DATA$$RW$$Limit = ADDR(.tfm_bl2_shared_data) + SIZEOF(.tfm_bl2_shared_data);
    .data : AT (__etext)
    {
        __data_start__ = .;
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
        __data_end__ = .;
    } > RAM
    .bss :
    {
        . = ALIGN(4);
        __bss_start__ = .;
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        __bss_end__ = .;
    } > RAM
    bss_size = __bss_end__ - __bss_start__;
    .msp_stack : ALIGN(32)
    {
        . += __msp_stack_size__;
    } > RAM
    Image$$ARM_LIB_STACK$$ZI$$Limit = ADDR(.msp_stack) + SIZEOF(.msp_stack);
    .heap : ALIGN(8)
    {
        __end__ = .;
        PROVIDE(end = .);
        __HeapBase = .;
        . += __heap_size__;
        __HeapLimit = .;
        __heap_limit = .;
    } > RAM
    PROVIDE(__stack = Image$$ARM_LIB_STACK$$ZI$$Limit);
}
