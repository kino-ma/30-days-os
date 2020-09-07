SRC := ipl.s

OBJS := $(IPL) $(SYS)
LSTS := $(LST_IPL) $(LST_SYS)

IPL := ipl.bin
SYS := haribote.sys
LST_IPL := ipl.lst
LST_SYS := haribote.lst

IMG := haribote.img

defailt: run



$(IMG): $(IPL) $(SYS)
	mformat -f 1440 -C -B $(IPL) -i $(IMG) ::
	mcopy -i $(IMG) $(SYS) ::

$(SYS):
	nasm $(SRC_SYS) -o $(SYS) -l $(LST_SYS)

$(IPL):
	nasm $(SRC_IPL) -o $(IPL) -l $(LST_IPL)



run: $(IMG)
	qemu-system-i386 \
		-boot a \
		-drive file=$(IMG),format=raw,if=floppy

clean:
	rm -rf $(OBJS) $(LSTS)

.PHONY: default run clean