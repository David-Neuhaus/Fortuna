#include <kernel/idt.h>
#include <stdint.h>
#include <stdio.h>

static uint64_t idt[IDT_ENTRIES];

static void init_idt(void);
static void load_idt(void);
static void init_pic(void);

static void set_entry(int i, uint32_t offs, uint16_t selector, uint8_t flags){
	idt[i] = offs & 0xffffLL;
	idt[i] |= (selector & 0xffffLL) << 16;
	idt[i] |= (0x00 & 0xffLL) << 32;
	idt[i] |= (flags & 0xffLL) << 40;
	idt[i] |= ((offs >> 16) & 0xffffLL) << 48;
}

void idt_init(void){
	init_idt();
	init_pic();
}

static void init_idt(void){
    set_entry(0, (uint32_t) intr_stub_0, 0x08, 0x8E);
    set_entry(1, (uint32_t) intr_stub_1, 0x08, 0x8E);
    set_entry(2, (uint32_t) intr_stub_2, 0x08, 0x8E);
    set_entry(3, (uint32_t) intr_stub_3, 0x08, 0x8E);
    set_entry(4, (uint32_t) intr_stub_4, 0x08, 0x8E);
    set_entry(5, (uint32_t) intr_stub_5, 0x08, 0x8E);
    set_entry(6, (uint32_t) intr_stub_6, 0x08, 0x8E);
    set_entry(7, (uint32_t) intr_stub_7, 0x08, 0x8E);
    set_entry(8, (uint32_t) intr_stub_8, 0x08, 0x8E);
    set_entry(9, (uint32_t) intr_stub_9, 0x08, 0x8E);
    set_entry(10, (uint32_t) intr_stub_10, 0x08, 0x8E);
    set_entry(11, (uint32_t) intr_stub_11, 0x08, 0x8E);
    set_entry(12, (uint32_t) intr_stub_12, 0x08, 0x8E);
    set_entry(13, (uint32_t) intr_stub_13, 0x08, 0x8E);
    set_entry(14, (uint32_t) intr_stub_14, 0x08, 0x8E);
    set_entry(15, (uint32_t) intr_stub_15, 0x08, 0x8E);
    set_entry(16, (uint32_t) intr_stub_16, 0x08, 0x8E);
    set_entry(17, (uint32_t) intr_stub_17, 0x08, 0x8E);
    set_entry(18, (uint32_t) intr_stub_18, 0x08, 0x8E);
    set_entry(32, (uint32_t) intr_stub_32, 0x08, 0x8E);
    set_entry(33, (uint32_t) intr_stub_33, 0x08, 0x8E);
    set_entry(34, (uint32_t) intr_stub_34, 0x08, 0x8E);
    set_entry(35, (uint32_t) intr_stub_35, 0x08, 0x8E);
    set_entry(36, (uint32_t) intr_stub_36, 0x08, 0x8E);
    set_entry(37, (uint32_t) intr_stub_37, 0x08, 0x8E);
    set_entry(38, (uint32_t) intr_stub_38, 0x08, 0x8E);
    set_entry(39, (uint32_t) intr_stub_39, 0x08, 0x8E);
    set_entry(40, (uint32_t) intr_stub_40, 0x08, 0x8E);
    set_entry(41, (uint32_t) intr_stub_41, 0x08, 0x8E);
    set_entry(42, (uint32_t) intr_stub_42, 0x08, 0x8E);
    set_entry(43, (uint32_t) intr_stub_43, 0x08, 0x8E);
    set_entry(44, (uint32_t) intr_stub_44, 0x08, 0x8E);
    set_entry(45, (uint32_t) intr_stub_45, 0x08, 0x8E);
    set_entry(46, (uint32_t) intr_stub_46, 0x08, 0x8E);
    set_entry(47, (uint32_t) intr_stub_47, 0x08, 0x8E);
    set_entry(48, (uint32_t) intr_stub_48, 0x08, 0x8E);
	
    load_idt();
}

static void load_idt(void){
	struct {
		uint16_t limit;
		void* pointer;
	} __attribute__((packed)) idtp = {
		.limit = IDT_ENTRIES * 8 - 1,
		.pointer = idt
	};

	idtp.pointer -= 0xC0000000; //higher kernel, we have to calculate it outside the definition, otherwise gcc optimizes it away somehow
	asm volatile("lidt %0" : : "m" (idtp));
}

static void init_pic(void){
    /* save masks */
    unsigned char a1, a2;
    a1 = inb(0x21);
    a2 = inb(0xa1);

    /* Master-PIC */
	outb(0x20, 0x11); //Initialise PIC
	outb(0x21, 0x20); //Interrupt number of IRQ 0
	outb(0x21, 0x04); //Slave on IRQ 2
	outb(0x21, 0x01); //ICW4

    /* Slave-PIc */
	outb(0xa0, 0x11); //Initialise PIC
	outb(0xa1, 0x28); //Interrupt number of IRQ 8
	outb(0xa1, 0x02); //Slave on IRQ 2
	outb(0xa1, 0x01); //ICW4

    /* activate all IRQs */
	outb(0x20, 0x0);
	outb(0xa0, 0x0);

    /* restore saved masks */
    outb(0x21, a1);
    outb(0xa1, a2);

	asm volatile("sti");
}
