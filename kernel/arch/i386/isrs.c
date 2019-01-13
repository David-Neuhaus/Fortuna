#include <kernel/isr.h>
#include <kernel/tss.h>

#include <stdio.h>

#include "cpu.h"

struct cpu_state* handle_exception(struct cpu_state* cpu){
	printf(" in handle exception \n");
	if(cpu->intr < 0x20){
		printf("%s, Kernel stopped!\n", err_msg[cpu->intr]);
		printf("CPU status: \n eax: \t%x\n ebx: \t%x\n ecx: \t%x\n edx: \t%x\n esi: \t%x\n "
                "edi: \t%x\n ebp: \t%x\n\nInterruptnummer: \t%x\nError-Code:\t%u\n\nAutomatisch"
                " gesichert: \n eip: \t%x\n cs: \t%x\n eflags: \t%x\n esp: \t%x\n ss: \t%x\n", cpu->eax, 
                cpu->ebx, cpu->ecx, cpu->edx, cpu->esi, cpu->edi, cpu->ebp, cpu->intr, cpu->error, cpu->eip, cpu->cs, cpu->eflags, cpu->esp, cpu->ss);
		while(1)
			asm volatile("cli; hlt");
	} 

    return cpu;
}

static void (*irq_handlers[16])(void*);

struct cpu_state* handle_irq(struct cpu_state* cpu){
	printf(" in handle_irq \n");
	struct cpu_state* new_cpu = cpu;
	//if(cpu->intr >= 0x20 && cpu->intr <= 0x2f){

        //if(cpu->intr == 0x20)
            //irq_handlers[cpu->intr - 0x20]((void*) cpu);

        /*
		if(cpu->intr == 0x20) {
			new_cpu = schedule(cpu);
			tss[1] = (uint32_t) (new_cpu + 1);
		} */ 
		printf("CPU struct at %p\n", (void*) cpu);		
		if(cpu->intr >= 0x28){
		        outb(0xa0, 0x20);
    	}
    	
		outb(0x20, 0x20);
		
		printf("Interrupt: %s\n", err_msg[cpu->intr]);
        printf("CPU status: \n eax: \t%x\n ebx: \t%x\n ecx: \t%x\n edx: \t%x\n esi: \t%x\n"
               " edi: \t%x\n ebp: \t%x\n\nInterruptnummer: \t%x\nError-Code:\t%u\n\nAutomatisch"
               " gesichert: \n eip: \t%x\n cs: \t%x\n eflags: \t%x\n esp: \t%x\n ss: \t%x\n", cpu->eax, 
                cpu->ebx, cpu->ecx, cpu->edx, cpu->esi, cpu->edi, cpu->ebp, cpu->intr, cpu->error, cpu->eip, cpu->cs, cpu->eflags, cpu->esp, cpu->ss);
		
		asm volatile("cli; hlt");
	//}
	printf("out of if case in irq_handler\n");
	return cpu;
}

void set_irq_handler(int num, void (*func)(void *cpu)){
    if(num<16 && func)
        irq_handlers[num] = func;
}

struct cpu_state* handle_sys(struct cpu_state* cpu){
    printf(" in handle_sys \n");
	return cpu;
}
