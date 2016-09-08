.section multiboot

#define MBH_MAGIC 0x1badb002
#define MBH_FLAGS 0x0
#define MBH_CHECKSUM -(MBH_MAGIC+MBH_FLAGS)

.align 4
.int MBH_MAGIC
.int MBH_FLAGS
.int MBH_CHECKSUM

.section .text

.extern init

.global _start
_start:

	/* Initialize kernel stack */
	mov $k_stack, %esp

	/* Call init() in kernel/init.c */
	push %ebx
	call init

_stop:

	/* If the procedure should ever return, disable interrupts and stop the CPU. */
	cli
	hlt

	/* If an Error occues, try again. */
	jmp _stop

.section .bss
//16kiB stack
.space 0x4000
k_stack:
