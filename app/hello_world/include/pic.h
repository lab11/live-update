#include <stdint.h>

#define PIC_FLASH_LINK_BASE 0x10000000

// github.com/lab11/pic-explorations and github.com/tock/libtock-c
struct pic_hdr {
    // Entry point to PIC main()
    uint32_t entry;
    // Offset to GOT in memory
    uint32_t got_start; 
    // GOT size
    uint32_t got_size;
    // App data gets linked at fixed address, must be updated during load
    uint32_t data_sym_start;
    // Data size
    uint32_t data_size;
    // Offset to the BSS in memory
    uint32_t bss_start;
    // BSS size
    uint32_t bss_size;
    // Relocation table offset
    uint32_t rel_start;
    // Relocation table size
    uint32_t rel_size;
};

