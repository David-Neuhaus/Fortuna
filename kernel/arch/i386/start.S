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
.extern init
.global _start
.type _start, @function

_start:

	/* Initialize kernel stack */
	movl $k_stack, %esp
	

	/* Call global constructors */
	call _init

	/* Call init() in kernel/init.c */
	pushl %eax
	pushl %ebx
	call init

_stop:

	/* If the procedure should ever return, disable interrupts and stop the CPU. */
	cli
	hlt

	/* If an Error occues, try again. */
	jmp _stop

.section .bss
.align 16
//16kiB stack
.space 0x4000
k_stack:
