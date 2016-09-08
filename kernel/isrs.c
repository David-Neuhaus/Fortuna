#include <system.h>
#include <isr.h>
#include <tss.h>



struct cpu_state* handle_interrupt(struct cpu_state* cpu){
	
	struct cpu_state* new_cpu = cpu;

	if(cpu->intr < 0x20){
		kprintf("%s, Kernel angehalten!\n", err_msg[cpu->intr]);
		kprintf("CPU Zustand: \n eax: \t%d\n ebx: \t%d\n ecx: \t%d\n edx: \t%d\n esi: \t%d\n edi: \t%d\n ebp: \t%d\n\nInterruptnummer: \t%d\nError-Code:\t%d\n\nAutomatisch gesichert: \n eip: \t%d\n cs: \t%d\n eflags: \t%d\n esp: \t%d\n ss: \t%d\n", cpu->eax, cpu->ebx, cpu->ecx, cpu->edx, cpu->esi, cpu->edi, cpu->ebp, cpu->intr, cpu->error, cpu->eip, cpu->eflags, cpu->esp, cpu->ss);
		while(1)
			asm volatile("cli; hlt");
	} 
	
	else if(cpu->intr >= 0x20 && cpu->intr <= 0x2f){
		
		if(cpu->intr == 0x20) {
			new_cpu = schedule(cpu);
			tss[1] = (uint32_t) (new_cpu + 1);
		} 
		
		if(cpu->intr >= 0x28) {
		        outb(0xa0, 0x20);
    		}
    		
		outb(0x20, 0x20);
		
		kprintf("Interrupt: %s", err_msg[cpu->intr]);
	}
	
	return new_cpu;
}
