SRCS = $(shell find -name '*.[cS]')
OBJS = $(addsuffix .o,$(basename $(SRCS)))

CC = /home/mad/opt/cross/bin/i686-elf-gcc
LD = /home/mad/opt/cross/bin/i686-elf-ld

ASFLAGS = -m32
CFLAGS = -m32 -Wall -g -ffreestanding -Wall -Wextra -I./include
LDFLAGS = -melf_i386 -Tkernel.ld

kernel: $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^

%.o: %.S
	$(CC) $(ASFLAGS) -c -o $@ $^

clean:
	rm $(OBJS)

