#include <limits.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int digitnum(int n){
    if (n < 0) n = (n == INT_MIN) ? INT_MAX : -n;
    if (n < 10) return 1;
    if (n < 100) return 2;
    if (n < 1000) return 3;
    if (n < 10000) return 4;
    if (n < 100000) return 5;
    if (n < 1000000) return 6;
    if (n < 10000000) return 7;
    if (n < 100000000) return 8;
    if (n < 1000000000) return 9;
    return 10; //max length for 32-bit integers
}

int printf(const char *input, ...){
    va_list args;

    char *in = input;

    int index,h,i;
    int b = 0;
    int written = 0;

    va_start(args, in);

    for(i = 0; in[i] != '\0'; i++){
        
        if(in[i]=='%'){
            b = 1;
        } else {
            if(b){
                if(in[i]=='d' || in[i]=='i'){
                    h  = va_arg(args,int);
                    int digits = digitnum(h);
                    
                    char out[digits+1];
                    itoa(h, out, 10);
                    index = 0;
                    
                    while(out[index]){
                        if(!putchar(out[index]))
                            return -1;
                        written++;
                        index++;
                        //outb(0x03f8, out[index]); //serial interface emulator
                    }

                } else if(in[i]=='x'){
                    h = va_arg(args,int);
                    char out[digitnum(h)+1];
                    itoa(h, out, 16);
                    index = 0;
                    while(out[index]){
                        putchar(out[index]);
                        written++;
                        index++;
                        //outb(0x03f8, out[index]); //serial interface emulator
                    }

                } else if(in[i]=='c'){
                    char out = (char) va_arg(args, int);
                    putchar(out);
                    written++;
                    //outb(0x03f8, out); //serial interface emulator

                } else if(in[i]=='s'){
                    char* out = va_arg(args, char *); 
                    while(*out){
                        putchar(*out);
                        written++;
                        out++;
                        //outb(0x03f8, *out); //serial interface emulator
                    }
                } else if(in[i]=='%'){
                    putchar('%');
                    written++;
                    //outb(0x03f8, '%'); //serial interface emulator
                }
                b = 0;
            } else {
                putchar(in[i]);
                //outb(0x03f8, in[index]); //serial interface emulator
            }
            
        }
        
    } va_end(args);

    return written;
}
