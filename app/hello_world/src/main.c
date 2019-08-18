// PIC app that doesn't include sys/printk.h
#include "interface.h"

void main(void) {
    printk("Hello, world!\n");
}

