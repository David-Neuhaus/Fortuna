#include <system.h>

void task_a(void){
	while(1){
		kprintf("A");
		asm volatile("cli; hlt");
	}
}

void task_b(void){
	while(1){
		kprintf("B");
	}
}
