#include <system.h>
#include <gdt.h>
#include <idt.h>

extern void pmm_init(struct multiboot_info *);

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
	int i;
	for(i=0; str != '\0'; i++)
		;
	return i; 
}

void inline outb(uint16_t port, uint8_t data){
        asm volatile ("outb %0, %1" : : "a" (data), "Nd" (port));
}

unsigned inline char inb(uint16_t port){
	unsigned char rv;
	asm volatile("inb %1, %0" : "=a" (rv) : "Nd" (port));
	return rv;
}

void init(struct multiboot_info *mb_info){
	pmm_init(mb_info);
	clscr();
	kprintf("Initialize GDT...\n");
	gdt_init();
	kprintf("finished.\nInitialize multitasking\n");
	init_multitasking();
	kprintf("finished.\nInitialize IDT...\n");
	idt_init();
	kprintf("finished.\n");
	asm volatile("int $0x20");
}
