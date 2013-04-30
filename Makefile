.phony: build
build: hd.img

.phony: run
run: build
	qemu -hda hd.img

.phony: clean
clean:
	rm -f *.bin *.lst *.img *.o

hd.img: boot.bin pgm.bin
	qemu-img create -f raw $@ 10M
	dd conv=notrunc bs=512 if=boot.bin of=$@
	dd conv=notrunc bs=512 if=pgm.bin of=$@ seek=1

pgm.bin: pgm.o main.o
	ld -T pgm.ld -o pgm.bin $^

pgm.o: pgm.asm
	nasm -f aout -o $@ -l $*.lst $<

boot.bin: boot.asm
	nasm -f bin -o $@ -l $*.lst $<

GCC_OPTS = \
	-O2 \
	-Tno-strict-aliasing \
	-pedantic-errors \
	-Werror \
	-Wall \
	-Wextra \
	-fno-builtin \
        -fomit-frame-pointer \
	-nostdinc \
	-std=c99

%.o: %.c
	gcc -c ${GCC_OPTS} -o $@ $<
