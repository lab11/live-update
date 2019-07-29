// PIC app that doesn't include sys/printk.h

extern void (*printk)(const char *fmt, ...);

void main(void)
{
    int b = 1;
    while (1);

    for (int a = b; a < 10; a++) {
	    printk("a: %d\n", a);
    }
}

