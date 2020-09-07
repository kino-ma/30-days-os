SRC_IPL := ipl.s
SRC_SYS := haribote.s

IPL := ipl.bin
SYS := haribote.sys
LST_IPL := ipl.lst
LST_SYS := haribote.lst

OBJS := $(IPL) $(SYS)
LSTS := $(LST_IPL) $(LST_SYS)

IMG := haribote.img


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