.phony: default
default: build

.SUFFIXES: .bin
-include *.d

.phony: build
build: hd.img

.phony: run
run: build
	qemu -hda hd.img

.phony: clean
clean:
	rm -f *.bin *.lst *.img *.o *.map

hd.img: boot.bin loader.bin
	qemu-img create -f raw $@ 10M
	dd conv=notrunc bs=512 if=boot.bin of=$@
	dd conv=notrunc bs=512 if=loader.bin of=$@ seek=1

OBJ_FILES = \
	bmmap.o \
	buffwrite.o \
	conv.o \
	cpuasm.o \
	idt.o \
	idtasm.o \
	int.o \
	intasm.o \
	main.o \
	pmmap.o \
	sprintf.o \
	string.o \
	stringasm.o \
	vid.o \
	vty.o

loader.bin: loader.o ${OBJ_FILES}
	ld --script=loader.ld --print-map -o loader.bin $^ >loader.map

NASM_OPTS = -o $@ -l $*.lst -MD $*.d

%.o: %.asm
	nasm -f aout ${NASM_OPTS} $<

boot.bin: boot.asm
	nasm -f bin ${NASM_OPTS} $<

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
