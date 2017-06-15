#include <stdio.h>

#if defined(__is_libk)
#include <kernel/console.h>
#endif

int putchar(int ic) {
#if defined(__is_libk)
    char c = (char) ic;
    kputchar(c);
#else
    // TODO implement stdio and write system call
#endif

    return ic;
}
