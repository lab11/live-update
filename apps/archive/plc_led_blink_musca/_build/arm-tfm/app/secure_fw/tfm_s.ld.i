;
MEMORY
{
  FLASH (rx) : ORIGIN = (((0x10000000) + ((((0x20000)) + (0x400))))), LENGTH = (((0x80000) - (0x400) - (0x400)) - (0x00000380))
  RAM (rwx) : ORIGIN = (((0x30000000) + (0x0))), LENGTH = ((0x00020000) / 2)
  VENEERS (rx) : ORIGIN = (((((0x10000000) + ((((0x20000)) + (0x400))))) + (((0x80000) - (0x400) - (0x400)) - (0x00000380)) - 1) + 1), LENGTH = (0x00000380)
}
__heap_size__ = 0x0001000;
__psp_stack_size__ = 0x0000800;
__msp_init_stack_size__ = 0x0000400;
GROUP(libgcc.a libc.a libm.a libnosys.a libc_nano.a)
ENTRY(Reset_Handler)
EXTERN(tfm_secure_client_service_veneer_run_tests)
SECTIONS
{
    .TFM_VECTORS : ALIGN(4)
    {
        __vectors_start__ = .;
        KEEP(*(.vectors))
        *startup*(.text*)
        . = ALIGN(4);
        __vectors_end__ = .;
    } > FLASH
    .copy.table : ALIGN(4)
    {
        __copy_table_start__ = .;
        LONG (LOADADDR(.TFM_DATA))
        LONG (ADDR(.TFM_DATA))
        LONG (SIZEOF(.TFM_DATA))
        __copy_table_end__ = .;
    } > FLASH
    .zero.table : ALIGN(4)
    {
        __zero_table_start__ = .;
        LONG (ADDR(.TFM_BSS))
        LONG (SIZEOF(.TFM_BSS))
        LONG (ADDR(.TFM_SECURE_STACK))
        LONG (SIZEOF(.TFM_SECURE_STACK))
        LONG (ADDR(.TFM_UNPRIV_SCRATCH))
        LONG (SIZEOF(.TFM_UNPRIV_SCRATCH))
        __zero_table_end__ = .;
    } > FLASH
    .ER_TFM_CODE : ALIGN(4)
    {
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
    } > FLASH
    .tfm_bl2_shared_data : ALIGN(32)
    {
        . += 0x400;
    } > RAM
    .msp_stack : ALIGN(32)
    {
        . += __msp_init_stack_size__;
    } > RAM
    Image$$ARM_LIB_STACK_MSP$$ZI$$Limit = ADDR(.msp_stack) + SIZEOF(.msp_stack);
    .psp_stack : ALIGN(32)
    {
        . += __psp_stack_size__;
    } > RAM
    Image$$ARM_LIB_STACK$$ZI$$Base = ADDR(.psp_stack);
    Image$$ARM_LIB_STACK$$ZI$$Limit = ADDR(.psp_stack) + SIZEOF(.psp_stack);
    .TFM_SECURE_STACK : ALIGN(128)
    {
        . += 0x2000;
    } > RAM
    Image$$TFM_SECURE_STACK$$ZI$$Base = ADDR(.TFM_SECURE_STACK);
    Image$$TFM_SECURE_STACK$$ZI$$Limit = ADDR(.TFM_SECURE_STACK) + SIZEOF(.TFM_SECURE_STACK);
    .TFM_UNPRIV_SCRATCH : ALIGN(32)
    {
        . += 0x400;
    } > RAM
    Image$$TFM_UNPRIV_SCRATCH$$ZI$$Base = ADDR(.TFM_UNPRIV_SCRATCH);
    Image$$TFM_UNPRIV_SCRATCH$$ZI$$Limit = ADDR(.TFM_UNPRIV_SCRATCH) + SIZEOF(.TFM_UNPRIV_SCRATCH);
    .heap : ALIGN(8)
    {
        __end__ = .;
        PROVIDE(end = .);
        __HeapBase = .;
        . += __heap_size__;
        __HeapLimit = .;
        __heap_limit = .;
    } > RAM
    .TFM_DATA : ALIGN(4)
    {
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
    } > RAM AT> FLASH
    Image$$ER_TFM_DATA$$RW$$Base = ADDR(.TFM_DATA);
    Image$$ER_TFM_DATA$$RW$$Limit = ADDR(.TFM_DATA) + SIZEOF(.TFM_DATA);
    .TFM_BSS : ALIGN(4)
    {
        __bss_start__ = .;
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        __bss_end__ = .;
    } > RAM
    Image$$ER_TFM_DATA$$ZI$$Base = ADDR(.TFM_BSS);
    Image$$ER_TFM_DATA$$ZI$$Limit = ADDR(.TFM_BSS) + SIZEOF(.TFM_BSS);
    Image$$ER_TFM_DATA$$Base = ADDR(.TFM_DATA);
    Image$$ER_TFM_DATA$$Limit = ADDR(.TFM_DATA) + SIZEOF(.TFM_DATA) + SIZEOF(.TFM_BSS);
    .gnu.sgstubs : ALIGN(32)
    {
        *(.gnu.sgstubs*)
        . = ALIGN(32);
    } > VENEERS AT> VENEERS
    Load$$LR$$LR_VENEER$$Base = ADDR(.gnu.sgstubs);
    Load$$LR$$LR_VENEER$$Limit = ADDR(.gnu.sgstubs) + SIZEOF(.gnu.sgstubs);
    Load$$LR$$LR_NS_PARTITION$$Base = (((0x00000000) + ((((0x20000)) + (0x80000)))));
    Load$$LR$$LR_SECONDARY_PARTITION$$Base = (((0x00000000) + (((0x120000)))));
    PROVIDE(__stack = Image$$ARM_LIB_STACK$$ZI$$Limit);
}
