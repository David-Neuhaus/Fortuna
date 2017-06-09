#ifndef SYSTEM_H
#define SYSTEM_H

/*Multiboot version */
int MB_OLD = 0;
int MB_NEW = 0;

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
    typedef long int intptr_t;
    typedef unsigned long int uintptr_t;
#endif /* INT_TYPES */

/* textmode color constants */
enum vga_color {
    VGA_COLOR_BLACK = 0,
    VGA_COLOR_BLUE = 1,
    VGA_COLOR_GREEN = 2,
    VGA_COLOR_CYAN = 3,
    VGA_COLOR_MAGENTA = 5,
    VGA_COLOR_BROWN = 6,
    VGA_COLOR_LIGHT_GREY = 7,
    VGA_COLOR_DARK_GREY = 8,
    VGA_COLOR_LIGHT_BLUE = 9,
    VGA_COLOR_LIGHT_GREEN = 10,
    VGA_COLOR_LIGHT_CYAN = 11,
    VGA_COLOR_LIGHT_RED = 12,
    VGA_COLOR_LIGHT_MAGENTA = 13,
    VGA_COLOR_LIGHT_BROWN = 14,
    VGA_COLOR_WHITE = 15,
};

/* kernel console functions */
#ifndef KCONSOLE_FUNCTIONS
#define KCONSOLE_FUNCTIONS
    int kprintf(char *, ...);
    void kterminal_setCursor(int col, int row);
    void kterminal_init(void);
    void kterminal_setColor(uint8_t color);
    void kputentryat(char c, uint8_t color, int column, int row);
    int kputchar(char c);
    int kputs(char *);
    uint8_t vga_entry_color(enum vga_color, enum vga_color);
    uint16_t vga_entry(unsigned char c, uint8_t color);
    void clscr(void);
    void scroll(void);
#endif /* KCONSOLE_FUNCTIONS */

/* type converting */
#ifndef TYPE_CONVERTING
#define TYPE_CONVERTING
    int itoab(int, char *);
    int itoao(int, char *);
    int itoad(int, char *);
    int itoax(int, char *);
    int itoa(int, char *, int);
    int digitnum(int);
#endif

/* memory functions */
    uint8_t *memcpy(uint8_t *dest, const uint8_t *src, int count);
    uint8_t *memset(uint8_t *dest, uint8_t val, int count);
    uint16_t *memsetw(uint16_t *dest, uint16_t val, int count);

/* string manipulating */
    int strlen(const char *str);

/* port functions */
#ifndef __OUTB
#define __OUTB
    void outb(uint16_t port, uint8_t data);
#endif /* __OUTB */
#ifndef __INB
#define __INB
    uint8_t inb(uint16_t port);
#endif /* __INB */

/* scheduler functions */
#ifndef SCHEDULER_FUNCTIONS
#define SCHEDULER_FUNCTIONS
    struct cpu_state* init_task(uint8_t* stack, uint8_t* user_stack, void* entry);
    void init_multitasking(void);
    struct cpu_state* schedule(struct cpu_state* cpu);
#endif /* SCHEDULER_FUNCTIONS */

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

