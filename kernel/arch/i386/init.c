#include <kernel/console.h>
#include <kernel/gdt.h>
#include <kernel/idt.h>
#include <kernel/pmm.h>
#include <kernel/multiboot.h>

#include <stdio.h>

/*Multiboot version */
int MB_OLD = 0;
int MB_NEW = 0;

extern void kernel_main(void);

void init(unsigned long mb_info, unsigned long mn){
    if(mn == 0x36d76289)
        MB_NEW = 1;
    else
        MB_OLD = 1;
    pmm_init((void *) mb_info);
    kterminal_init();
    printf("Initialize GDT...\n");
    gdt_init();
    printf("finished.\nInitialize IDT...\n");
    idt_init();
    printf("finished.\n");
    asm volatile("int $0x20");
    kernel_main();
}
