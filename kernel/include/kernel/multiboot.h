#ifndef KERNEL_MULTIBOOT_H
#define KERNEL_MULTIBOOT_H

extern int MB_OLD, MB_NEW;

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
        struct mb2_mmap_entry entries[0];
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

#endif /* KERNEL_MULTIBOOT_H */
