#ifndef KERNEL_TSS_H
#define KERNEL_TSS_H

#include <stdint.h>

#define TSS_ENTRIES 32

static uint32_t tss[TSS_ENTRIES] = {0,0,0x10};

#endif /* KERNEL_TSS_H */
