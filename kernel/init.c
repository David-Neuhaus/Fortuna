#include <system.h>
#include <gdt.h>
#include <idt.h>
#include <pmm.h>

uint8_t *memcpy(uint8_t *dest, const uint8_t *src, int count){
    int i;
    for(i=0; i<count; i++)
        dest[i] = src[i];
    return dest; 
}

uint8_t *memset(uint8_t *dest, uint8_t val, int count){
    int i;
    for(i=0; i<count; i++)
        dest[i] = val;
    return dest;
}

uint16_t *memsetw(uint16_t *dest, uint16_t val, int count){
    int i;
    for(i=0; i<count; i++)
        dest[i] = val;
    return dest;
}

int strlen(const char *str){
    const char *e;
    while(*str++);
    return (int) (e - str -1); 
}

void inline outb(uint16_t port, uint8_t data){
        asm volatile ("outb %0, %1" : : "a" (data), "Nd" (port));
}

unsigned inline char inb(uint16_t port){
    unsigned char rv;
    asm volatile("inb %1, %0" : "=a" (rv) : "Nd" (port));
    return rv;
}

void init(unsigned long mb_info, unsigned long mn){
    if(mn == 0xE85250D6)
        MB_NEW = 1;
    else
        MB_OLD = 1;
    pmm_init((void *) mb_info);
    kterminal_init();
    kprintf("Initialize GDT...\n");
    gdt_init();
    kprintf("finished.\nInitialize multitasking\n");
    init_multitasking();
    kprintf("finished.\nInitialize IDT...\n");
    idt_init();
    kprintf("finished.\n");
    asm volatile("int $0x20");
}
