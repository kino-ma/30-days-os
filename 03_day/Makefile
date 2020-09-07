SRC_IPL := ipl10.s
SRC_SYS := haribote.s

IPL := ipl10.bin
SYS := haribote.sys
LST_IPL := ipl10.lst
LST_SYS := haribote.lst

IMG := haribote.img

OBJS := $(IPL) $(SYS) $(IMG)
LSTS := $(LST_IPL) $(LST_SYS)


default: run



$(IMG): $(IPL) $(SYS)
	mformat -f 1440 -C -B $(IPL) -i $(IMG) ::
	mcopy -i $(IMG) $(SYS) ::

$(SYS): $(SRC_SYS)
	nasm $(SRC_SYS) -o $(SYS) -l $(LST_SYS)

$(IPL): $(SRC_IPL)
	nasm $(SRC_IPL) -o $(IPL) -l $(LST_IPL)


build: $(IMG)

run: $(IMG)
	qemu-system-i386 \
		-boot a \
		-drive file=$(IMG),format=raw,if=floppy

clean:
	rm -rf $(OBJS) $(LSTS) $(IMG)

.PHONY: default build run clean