/* hello.c */
#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("Dual BSD/GPL");

int init_module(void)
{
#ifdef __x86_64__
    u64 cr0, cr2, cr3, cr4;
    __asm__ __volatile__ (
        "mov %%cr0, %%rax\n\t"
        "mov %%rax, %0\n\t"
        "mov %%cr2, %%rax\n\t"
        "mov %%rax, %1\n\t"
        "mov %%cr3, %%rax\n\t"
        "mov %%rax, %2\n\t"
        "mov %%cr4, %%rax\n\t"
        "mov %%rax, %3\n\t"
    : "=m" (cr0), "=m" (cr2), "=m" (cr3), "=m" (cr4)
    : /* no input */
    : "%rax"
    );
#elif defined(__i386__)
    u32 cr0, cr2, cr3, cr4;
    __asm__ __volatile__ (
        "mov %%cr0, %%eax\n\t"
        "mov %%eax, %0\n\t"
        "mov %%cr2, %%eax\n\t"
        "mov %%eax, %1\n\t"
        "mov %%cr3, %%eax\n\t"
        "mov %%eax, %2\n\t"
        "mov %%cr4, %%eax\n\t"
        "mov %%eax, %3\n\t"
    : "=m" (cr0), "=m" (cr2), "=m" (cr3), "=m" (cr4)
    : /* no input */
    : "%eax"
    );
#endif
    printk(KERN_INFO "lbn11 cr0 = 0x%016llx\n", cr0);
    printk(KERN_INFO "lbn11 cr2 = 0x%016llx\n", cr2);
    printk(KERN_INFO "lbn11 cr3 = 0x%016llx\n", cr3);
    printk(KERN_INFO "lbn11 cr4 = 0x%016llx\n", cr4);
    return 0;
}

void cleanup_module(void)
{
}

/*
# Makefile

obj-m += hello.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

test: all
	sudo insmod ./hello.ko
	sudo rmmod hello
	sudo dmesg | tail
*/
