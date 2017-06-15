#include <string.h>

void *memcpy(void* restrict dest, const void* restrict source, size_t count){
    unsigned char *dst = (unsigned char *) dest;
    const unsigned char *src = (unsigned char *) source;
    int i;
    for(i=0; i<count; i++)
        dst[i] = src[i];
    return dest; 
}

