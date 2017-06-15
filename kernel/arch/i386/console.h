#ifndef ARCH_I386_CONSOLE_H
#define ARCH_I386_CONSOLE_H
    uint8_t vga_entry_color(enum vga_color, enum vga_color);
    uint16_t vga_entry(unsigned char c, uint8_t color);
    void clscr(void);
    void clline(void);
    void scroll(void);
    void kputentryat(char c, uint8_t color, int column, int row);

/* kernel console functions */
#ifndef KCONSOLE_FUNCTIONS
#define KCONSOLE_FUNCTIONS
//    int kprintf(char *, ...);
    void kterminal_setCursor(int col, int row);
    void kterminal_init(void);
    void kterminal_setColor(uint8_t color);
    int kputchar(char c);
    int kputs(char *);
#endif /* KCONSOLE_FUNCTIONS */


#endif