/* Old Multiboot */

    struct mb_aout {
        uint32_t tabsize;
        uint32_t strsize;
        uint32_t addr;
        uint32_t reservered;
    };


    struct mb_elf {
        uint32_t num;
        uint32_t size;
        uint32_t addr;
        uint32_t reserved;
    };

    struct multiboot_info {
        uint32_t mbs_flags;
        uint32_t mbs_mem_lower;
        uint32_t mbs_bootdevice;
        uint32_t mbs_cmdline;
        uint32_t mbs_mods_count;
        uint32_t mbs_mods_addr;
        union {
            struct mb_aout maout;
            struct mb_aout melf;
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

    struct mb_mmap_entry {
        uint32_t size;
        uint64_t baseAddr;
        uint64_t length;
        uint32_t type;
    };

    struct mb_mod_entry {
        uint32_t mod_start;
        uint32_t mod_end;
        uint32_t string;
        uint32_t reserved; /*Must Be Zero!*/
    };
    
/* New Multiboot */

    struct mb2_mem_info {
        uint32_t type;
        uint32_t size;
        uint32_t mem_lower;
        uint32_t mem_upper;
    } __attribute__((aligned(8)));
    
    struct mb2_bios_boot_device {
        uint32_t type;
        uint32_t size;
        uint32_t biosdev;
        uint32_t partition;
        uint32_t sub_partition;
    } __attribute__((aligned(8)));


    struct mb2_command_line {
        uint32_t type;
        uint32_t size;
        uint8_t *string;
    } __attribute__((aligned(8)));

    struct mb2_modules {
        uint32_t type;
        uint32_t size;
        uint32_t mod_start;
        uint32_t mod_end;
        uint8_t *string;
    } __attribute__((aligned(8)));

    struct mb2_elf_symbols {
        uint32_t type;
        uint32_t size;
        uint16_t num;
        uint16_t entsize;
        uint16_t shndx;
        uint16_t reserved;
    } __attribute__((aligned(8)));

    struct mb2_mmap_entry {
        uint64_t base_addr;
        uint64_t length;
        uint32_t type;
        uint32_t reserved;
    } __attribute__((aligned(8)));

    struct mb2_mmap {
        uint32_t type;
        uint32_t size;
        uint32_t entry_size;
        uint32_t entry_version;
        struct mb_mmap_entry entries[0];
    } __attribute__((aligned(8)));

    struct mb2_boot_loader_name {
        uint32_t type;
        uint32_t size;
        uint8_t *name;
    } __attribute__((aligned(8)));

    struct mb2_apm_table {
        uint32_t type;
        uint32_t size;
        uint16_t version;
        uint16_t cseg;
        uint32_t offset;
        uint16_t cseg_16;
        uint16_t dseg;
        uint16_t flags;
        uint16_t cseg_len;
        uint16_t cseg_16_len;
        uint16_t dseg_len;
    } __attribute__((aligned(8)));

    struct mb2_vbe_info {
        uint32_t type;
        uint32_t size;
        uint16_t vbe_mode;
        uint16_t vbe_interface_seg;
        uint16_t vbe_interface_off;
        uint16_t vbe_interface_len;
        uint8_t vbe_control_info[512];
        uint8_t vbe_mode_info[256];
    } __attribute__((aligned(8)));

    struct mb2_framebuffer_color_desc {
        uint8_t red;
        uint8_t green;
        uint8_t blue;
    } __attribute__((aligned(8)));

    struct mb2_framebuffer_info {
        uint32_t type;
        uint32_t size;
        uint64_t framebuffer_addr;
        uint32_t framebuffer_pitch;
        uint32_t framebuffer_width;
        uint32_t framebuffer_height;
        uint8_t framebuffer_bpp;
        uint8_t framebuffer_type;
        uint8_t reserved;
        union {
            struct {
                uint32_t framebuffer_palette_num_colors;
                struct mb2_framebuffer_color_desc framebuffer_palette;
            };
            struct {
                uint8_t framebuffer_red_field_position;
                uint8_t framebuffer_red_mask_size;
                uint8_t framebuffer_green_field_position;
                uint8_t framebuffer_green_mask_size;
                uint8_t framebuffer_blue_field_position;
                uint8_t framebuffer_blue_mask_size;
            };
        };
    };

    

    struct multiboot_info_v2 {
        uint32_t total_size;
        uint32_t reserved;
        struct mb2_mem_info mb2_mem_info;
        struct mb2_bios_boot_device mb2_bios_boot_device;
        struct mb2_command_line mb2_command_line;
        struct mb2_modules mb2_modules;
        struct mb2_elf_symbols mb2_elf_symbols;
        struct mb2_mmap mb2_mmap;
        struct mb2_boot_loader_name mb2_boot_loader_name;
        struct mb2_apm_table mb2_apm_table;
        struct mb2_vbe_info mb2_vbe_info;
        struct mb2_framebuffer_info mb2_framebuffer;
    } __attribute__((aligned(8)));

#endif /* SYSTEM_H */
