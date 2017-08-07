.. Memory Management in Fortuna

============================
Memory Management in Fortuna
============================

In Fortuna the whole memory management is located in the kernel. It is split in two parts:

* :doc:`The physical memory manager <../kernel/kernel/pmm.rst>`
* :doc:`The virtual memory manager <../kernel/kernel/vmm.rst>`

Physical memory management
==========================

The physical memory manager is located in the kernel source file :file:`kernel/pmm.c`.

Physical memory map
*******************

The memory in fortuna is divided in two different parts. These are DMA and RAM/Reserved
areas. Those areas are divided in pages of 4 KiB which is the standard page size in Fortuna. 

The DMA memory is 16 MiB large and is continguous to provide back compatibility to ISA-DMA. 
It is at the lowest available physical memory location. The status of the DMA memory is stored 
in a bitmap of a size of 8 bytes where on bit displays 4KiB (one page) of the memory. A 1 means 
that the memory is in use and a 0 that it is free. The bitmap is stored in the struct dma_mem::

    #define BITMAP_SIZE 2
    struct dma_mem_struct {
        uint32_t base_low;
        uint32_t base_high;
        uint32_t bitmap[BITMAP_SIZE];
    } dma_mem;

With this definition the structs size is less than 0,1% of the displayed memory. The struct is
stored in the RAM memory used by the kernel.

The RAM is stored in a special RAM structure which points to often used page 
sizes::

    static struct ram_mem_struct {
        size_t free_pages;
        ppage_t *free[10];      /* Points to first block of 2^i pages in mem_map, max page size
                                   in PSE mode is 2^10 (4 MiB) */
    } ram_mem;

The actual memory map of the RAM/reserved is the struct mem_map which is an array of type ppage_t::

    typedef struct ppage {
        size_t       index;         // Used to calculate the physical address
        vpage_t      *virtual;      // Pointer to the corrsponding virtual page
        short        flags;         // Flags used to indicate the status
        struct ppage *next;         // Used to create linked lists (e. g. for free pages)
        struct ppage *previous;     
    } ppage_t;

..    ppage_t mem_map[TOTAL_RAM_SIZE];

.. The variable TOTAL_RAM_SIZE is counted in 4 KiB pages in the init function of pmm.c. 

The mem_map also contains the DMA region, although this region is marked special. This is necessary 
because the memory manager needs to calculate the physical memory address from the index given by the 
ppage entry. Whith this definitions the struct ppage has a size of 18 Bytes on an 32 bit x86 machine which 
is 0,44% of the displayed size.

The flags of a ppage can be as follows:

=====  ==========================
Bits   Description
=====  ==========================
0-3    Type of the physical page.
4-7    Status of the page.
8      Caching disabled bit.
9-11   Protection bits.
12-15  Reserved.
=====  ==========================

The type is one of the following:

====  ================
Bits  Meaning
====  ================
000   DMA Memory
001   BIOS Memory Area
010   VGA memory
011   ROM
100   RAM
101   various
110   reserved Memory
111   unknown
====  ================

The meaning of the status bits (if up) are as follows:

====  =========
Bit   Meaning
====  =========
0     free page
1     reserved
2     reserved
3     reserved
====  =========

The protection is handled through the read, write, execute bits which are bits 9-11, respectively.
Bits 12-15 are reserved for future use.
