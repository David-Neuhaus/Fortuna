#ifndef KCONSOLE_H
#define KCONSOLE_H

#include <stdint.h>

/* kernel console functions */
#ifndef KCONSOLE_FUNCTIONS
#define KCONSOLE_FUNCTIONS
    //int kprintf(char *, ...);
    void kterminal_setCursor(int col, int row);
    void kterminal_init(void);
    void kterminal_setColor(uint8_t color);
    int kputchar(char c);
    int kputs(char *);
#endif /* KCONSOLE_FUNCTIONS */
#endif /* KCONSOLE_H */
