/*  Start at _start (in start.S) */
ENTRY(_start)

/*
 * Specify the order of the sections in the exectubale.
 */
SECTIONS
{
    /*
     * Put the kernel at 1 MiB.
     */
    . = 1M;
    
    ld_kernel_start = . ;

    /*
     * The multiboot header has to be first. We put the old version before
     * the new cause grub currently doesn't support multiboot 2.
     * The standart segments come next.
     */
    .text BLOCK(4K) : ALIGN(4K) {
	    *(multiboot_old)
        *(multiboot_new)
        *(.text)
    }
    .data BLOCK(4K) : ALIGN(4K) {
        *(.data)
    }
    .rodata BLOCK(4K) : ALIGN(4K) {
        *(.rodata)
    }
    .bss BLOCK(4K) : ALIGN(4K) {
        *(COMMON)
        *(.bss)
    }

    ld_kernel_end = . ;

}
