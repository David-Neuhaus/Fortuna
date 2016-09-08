#include <system.h>
#include <stdarg.h>

static int x=0;
static int y=0;
int i;

#ifndef __OUTB
#define __OUTB
void outb(unsigned short port, unsigned char data){
	asm volatile ("outb %0, %1" : : "a" (data), "Nd" (port));
}
#endif

void setCursor(int col, int row){

	int position = row*80+col;
	outb(0x3d4, 0xf); //lowbybte
	outb(0x3d5, (unsigned char) position&0xFF);
	outb(0x3d4, 0xe); //highbyte
	outb(0x3d5, (unsigned char) (position>>8)&0xFF);

}

void kprintf(char *in, ...){
	va_list args;
	int index,h;
	int b = 0;
	char *vid= (char*) 0xb8000;
	
	va_start(args, in);

	for(i = 0; in[i] != '\0'; i++){
		
		if(in[i] == '\n'){
			if(y!=24)
				y++;
			else{
				scroll();
			} x=0;
			continue;
		}
		else if(in[i] == '\t'){
			if(x+2 < 80)
				x += 2;
			continue;
		}		
		else if(in[i]=='%'){
			b = 1;
		} else {
			if(x==80){
				if(y!=24)
					y++;
				else{
					scroll();
				}
				x=0;
			}
			if(b){
				if(in[i]=='d'){
					h  = va_arg(args,int);
					char out[digitnum(h)+1];
					itoad(h, out);
					for(index=0;out[index]!='\0' && index<151; index++){
						vid[(y*80+x)*2] = out[index];
						vid[(y*80+x)*2+1] = 0x07;
						//outb(0x03f8, out[index]); //serial interface emulator
						if(x!=80)
							x++;
						else{
							x=0;
							if(y!=24)
								y++;
							else{
								scroll();
							}
						}
					}

				} else if(in[i]=='x'){
					h = va_arg(args,int);
					char out[digitnum(h)+1];
					itoax(h, out);
					for(index=0;out[index]!='\0' && index<151; index++){
						vid[(y*80+x)*2] = out[index];
						vid[(y*80+x)*2+1] = 0x07;
						//outb(0x03f8, out[index]); //serial interface emulator
						if(x!=80)
                                                        x++;
                                                else{
                                                        x=0;
							if(y!=24)
                                                        	y++;
							else{
								scroll();
							}
                                                }
					}

				} else if(in[i]=='s'){
					char* out = va_arg(args, char *); 
					for(index=0;out[index]!='\0'; index++){
                                                vid[(y*80+x)*2] = out[index];
                                                vid[(y*80+x)*2+1] = 0x07;
                                                //outb(0x03f8, out[index]); //serial interface emulator
                                                if(x!=80)
                                                        x++;
                                                else{   
                                                        x=0;
							if(y!=24)
                                                        	y++;
							else{
								scroll();
							}
                                                }
 
                                        }

				}
				b = 0;
			} else {
				vid[(y*80+x)*2] = in[i];
				vid[(y*80+x)*2+1] = 0x07;
				//outb(0x03f8, out[index]); //serial interface emulator
				x++;
			}
			
		}
		
	} va_end(args);
	setCursor(x,y);
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

void clscr(){
	char *video = (char *) 0xb8000;
	for(; i<2000; i++){
		video[i*2]=0x00;
		video[i*2+1]=0x07;
	}
	i = 0;
	x = 0;
	y = 0;
	setCursor(0,0);
}

void scroll(){
	unsigned char *video = (unsigned char *) 0xb8000;
	memcpy(video, &video[160], 160*24);
	memset(&video[160*24], 0x00, 160);
}
