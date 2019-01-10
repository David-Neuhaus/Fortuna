.section multiboot_old

#define MBH_MAGIC 0x1badb002
#define MBH_MEMINFO 1<<1 	/* Provide memory map */
#define MBH_ALIGN 1<<0 		/* align loaded modules on page boundaries */
#define MBH_FLAGS (MBH_ALIGN | MBH_MEMINFO)
#define MBH_CHECKSUM -(MBH_MAGIC+MBH_FLAGS)

.int MBH_MAGIC
.int MBH_FLAGS
.int MBH_CHECKSUM

.section multiboot_new

new_header_start:

#define MBHn_MAGIC 0xE85250D6
#define MBHn_ARCH 0
#define MBHn_HEADER_LENGTH (new_header_end - new_header_start)
#define MBHn_CHECKSUM (0x100000000 -(MBHn_MAGIC + MBHn_ARCH + MBHn_HEADER_LENGTH))

.int MBHn_MAGIC
.int MBHn_ARCH
.int MBHn_HEADER_LENGTH
.int MBHn_CHECKSUM

/* tags */

/* End tag */
.word 0 /* type */
.word 0 /* flags */
.int 8 /* size */

/* End tag end */
new_header_end:


/* Kernel entry point */
.section .text


#include <boot/start.h>
.extern init
.extern init_gdt
.extern kernel_main

.global _start
.type _start, @function
_start:

	/* Initialize kernel stack with physical address of k_stack */
	movl $(k_stack - 0xC0000000), %esp
	
	/* Safe multiboot structure */
	pushl %eax
	pushl %ebx

	/* Set up GDT so that segments point to higher half kernel */
	movl $(init_gdt - 0xC0000000), %eax
	call %eax


	/* Call early init() in kernel/arch/i386/init.c */
	call init

	/* Call global constructors */
	call _init

	/* Call kernel_main in kernel/main.c*/
	call kernel_main

_stop:

	/* If the procedure should ever return, disable interrupts and stop the CPU. */
	cli
	hlt

	/* If an Error occues, try again. */
	jmp _stop

.section .bss
.align 16
//16kiB stack
.space STACK_SIZE
k_stack:
