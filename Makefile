.phony: build
build: hd.img

.phony: run
run: build
	qemu -hda hd.img

.phony: clean
clean:
	rm -f *.bin *.lst *.img

hd.img: boot.bin pgm.bin
	qemu-img create -f raw $@ 10M
	dd conv=notrunc bs=512 if=boot.bin of=$@
	dd conv=notrunc bs=512 if=pgm.bin of=$@ seek=1

%.bin: %.asm
	nasm $< -o $@ -l $*.lst
