#include <limits.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

char *utoax(uint32_t in, char *ascii){
    uint32_t tmp;
    int id,d;
    char temp;
    for(id=0; in>=1; id++){
        tmp = in % 16;
        if(tmp<10)
           ascii[id]=(char) (((int)'0') + tmp);
        else{
            if(tmp==10)
                ascii[id]='a';
            else if(tmp==11)
                ascii[id]='b';
            else if(tmp==12)
                ascii[id]='c';
            else if(tmp==13)
                ascii[id]='d';
            else if(tmp==14)
                ascii[id]='e';
            else if(tmp==15)
                ascii[id]='f';
        }
        in = in/16;
    } id--;
    for(d=0; d<=id/2; d++){
        temp = ascii[d];
        ascii[d] = ascii[id-d];
        ascii[id-d] = temp;
    }
    id++;
    ascii[id]='\0';
    return ascii;

}

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

int digitnumh(int n){
    if (n < 0x0) n = (n == INT_MIN) ? INT_MAX : -n;
    if (n < 0x10) return 1;
    if (n < 0x100) return 2;
    if (n < 0x1000) return 3;
    if (n < 0x10000) return 4;
    if (n < 0x100000) return 5;
    if (n < 0x1000000) return 6;
    if (n < 0x10000000) return 7;
    return 8; 
}

int digitnumh_unsigned(uint32_t n){
    if (n < 0x10) return 1;
    if (n < 0x100) return 2;
    if (n < 0x1000) return 3;
    if (n < 0x10000) return 4;
    if (n < 0x100000) return 5;
    if (n < 0x1000000) return 6;
    if (n < 0x10000000) return 7;
    return 8; 
}



int printf(const char *in, ...){
    va_list args;

    //char *in = input;
    char out[1024]; //TODO this buffer is just a workaround, need to use malloc or check if buffer is large enough

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
                    itoa(h, out, 10);
		    out[digits] = '\0';
                    index = 0;
                    
                    while(out[index] != '\0'){
                        if(!putchar(out[index]))
                            return -1;
                        written++;
                        index++;
                        //outb(0x03f8, out[index]); //serial interface emulator
                    }

                } else if(in[i]=='p'){
                    printf("%x", (uintptr_t) va_arg(args, void*));
		    /*uint32_t tmp = (uint32_t) va_arg(args,void *);
                    int digits = digitnumh(tmp);
                    itoa(tmp, out, 16);
		    out[digits] = '\0';
                    index = 0;
                    
                    while(out[index] != '\0'){
                        if(!putchar(out[index]))
                            return -1;
                        written++;
                        index++;
                        //outb(0x03f8, out[index]); //serial interface emulator
                    }*/

                } else if(in[i]=='x'){
                    uint32_t hex_value = va_arg(args, uint32_t);
		    int digits = digitnumh_unsigned(hex_value);
		    out[0] = '0';
		    out[1] = 'x';
                    utoax(hex_value, (out+2));
		    out[digits+2] = '\0';
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
