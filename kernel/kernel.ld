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
    . = 0x100000;

    /*
     * The multiboot header has to be first. We put the new version before
     * the old to make sure that the right one is used.
     * The standart segments come next.
     */
    .text : {
        *(multiboot_new)
	*(multiboot_old)
        *(.text)
    }
    .data ALIGN(4096) : {
        *(.data)
    }
    .rodata ALIGN(4096) : {
        *(.rodata)
    }
    .bss ALIGN(4096) : {
        *(.bss)
    }
}
