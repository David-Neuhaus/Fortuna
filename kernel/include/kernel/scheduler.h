#ifndef KERNEL_SCHEDULER_H
#define KERNEL_SCHEDULER_H

#include <kernel/cpu.h>

/* scheduler functions */
#ifndef SCHEDULER_FUNCTIONS
#define SCHEDULER_FUNCTIONS
    struct cpu_state* init_task(void* entry);
    void init_multitasking(void);
    struct cpu_state* schedule(struct cpu_state* cpu);
#endif /* SCHEDULER_FUNCTIONS */

#endif /* KERNEL_SCHEDULER_H */
