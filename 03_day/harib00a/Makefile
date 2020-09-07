SRC := ipl.s
BIN := ipl.bin
IMG := helloos.img
LST := boot.lst

run: $(IMG)
	qemu-system-i386 \
		-boot a \
		-drive file=$(IMG),format=raw,if=floppy

$(IMG): $(BIN)
	mformat -f 1440 -C -B $(BIN) -i $(IMG) ::

$(BIN):
	nasm $(SRC) -o $(BIN) -l $(LST)

clean:
	rm -rf $(IMG) $(LST) $(BIN)

.PHONY: run clean