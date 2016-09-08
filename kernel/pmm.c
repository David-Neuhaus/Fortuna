#include <system.h>
#include <pmm.h>
#include <multiboot.h>
#include <pmm.h>

static unsigned int base;

void pmm_init(struct multiboot_info *mb_info){
	struct mb_mmap_entry* mmap = mb_info->mbs_mmap_addr;
	struct mb_mmap_entry* mmap_end = (void*) ((uintptr_t) mb_info->mbs_mmap_addr + mb_info->mbs_mmap_length);
	

	while(mmap < mmap_end) {
				
		if(mmap->type == 1){
			uintptr_t addr = mmap->baseAddr;
			uintptr_t end_addr = mmap->length + addr;

			while(addr < end_addr) {
				pmm_free((void*) addr);
				addr += 0x1000;
			}
		}
		mmap++;
	} 
	
	//Noch Kernel als belegt markieren
}

uint8_t* pmm_alloc(){
	
	int i=0;
	for(i=0;i<2;i++){
		if(!(bitmap[i] & 0xFFFFFFFF)){
			int i2;
			for(i2=0; i2<32; i2++){
				if(bitmap[i] & (1 << i2)){
					pmm_mark_used((i*32)+i2;
					return (uint8_t*) (base + 4096*((i*32) + i2));
				}
			}
			
		}
	}  
	
	uint8_t* addr;	
	addr = free_mem[0];
	free_mem = &free_mem[1];
	
	if(
	
	return addr;
	}	
	
}

void pmm_free(void* addr){
	
	
	
}
