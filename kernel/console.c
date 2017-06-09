#include <system.h>
#include <stdarg.h>

static int x=0;
static int y=0;
int i;
uint8_t terminal_color;
static const uint16_t VGA_WIDTH = 25;
static const uint16_t VGA_HEIGHT = 80;
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

int kprintf(char *in, ...){
    va_list args;
    int index,h;
    int b = 0;
    
    int ret = 0;

    va_start(args, in);

    for(i = 0; in[i] != '\0'; i++){
        
        if(in[i] == '\n'){
            if(++y == VGA_HEIGHT)
                scroll();
            x=0;
            ret++;
            continue;
        }
        else if(in[i] == '\t'){
            if(x+2 < VGA_WIDTH)
                x += 4 - (x%4);
            ret++;
            continue;
        }        
        else if(in[i]=='%'){
            b = 1;
        } else {
            if(b){
                if(in[i]=='d'){
                    h  = va_arg(args,int);
                    char out[digitnum(h)+1];
                    itoa(h, out, 10);
                    index = 0;
                    while(out[index]){
                        kputchar(*out);
                        index++;
                        //outb(0x03f8, *out); //serial interface emulator
                    }

                } else if(in[i]=='x'){
                    h = va_arg(args,int);
                    char out[digitnum(h)+1];
                    itoa(h, out, 16);
                    index = 0;
                    while(out[index]){
                        kputchar(*out);
                        index++;
                        //outb(0x03f8, out[index]); //serial interface emulator
                    }

                } else if(in[i]=='s'){
                    char* out = va_arg(args, char *); 
                    while(*out){
                        kputchar(*out);
                        *out++;
                        //outb(0x03f8, out[index]); //serial interface emulator
                    }

                }
                b = 0;
            } else {
                kputchar(in[i]);
                //outb(0x03f8, out[index]); //serial interface emulator
            }
            
        }
        
    } va_end(args);
    kterminal_setCursor(x,y);


    return ret;
}

uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg){
    return fg | bg << 4;
}

uint16_t vga_entry(unsigned char c, uint8_t color){
    return (uint16_t) c | (uint16_t) color << 8;
}

void kterminal_init(void){
    clscr();
    x=y=0;
    kterminal_setCursor(0,0);
    kterminal_setColor(vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK));
    vid = (uint16_t*) 0xB8000;
    memsetw(vid, vga_entry(' ', terminal_color), VGA_WIDTH * VGA_HEIGHT);
}

void kterminal_setColor(uint8_t color){
    terminal_color = color;
}

void kputentryat(char c, uint8_t color, int column, int row){
    vid[row * VGA_WIDTH + column] = vga_entry(c, color);
}

int kputchar(char c){
    kputentryat(c, terminal_color, x, y);
    if(++x == VGA_WIDTH){
        x = 0;
        if(++y == VGA_HEIGHT)
            scroll();
    }

    return (int) c;
}

int kputs(char *str){
    char *s = str;
    while(*s){
        kputchar(*s);
        s++;
    }
    
    if(++y == VGA_HEIGHT)
        scroll();

    return 1;
}

/* TODO */
int itoab(int in, char *ascii){
    return -1;
}

/* TODO */
int itoao(int in, char *ascii){
    return -1;
}

int itoad(int in, char *ascii){
    int tmp;
    int id,d;
    char temp;
    for(id=0; in>=1; id++){
        tmp = in % 10;
        ascii[id]=(char) (((int)'0') + tmp);
        in = in/10;
    } id--;
    for(d=0; d<=id/2; d++){
        temp = ascii[d];
        ascii[d] = ascii[id-d];
        ascii[id-d] = temp;
    }
    id++;
    ascii[id]='\0';
    return id;
}

int itoax(int in, char *ascii){
    int tmp;
    int id,d;
    char temp;
    for(id=0; in>1; i++){
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
    return in;
}

int digitnum(int in){
    int id;
    for(id=0; in/10>=1; id++)
        in/=10;
    return id;
}

int itoa(int value, char *str, int base){
    if(base == 2)
        return itoab(value, str);
    else if(base == 8)
        return itoao(value, str);
    else if(base == 10)
        return itoad(value, str);
    else if(base == 16)
        return itoax(value, str);
    else
        return -1;
}


void clscr(void){
    char *video = (char *) 0xb8000;
    for(; i<2000; i++){
        video[i*2]=0x00;
        video[i*2+1]=0x07;
    }
    i = 0;
    x = 0;
    y = 0;
    kterminal_setCursor(0,0);
}

void scroll(void){
    unsigned char *video = (unsigned char *) 0xb8000;
    memcpy(video, &video[160], 160*24);
    memset(&video[160*24], 0x00, 160);
    x = 0;
    y--;
}
