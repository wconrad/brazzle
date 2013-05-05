.phony: default
default: build

-include *.d

.phony: build
build: hd.img

.phony: run
run: build
	qemu -hda hd.img

.phony: clean
clean:
	rm -f *.bin *.lst *.img *.o *.map

hd.img: boot.bin stage2.bin
	qemu-img create -f raw $@ 10M
	dd conv=notrunc bs=512 if=boot.bin of=$@
	dd conv=notrunc bs=512 if=stage2.bin of=$@ seek=1

OBJ_FILES = \
	conv.o \
	cpuasm.o \
	idt.o \
	idtasm.o \
	int.o \
	intasm.o \
	main.o \
	sprintf.o \
	string.o \
	vid.o \
	vty.o

stage2.bin: stage2.o ${OBJ_FILES}
	ld --script=stage2.ld --print-map -o stage2.bin $^ >stage2.map

%.o: %.asm
	nasm -f aout -o $@ -l $*.lst $<

boot.bin: boot.asm
	nasm -f bin -o $@ -l $*.lst $<

GCC_OPTS = \
	-Os \
	-Tno-strict-aliasing \
	-pedantic-errors \
	-Wall \
	-Wextra \
	--freestanding \
	-std=c99 \
        -MMD

%.o: %.c
	gcc -c ${GCC_OPTS} -o $@ $<
