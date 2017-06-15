#ifndef _STDIO_H
#define _STDIO_H 1

#include <sys/cdefs.h>

#define EOF (-1)

#ifdef __cplusplus
extern "C" {
#endif

int printf(const char*, ...);
int putchar(int);
int puts(const char*);

#include <stdint.h> /* Needed for port functions */

/* port functions */
#ifndef __OUTB
#define __OUTB
    void inline outb(uint16_t port, uint8_t data){
        asm volatile ("outb %0, %1" : : "a" (data), "Nd" (port));
    }


#endif /* __OUTB */

#ifndef __INB
#define __INB
    static inline uint8_t inb(uint16_t port){
        uint8_t ret;
        asm volatile ( "inb %1, %0"
                   : "=a"(ret)
                   : "Nd"(port) );
        return ret;
    }
#endif /* __INB */

#ifdef __cplusplus
}
#endif

#endif

