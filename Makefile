.phony: build
build: fd.img

.phony: run
run: build
	qemu -fda fd.img

.phony: clean
clean:
	rm -f *.bin *.lst *.img

fd.img: boot.bin
	dd if=/dev/zero of=$@ bs=512 count=2880
	dd conv=notrunc bs=512 if=$< of=$@

boot.bin: boot.asm
	nasm $< -o $@ -l boot.lst
