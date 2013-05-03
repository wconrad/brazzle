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

hd.img: boot.bin pgm.bin
	qemu-img create -f raw $@ 10M
	dd conv=notrunc bs=512 if=boot.bin of=$@
	dd conv=notrunc bs=512 if=pgm.bin of=$@ seek=1

OBJ_FILES = \
	main.o \
	conv.o \
	sprintf.o \
	string.o \
	vid.o \
	vty.o

pgm.bin: pgm.o ${OBJ_FILES}
	ld --script=pgm.ld --print-map -o pgm.bin $^ >pgm.map

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
