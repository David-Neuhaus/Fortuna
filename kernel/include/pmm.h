#ifndef _PMM_HEADER
#define _PMM_HEADER

#define MEM_LOCATION 0x
#define BITMAP_SIZE 2
static uint32_t bitmap[BITMAP_SIZE];
uint8_t* free_mem;

void pmm_set_used(void *, void *);
void pmm_init(void *);
void pmm_free(void *);
void *pmm_alloc(size_t);
void pmm_mark_used(int);

#endif
