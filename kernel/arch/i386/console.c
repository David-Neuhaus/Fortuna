#include <stdint.h>
#include <stdlib.h>
#include <string.h>

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
    vid = (uint16_t*) 0xB8000;
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
        if(++x == VGA_WIDTH){
            x = 0;
            if(++y == VGA_HEIGHT)
                scroll();
        }
    }
    
    kterminal_setCursor(x,y);
    return (int) c;
}

int kputs(char *str){
    char *s = str;
    while(*s){
        if(*s == '\n'){
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

        kputchar(*s);
        s++;
    }
    
    if(++y == VGA_HEIGHT)
        scroll();

    kterminal_setCursor(x,y);
    return 1;
}

void clscr(void){
    
    memset(vid, vga_entry(' ', terminal_color), VGA_WIDTH * 2 * VGA_HEIGHT);
    
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

void scroll(void){
    unsigned char *video = (unsigned char *) 0xb8000;
    memcpy(video, &video[160], 160*24);
    memset(&video[160*24], 0x00, 160);
    x = 0;
    y--;
}
