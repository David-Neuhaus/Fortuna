.section .text

/* 
 * The functions to load the GDT table.
 */

.global loadgdt

loadgdt:
	mov $0x10, %ax
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs
	mov %ax, %ss
	ljmp $0x8, $.1
.1:
	ret

/* 
 * Macros for interruuption stubs.
 */

.macro intr_stub nr
.global intr_stub_\nr
intr_stub_\nr:
	pushl $0x0;
	pushl $\nr
	jmp intr_common_handler
.endm

.macro intr_stub_err nr
.global intr_stub_\nr
intr_stub_\nr:
	pushl $\nr
	jmp intr_common_handler
.endm

/* Exceptions */
intr_stub 0
intr_stub 1
intr_stub 2
intr_stub 3
intr_stub 4
intr_stub 5
intr_stub 6
intr_stub 7
intr_stub_err 8
intr_stub 9
intr_stub_err 10
intr_stub_err 11
intr_stub_err 12
intr_stub_err 13
intr_stub_err 14
intr_stub 15
intr_stub 16
intr_stub_err 17
intr_stub 18

/* IRQs */
intr_stub 32
intr_stub 33
intr_stub 34
intr_stub 35
intr_stub 36
intr_stub 37
intr_stub 38
intr_stub 39
intr_stub 40
intr_stub 41
intr_stub 42
intr_stub 43
intr_stub 44
intr_stub 45
intr_stub 46
intr_stub 47

/* Syscall */
intr_stub 48

/*
 * Save the registers, load the kernel data segments and call the handlers in the IDT.
 * After that return to the (user space?) program.
 */

.extern handle_interrupt
intr_common_handler:

	push %ebp
	push %edi
	push %esi
	push %edx
	push %ecx
	push %ebx
	push %eax
	
	mov $0x10, %ax
	mov %eax, %ds
	mov %eax, %es

	push %esp
	call handle_interrupt
	mov %eax, %esp
	
	mov $0x23, %ax
	mov %ax, %ds
	mov %ax, %es

	pop %eax
	pop %ebx
	pop %ecx
	pop %edx
	pop %esi
	pop %edi
	pop %ebp

	/* Fetch error code from the stack */
	add $8, %esp
	
	iret