#define GDT_ENTRIES 6

#define GDT_FLAG_DATA 0x02
#define GDT_FLAG_CODE 0x0a
#define GDT_FLAG_TSS 0x09

#define GDT_FLAG_PRESENT 0x80
#define GDT_FLAG_KRNLMODE 0x00
#define GDT_FLAG_USRMODE 0x60
#define GDT_FLAG_SEGMENT 0x10

#define GDT_FLAG_4KGRAN 0x800
#define GDT_FLAG_32 0x400

void gdt_init(void);

