.section multiboot_old

#define MBH_MAGIC 0x1badb002
#define MBH_FLAGS 0x0
#define MBH_CHECKSUM -(MBH_MAGIC+MBH_FLAGS)

.align 4
.int MBH_MAGIC
.int MBH_FLAGS
.int MBH_CHECKSUM

.section multiboot_new

new_header_start:

#define MBHn_MAGIC 0xE85250D6
#define MBHn_ARCH 0
#define MBHn_HEADER_LENGTH (new_header_end - new_header_start)
#define MBHn_CHECKSUM -(MBHn_MAGIC + MBHn_ARCH + MBHn_HEADER_LENGTH)

.align 8
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

.section .text

.extern init

.global _start

.align 4

_start:

	/* Initialize kernel stack */
	movl $k_stack, %esp
	
	/* Check version of multiboot */
	movl $0x36d76289, %ecx
	cmp %eax, %ecx
	je 1f

	#define MB_old
	jmp 2f

	1:
	#define MB_new
	
	2:
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
