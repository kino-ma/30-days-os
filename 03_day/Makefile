GCC := i386-elf-gcc
LINKER := hrb.ld

SRC_IPL := ipl10.s
SRC_ASM_HEAD := asmhead.s
SRC_BOOTPACK := bootpack.c
SRC_NASMFUNC := nasmfunc.s

IPL := ipl10.bin
SYS := haribote.sys
BOOT_HRB := bootpack.hrb
HEAD_BIN := asmhead.bin
NASMFUNC_O := nasmfunc.o

LST_IPL := ipl10.lst
LST_HEAD := asmhead.lst
LST_NASMFUNC := nasmfunc.lst

IMG := haribote.img

OBJS := $(IPL) $(SYS) $(BOOT_HRB) $(HEAD_BIN) $(NASMFUNC_O) $(IMG)
LSTS := $(LST_IPL) $(LST_HEAD) $(LST_NASMFUNC)



default: run


$(IMG): $(IPL) $(SYS)
	mformat -f 1440 -C -B $(IPL) -i $(IMG) ::
	mcopy -i $(IMG) $(SYS) ::


$(SYS): $(BOOT_HRB) $(HEAD_BIN)
	cat $(HEAD_BIN) $(BOOT_HRB) > $(SYS)

$(HEAD_BIN):
	nasm $(SRC_ASM_HEAD) -o $(HEAD_BIN) -l $(LST_HEAD)

$(BOOT_HRB): $(SRC_BOOTPACK) $(LINKER) $(NASMFUNC_O)
	$(GCC) -march=i486 -m32 -nostdlib -T $(LINKER) -g $(SRC_BOOTPACK) $(NASMFUNC_O) -o $(BOOT_HRB)

$(NASMFUNC_O): $(SRC_NASMFUNC)
	nasm -g -f elf $(SRC_NASMFUNC) -o $(NASMFUNC_O) -l $(LST_NASMFUNC)


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