#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h> //just for debugging

#include "vga.h"
#include "console.h"

static int x=0;
static int y=0;
uint8_t terminal_color;
static const uint16_t VGA_WIDTH = 80;
static const uint16_t VGA_HEIGHT = 25;
uint16_t* vid;


#ifndef __OUTB
#define __OUTB
void inline outb(unsigned short port, unsigned char data){
    asm volatile ("outb %0, %1" : : "a" (data), "Nd" (port));
}
#endif

void kterminal_setCursor(int col, int row){

    int position = row * VGA_WIDTH + col;
    outb(0x3d4, 0xf); //lowbyte
    outb(0x3d5, (unsigned char) position&0xFF);
    outb(0x3d4, 0xe); //highbyte
    outb(0x3d5, (unsigned char) (position>>8)&0xFF);

}
//
//int kprintf(char *in, ...){
//    va_list args;
//    int index,h;
//    int b = 0;
//    
//    int ret = 0;
//
//    va_start(args, in);
//
//    for(i = 0; in[i] != '\0'; i++){
//        
//        if(in[i] == '\n'){
//            if(++y == VGA_HEIGHT)
//                scroll();
//            x=0;
//            ret++;
//            continue;
//        }
//        else if(in[i] == '\t'){
//            if(x+2 < VGA_WIDTH)
//                x += 4 - (x%4);
//            ret++;
//            continue;
//        }        
//        else if(in[i]=='%'){
//            b = 1;
//        } else {
//            if(b){
//                if(in[i]=='d'){
//                    h  = va_arg(args,int);
//                    char out[digitnum(h)+1];
//                    itoa(h, out, 10);
//                    index = 0;
//                    while(out[index]){
//                        kputchar(*out);
//                        index++;
//                        //outb(0x03f8, *out); //serial interface emulator
//                    }
//
//                } else if(in[i]=='x'){
//                    h = va_arg(args,int);
//                    char out[digitnum(h)+1];
//                    itoa(h, out, 16);
//                    index = 0;
//                    while(out[index]){
//                        kputchar(*out);
//                        index++;
//                        //outb(0x03f8, out[index]); //serial interface emulator
//                    }
//
//                } else if(in[i]=='s'){
//                    char* out = va_arg(args, char *); 
//                    while(*out){
//                        kputchar(*out);
//                        *out++;
//                        //outb(0x03f8, out[index]); //serial interface emulator
//                    }
//
//                }
//                b = 0;
//            } else {
//                kputchar(in[i]);
//                //outb(0x03f8, out[index]); //serial interface emulator
//            }
//            
//        }
//        
//    } va_end(args);
//    kterminal_setCursor(x,y);
//
//
//    return ret;
//}
//
uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg){
    return fg | bg << 4;
}

uint16_t vga_entry(unsigned char c, uint8_t color){
    return (uint16_t) c | (uint16_t) color << 8;
}

void kterminal_init(void){
    kterminal_setColor(vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK));
    clscr();
    vid = (uint16_t*) 0xC00B8000;
}

void kterminal_setColor(uint8_t color){
    terminal_color = color;
}

void kputentryat(char c, uint8_t color, int column, int row){
    vid[row * VGA_WIDTH + column] = vga_entry(c, color);
}

int kputchar(char c){
    if(c == '\n'){
        if(++y == VGA_HEIGHT)
            scroll();
        x=0;
	outb(0x03f8, '\n'); //serial interface emulator
    }

    else if(c == '\t'){
        if((x += 4 - (x%4)) >= VGA_WIDTH){
            x = 0;
            if(++y == VGA_HEIGHT)
                scroll();
        }
    }

    else if(c == '\b'){
        if(--x == -1){
            x=0;
            if( y != 0)
                y--;
        }
        kputentryat(' ', terminal_color, x, y);
	outb(0x03f8, ' '); //serial interface emulator
    }

    else if(c == '\f'){
        clscr();
        x=0;
        y=0;
    }

    else if(c == '\r'){
        clline();
        x=0;
    }

    else if(c == '\v'){
        if(++y == VGA_HEIGHT)
            scroll();
        x=0;
    }

    else{
        kputentryat(c, terminal_color, x, y);
	outb(0x03f8, c); //serial interface emulator

        if(++x == VGA_WIDTH){
            x = 0;
            if(++y == VGA_HEIGHT)
                scroll();
        }
    }
    
    kterminal_setCursor(x,y);
    return (int) c;
}

/*
        if(*s == '\n'){
        outb(0x03f8, '\n'); //serial interface emulator
	    if(++y == VGA_HEIGHT)
                scroll();
            x=0;
            s++;
            continue;
        }

        if(*s == '\t'){
            if((x += 4 - (x%4)) >= VGA_WIDTH){
                x = 0;
                if(++y == VGA_HEIGHT)
                    scroll();
            }
            s++;
            continue;
        }

        if(*s == '\b'){
            if(--x == -1){
                x=0;
                if( y != 0)
                    y--;
            }
            kputentryat(' ', terminal_color, x, y);
	    outb(0x03f8, ' '); //serial interface emulator

            s++;
            continue;
        }

        if(*s == '\f'){
            clscr();
            x=0;
            y=0;
            s++;
            continue;
        }

        if(*s == '\r'){
            clline();
            x=0;
            s++;
            continue;
        }

        if(*s == '\v'){
            if(++y == VGA_HEIGHT)
                scroll();
            x=0;
            s++;
            continue;
        }

        //TODO add implementation of \nnn \xhh \Uhhhhhhhh \uhhhh



*/
int kputs(char *str){
    char *s = str;
    while(*s){
        kputchar(*s);
        s++;
    }
    
    if(++y == VGA_HEIGHT)
        scroll();

    kterminal_setCursor(x,y);
    return 1;
}

void clscr(void){
    int i;
    for(i=0; i< VGA_WIDTH*VGA_HEIGHT; i++)
    	vid[i] = vga_entry(' ', terminal_color);
    
    x = 0;
    y = 0;

    kterminal_setCursor(0,0);
}

void clline(void){
    while(x){
        kputentryat(' ', terminal_color, x, y);
        x--;
    }

    kterminal_setCursor(x,y);
}

/* scrolls two lines */
void scroll(void){
    memmove((void *) vid, (void *) &vid[VGA_WIDTH*2], (size_t) VGA_WIDTH*(VGA_HEIGHT-2)*2);
    int i;
    for(i=0; i<VGA_WIDTH*2; i++){
	vid[VGA_WIDTH*(VGA_HEIGHT-2)+i] = vga_entry(' ', terminal_color);
    }
    x = 0;
    y = VGA_HEIGHT-2;
}
