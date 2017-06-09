#include <system.h>
#include <pmm.h>

static unsigned int base;
extern uint32_t ld_kernel_start, ld_kernel_end;

void pmm_set_used(void *start, void *end){
    
    
    
}

void pmm_init(void *mb_info_ptr){
if(MB_OLD){
    
    struct multiboot_info *mb_info = (struct multiboot_info *) mb_info_ptr;

    if(!(mb_info->mbs_flags & (1 << 6))){
        kprintf("Keine gültige memory map.\n");
        return; //TODO error management
    }

	struct mb_mmap_entry *mmap = &(mb_info->mbs_mmap_addr);
	struct mb_mmap_entry *mmap_end = (struct mb_mmap_entry *) ((uintptr_t) mmap + mb_info->mbs_mmap_length);
	

	while(mmap < mmap_end) {
				
		if(mmap->type == 1){
			uintptr_t addr = mmap->baseAddr;
			uintptr_t end_addr = mmap->length + addr;

			while(addr < end_addr) {
				pmm_free((void*) addr);
				addr++;
			}
		}
		mmap += mmap->size + 4;
	} 
}

if(MB_NEW){
    
    struct multiboot_info_v2 *mb_info = (struct multiboot_info_v2 *) mb_info_ptr;

    if((mb_info->mb2_mmap).type != 6){
        kprintf("Keine gültige memory map.\n");
        return; //TODO error management
    }
    
    uint32_t entry_size = (mb_info->mb2_mmap).entry_size;

    struct mb2_mmap_entry *mmap = &((mb_info->mb2_mmap).entries);
    struct mb2_mmap_entry *mmap_end = (struct mb2_mmap_entry *) ((uintptr_t) mmap + (mb_info->mb2_mmap).size);
    

    while(mmap < mmap_end){

        if(mmap->type == 1){
            uintptr_t addr = (uintptr_t) mmap->base_addr;
            uintptr_t end_addr = (uintptr_t) mmap->length + addr;
            
            while(addr < end_addr) {
                pmm_free((void *) addr);
                addr++;
            }
        }
        mmap += entry_size;
    }
}

    /* Kernel als belegt markieren */
    pmm_set_used(&ld_kernel_start, &ld_kernel_end);
    

    /* Array für freien Speicher */
    free_mem = pmm_alloc(1);
}

void pmm_mark_used(int num){

}

void* pmm_alloc(int bytes){
	
	int i=0;
	for(i=0;i<2;i++){
		if(!(bitmap[i] & 0xFFFFFFFF)){
			int i2;
			for(i2=0; i2<32; i2++){
				if(bitmap[i] & (1 << i2)){
					pmm_mark_used((i*32)+i2);
					return (uint8_t*) (base + 4096*((i*32) + i2));
				}
			}
			
		}
	}  
	
	uint8_t* addr;	
	addr = free_mem[0];
	free_mem = &free_mem[1];
	
	
	return addr;
	
}

void pmm_free(void* addr){
	
	
	
}
