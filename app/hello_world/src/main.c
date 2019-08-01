// PIC app that doesn't include sys/printk.h

extern void (*printk)(const char *fmt, ...);

void main(void) {
    printk("testing this PIC app...\n");

    while(1);
}

