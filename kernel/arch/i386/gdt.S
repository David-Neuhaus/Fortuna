.section .bss

early_gdt:
.fill 3, 8

early_gdtp:
.skip 6

.section .text

#include <kernel/gdt.h>

.macro set_entry i, base, limit, flags
	movl $((\i * 8) + early_gdt-0xC0000000), %ebx
	movl $(\limit & 0xffff), (%ebx) 
	movl $(\base & 0xffffff), 2(%ebx)
	movl $(\flags & 0xff), 5(%ebx)
	movl $((\limit >> 16) & 0xf), %ecx
	or $(((\flags >> 8) & 0xff) << 4), %ecx
	movl %ecx, 6(%ebx)
	movl $((\base >> 24) & 0xff), 7(%ebx)
.endm


.global init_gdt
.type init_gdt, @function
init_gdt:
	/* Setup early GDT just for kernel space */
	
	set_entry 0, 0, 0, 0
	set_entry 1, 0x40000000, 0xffffffff, (GDT_FLAG_PRESENT | GDT_FLAG_SEGMENT | GDT_FLAG_CODE | GDT_FLAG_32 | GDT_FLAG_4KGRAN)
	set_entry 2, 0x40000000, 0xffffffff, (GDT_FLAG_PRESENT | GDT_FLAG_SEGMENT | GDT_FLAG_DATA | GDT_FLAG_32 | GDT_FLAG_4KGRAN)
	
	/* Prepare Global Descriptor Table Pointer */
	movw $((3 * 8) - 1), (early_gdtp - 0xC0000000)
	movl $(early_gdt - 0xC0000000), (early_gdtp - 0xC0000000 + 2)

	/* Load GDT and reload Segment Registers */
	lgdt (early_gdtp - 0xC0000000)

	mov $0x10, %ax
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs
	mov %ax, %ss
	ljmp $0x8, $.1
.1:
	
	/* Adjust stack */
	movl %esp, %eax
	add $0xC0000000, %eax
	movl %eax, %esp
	movl (%esp), %eax
	add $0xC0000000, %eax	/* Fix return address */
	movl %eax, (%esp)
	movl 4(%esp), %eax
	add $0xC0000000, %eax	/* Fix multiboot structure address */
	movl %eax, 4(%esp)

	ret

