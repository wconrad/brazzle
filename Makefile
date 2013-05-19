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

# Sector numbers for loader and kernel.  Must match corresponding
# equates in mem.inc

LOADER_LBA=1
KERNEL_LBA=100

hd.img: boot.bin loader.bin kernel.bin
	qemu-img create -f raw $@ 10M
	dd conv=notrunc bs=512 if=boot.bin of=$@
	dd conv=notrunc bs=512 if=loader.bin of=$@ seek=${LOADER_LBA}
	dd conv=notrunc bs=512 if=kernel.bin of=$@ seek=${KERNEL_LBA}

loader.bin: loader.o
	ld --script=loader.ld --print-map -o loader.bin $^ >loader.map

KERNEL_OBJ_FILES = \
	addr.o \
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

kernel.bin: kinit.o ${KERNEL_OBJ_FILES}
	ld --script=kernel.ld --print-map -o kernel.bin $^ >kernel.map

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
