#include <gdt.h>
#include <system.h>
#include <tss.h>

static uint64_t gdt[GDT_ENTRIES];

extern void loadgdt(void);

static void init_gdt(void);
static void load_gdt(void);


void gdt_init(void){
	init_gdt();
}

static void set_entry(int i, uint32_t base, uint32_t limit, int flags){
	gdt[i] = limit & 0xffffLL;
	gdt[i] |= (base & 0xffffffLL) << 16;
	gdt[i] |= (flags & 0xffLL) << 40;
	gdt[i] |= ((limit >> 16) & 0xfLL) << 48;
	gdt[i] |= ((flags >> 8) & 0xffLL) << 52;
	gdt[i] |= ((base >> 24) & 0xffLL) << 56;
}

static void init_gdt(void){
	set_entry(0,0,0,0);
	set_entry(1,0,0xffffffff, GDT_FLAG_PRESENT | GDT_FLAG_SEGMENT | GDT_FLAG_CODE | GDT_FLAG_32 | GDT_FLAG_4KGRAN);
	set_entry(2,0,0xffffffff, GDT_FLAG_PRESENT | GDT_FLAG_SEGMENT | GDT_FLAG_DATA | GDT_FLAG_32 | GDT_FLAG_4KGRAN);
	set_entry(3,0,0xffffffff, GDT_FLAG_PRESENT | GDT_FLAG_SEGMENT | GDT_FLAG_CODE | GDT_FLAG_32 | GDT_FLAG_4KGRAN | GDT_FLAG_USRMODE);
	set_entry(4,0,0xffffffff, GDT_FLAG_PRESENT | GDT_FLAG_SEGMENT | GDT_FLAG_DATA | GDT_FLAG_32 | GDT_FLAG_4KGRAN | GDT_FLAG_USRMODE);
	set_entry(5,(uint32_t) tss, sizeof(tss), GDT_FLAG_PRESENT | GDT_FLAG_TSS | GDT_FLAG_USRMODE);
	//set_entry(6,0,0xfffff, GDT_FLAG_PRESENT | GDT_FLAG_TSS | GDT_FLAG_USRMODE );
	
	load_gdt();
}

static void load_gdt(void){
	struct {
    		uint16_t limit;
    		void* pointer;
	} __attribute__((packed)) gdtp = {
    		.limit = GDT_ENTRIES * 8 - 1,
    		.pointer = gdt,
	};
	asm volatile("lgdt %0" : : "m" (gdtp));
	loadgdt();
	
	//Load TSL registers
	asm volatile("ltr %%ax" : : "a" (5 << 3));
}
