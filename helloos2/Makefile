SRC := boot.s
IMG := boot.img

run: $(IMG)
	qemu-system-i386 \
		-boot a \
		-drive file=$(IMG),format=raw,if=floppy

$(IMG): $(SRC)
	nasm $(SRC) -o $(IMG)