.section .data
debtext1:
.ascii "In tbl.S exception handler\n\0x0"

debtext2:
.ascii "In tbl.S irq handler\n\0x0"

.section .text
	
/* 
 * Macros for interruption stubs.
 */

.macro intr_stub nr
.global intr_stub_\nr
intr_stub_\nr:
	pushl $0x0
	pushl $\nr
	jmp exception_handler
.endm

.macro intr_stub_err nr
.global intr_stub_\nr
intr_stub_\nr:
	pushl $\nr
	jmp exception_handler
.endm

.macro intr_stub_irq nr
.global intr_stub_\nr
intr_stub_\nr:
	pushl 0x0
	pushl $\nr
	jmp irq_handler
.endm

.macro intr_stub_sys nr
.global intr_stub_\nr
intr_stub_\nr:
	pushl 0x0
	pushl $\nr
	jmp sys_handler
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
intr_stub_irq 32
intr_stub_irq 33
intr_stub_irq 34
intr_stub_irq 35
intr_stub_irq 36
intr_stub_irq 37
intr_stub_irq 38
intr_stub_irq 39
intr_stub_irq 40
intr_stub_irq 41
intr_stub_irq 42
intr_stub_irq 43
intr_stub_irq 44
intr_stub_irq 45
intr_stub_irq 46
intr_stub_irq 47

/* Syscall */
intr_stub 48

/*
 * Save the registers, load the kernel data segments and call the exception handler.
 * After that return to the (user space?) program.
 */
.extern printf
.extern handle_exception
exception_handler:

	push %ebp
	push %edi
	push %esi
	push %edx
	push %ecx
	push %ebx
	push %eax
	
	push debtext1
	call printf

	mov $0x10, %ax
	mov %eax, %ds
	mov %eax, %es

	push %esp
	call handle_exception
	mov %eax, %esp
	
	//mov $0x23, %ax
	//mov %ax, %ds
	//mov %ax, %es

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

/*
 * Save the registers, load the kernel data segments and call the hardware interrupt handler.
 * After that return to the (user space?) program.
 */

.extern handle_irq
irq_handler:

	push %ebp
	push %edi
	push %esi
	push %edx
	push %ecx
	push %ebx
	push %eax
	
	push debtext2
	call printf

	mov $0x10, %ax
	mov %eax, %ds
	mov %eax, %es

	push %esp
	call handle_irq
	mov %eax, %esp
	
	//mov $0x23, %ax
	//mov %ax, %ds
	//mov %ax, %es

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

/*
 * Save the registers, load the kernel data segments and call the syscall handler.
 * After that return to the (user space?) program.
 */

.extern handle_sys
sys_handler:

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
	call handle_sys
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
