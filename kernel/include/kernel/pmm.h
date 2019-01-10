#ifndef _PMM_HEADER
#define _PMM_HEADER

#include <stdint.h>
#include <stddef.h>

/* Bitmap for DMA access */
#define BITMAP_SIZE 8
static uint32_t bitmap[BITMAP_SIZE];

/* Functions */

void pmm_set_used(void *, void *);
void pmm_init(void *);
void pmm_free(void *);
void *pmm_alloc(size_t);
void pmm_mark_used(int);

/* Structures */
struct mem_region {
	uint64_t phys_addr;
	uint32_t size;
	struct mem_region *next;
};

struct mem_region mem_broke;

struct p_mmap_struct {
	uint64_t phys_addr;
	uint32_t state;
	struct p_mmap_struct *next;
} p_mmap;

#endif
