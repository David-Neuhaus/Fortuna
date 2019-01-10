/*  Start at _start (in start.S) */
ENTRY(_start)

/*
 * Specify the order of the sections in the exectubale.
 */
SECTIONS
{
    /*
     * Put the kernel as Upper Kernel.
     */
    . = 0xC0100000;
    
    ld_kernel_start = . ;

    /*
     * The multiboot header has to be first. We put the old version before
     * the new cause grub currently doesn't support multiboot 2.
     * The standart segments come next.
     */
    .text ALIGN(4K) : AT (ADDR (.text) - 0xC0000000) {
	*(multiboot_old)
        *(multiboot_new)
        *(.text)
    }
    .data ALIGN(4K) : AT (ADDR (.data) - 0xC0000000) {
        *(.data)
    }
    .rodata ALIGN(4K) : AT (ADDR (.rodata) - 0xC0000000){
        *(.rodata)
    }
    .bss ALIGN(4K) : AT (ADDR (.bss) - 0xC0000000) {
        *(COMMON)
        *(.bss)
    }

    ld_kernel_end = . ;

}
