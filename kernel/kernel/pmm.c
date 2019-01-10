#include <kernel/pmm.h>
#include <kernel/multiboot.h>

#include <stdio.h>
#include <stdint.h>

static unsigned int base;
extern const void* ld_kernel_start, ld_kernel_end;

void pmm_set_used(void *start, void *end){
    //TODO
    
    
}

void pmm_init(void *mb_info_ptr){

    size_t free_memory = 0;

    if(MB_OLD){
        
        struct multiboot_info *mb_info = (struct multiboot_info *) mb_info_ptr;

        if(!(mb_info->mbs_flags & (1 << 6))){
            printf("Keine gültige memory map.\n");
            return; //TODO error management
        }

        struct mb_mmap_entry *mmap = (struct mb_mmap_entry *) mb_info->mbs_mmap_addr;
        struct mb_mmap_entry *mmap_end = (struct mb_mmap_entry *) ((uintptr_t) mmap + mb_info->mbs_mmap_length);

        while(mmap < mmap_end) {
                    
            if(mmap->type == 1){
                uintptr_t addr = mmap->baseAddr;
                uintptr_t end_addr = mmap->length + addr;
                
                free_memory += (size_t) end_addr - addr;

                while(addr <= (end_addr-4096)) {
                    //add_mmap((void*) addr);
                    addr += 4096;
                }
            }
            mmap += mmap->size + 4;
        } 
    }

    if(MB_NEW){
        
        struct multiboot_info_v2 *mb_info = (struct multiboot_info_v2 *) mb_info_ptr;

        if((mb_info->mb2_mmap).type != 6){
            printf("Keine gültige memory map.\n");
            return; //TODO error management
        }
        
        uint32_t entry_size = (mb_info->mb2_mmap).entry_size;

        struct mb2_mmap_entry *mmap = (mb_info->mb2_mmap).entries;
        struct mb2_mmap_entry *mmap_end = (struct mb2_mmap_entry *) ((uintptr_t) mmap + (mb_info->mb2_mmap).size);
        

        while(mmap < mmap_end){

            if(mmap->type == 1){
                uintptr_t addr = (uintptr_t) mmap->base_addr;
                uintptr_t end_addr = (uintptr_t) mmap->length + addr;
             
                free_memory += (size_t) end_addr - addr;

                while(addr < end_addr) {
                    pmm_free((void *) addr);
                    addr++;
                }
            }
            mmap += entry_size;
        }
    }

    /* Kernel als belegt markieren */
    void *kernel_start = &ld_kernel_start;
    void *kernel_end = &ld_kernel_end;
    pmm_set_used(kernel_start, kernel_end);

    /* Multiboot structure als belegt markieren */

    //TODO
    
    free_memory -= (size_t) kernel_end - (size_t) kernel_start;

    /* Array für freien Speicher */
    //free_mem = pmm_alloc(free_memory);
}

void pmm_mark_used(int num){
	//TODO
}

void *pmm_alloc(size_t bytes){
	
	/* For little memory lookup in the bitmap for quick access */
	//TODO return right address and amount of memory, think about design
	if(bytes < 32){
		int i=0;
		for(i=0;i<2;i++){
			if(!(bitmap[i] & 0xFFFFFFFF)){
				int i2;
				for(i2=0; i2<32; i2++){
					if(bitmap[i] & (1 << i2)){
						pmm_mark_used((i*32)+i2);
						return (void *) (base + 4096*((i*32) + i2));
					}
				}
				
			}
		}  
	}
	
	/* Otherwise use the free memory list */
	void *addr;	
	//addr = (void *) free_mem[0];
	//free_mem = &free_mem[1];
	
	
	return addr;
	
}

void pmm_free(void* addr){
	
	//TODO
	
}
