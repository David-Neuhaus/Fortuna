#ifndef SYSTEMH
#define SYSTEMH

#ifndef INT_TYPES
   #define INT_TYPES
   typedef signed char            int8_t;
   typedef short int              int16_t;
   typedef int                    int32_t;
#if __WORDSIZE == 64
   typedef long int               int64_t;
#else
   typedef long long int          int64_t;
#endif
   typedef unsigned char          uint8_t;
   typedef unsigned short int     uint16_t;
   typedef unsigned int           uint32_t;
#if __WORDSIZE == 64
   typedef unsigned long int      uint64_t;
#else
   typedef unsigned long long int  uint64_t;
#endif
   typedef int16_t intptr_t;
   typedef uint16_t uintptr_t;
#endif /* INT_TYPES */


void kprintf(char *, ...);
int itoad(int, char *);
int itoax(int, char *);
int digitnum(int);
void clscr();
void scroll();
uint8_t *memcpy(uint8_t *dest, const uint8_t *src, int count);
uint8_t *memset(uint8_t *dest, uint8_t val, int count);
uint16_t *memsetw(uint16_t *dest, uint16_t val, int count);
int strlen(const char *str);
#ifndef __OUTB
#define __OUTB
void outb(uint16_t port, uint8_t data);
#endif
#ifndef __INB
uint8_t inb(uint16_t port);
#define __INB
#endif

struct cpu_state* init_task(uint8_t* stack, uint8_t* user_stack, void* entry);
void init_multitasking(void);
struct cpu_state* schedule(struct cpu_state* cpu);

struct cpu_state {
    uint32_t   eax;
    uint32_t   ebx;
    uint32_t   ecx;
    uint32_t   edx;
    uint32_t   esi;
    uint32_t   edi;
    uint32_t   ebp;
 
    uint32_t   intr;
    uint32_t   error;
 
    uint32_t   eip;
    uint32_t   cs;
    uint32_t   eflags;
    uint32_t   esp;
    uint32_t   ss;
};

struct mb_aout {
    uint32_t tabsize;
    uint32_t strsize;
    uint32_t addr;
    uint32_t reservered;
};

typedef struct mb_aout mb_symbol_table_aout_t;


struct mb_elf {
    uint32_t num;
    uint32_t size;
    uint32_t addr;
    uint32_t reserved;
};

typedef struct mb_elf mb_section_table_elf_t;

struct multiboot_info {
    uint32_t mbs_flags;
    uint32_t mbs_mem_lower;
    uint32_t mbs_bootdevice;
    uint32_t mbs_cmdline;
    uint32_t mbs_mods_count;
    uint32_t mbs_mods_addr;
    union {
        mb_symbol_table_aout_t maout;
        mb_section_table_elf_t melf;
    } mbs_syms;
    uint32_t mbs_mmap_length;
    uint32_t mbs_mmap_addr;
    uint32_t mbs_drives_length;
    uint32_t mbs_drives_addr;
    uint32_t mbs_config_table;
    uint32_t mbs_boot_loader_name;
    uint32_t mbs_apm_table;
    uint32_t mbs_vbe_control_info;
    uint32_t mbs_vbe_mode_info;
    uint16_t mbs_vbe_mode;
    uint16_t mbs_vbe_interface_seg;
    uint16_t mbs_vbe_interface_off;
    uint16_t mbs_vbe_interface_len;
};

typedef struct multiboot_info multiboot_info_t;

struct mb_mmap_entry {
    uint32_t size;
    uint64_t baseAddr;
    uint64_t length;
    uint32_t type;
};

typedef struct mb_mmap_entry mbs_mmap_entry_t;

struct mb_mod_entry {
    uint32_t mod_start;
    uint32_t mod_end;
    uint32_t string;
    uint32_t reserved; /*Must Be Zero!*/
};

typedef struct mb_mod_entry mb_mod_entry_t;
#endif
