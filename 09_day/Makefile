GCC := i386-elf-gcc
LINKER := hrb.ld
FONT_CONVERTER := convHankakuTxt.c
CONVERTER := converter

SRC_IPL := ipl10.s
SRC_ASM_HEAD := asmhead.s
SRC_BOOTPACK := bootpack.h bootpack.c graphic.c dsctbl.c int.c fifo.c
SRC_NASMFUNC := nasmfunc.s
SRC_MYSPRINTF := mysprintf.c

FONT_TXT := hankaku.txt
FONT_DATA := hankaku.c

IPL := ipl10.bin
SYS := haribote.sys
BOOT_HRB := bootpack.hrb
HEAD_BIN := asmhead.bin
NASMFUNC_O := nasmfunc.o

LST_IPL := ipl10.lst
LST_HEAD := asmhead.lst
LST_NASMFUNC := nasmfunc.lst

IMG := haribote.img

OBJS := $(IPL) $(SYS) $(BOOT_HRB) $(HEAD_BIN) $(NASMFUNC_O) $(CONVERTER)
LSTS := $(LST_IPL) $(LST_HEAD) $(LST_NASMFUNC)
DATA := $(FONT_DATA)



default: run


$(IMG): $(IPL) $(SYS)
	mformat -f 1440 -C -B $(IPL) -i $(IMG) ::
	mcopy -i $(IMG) $(SYS) ::


$(SYS): $(BOOT_HRB) $(HEAD_BIN)
	cat $(HEAD_BIN) $(BOOT_HRB) > $(SYS)

$(HEAD_BIN):
	nasm $(SRC_ASM_HEAD) -o $(HEAD_BIN) -l $(LST_HEAD)

$(BOOT_HRB): $(SRC_BOOTPACK) $(LINKER) $(NASMFUNC_O) $(FONT_DATA) $(SRC_MYSPRINTF)
	$(GCC) -march=i486 -m32 -nostdlib -fno-builtin \
		-T $(LINKER) \
		-o $(BOOT_HRB) \
		-g \
		$(SRC_BOOTPACK) \
		$(NASMFUNC_O) \
		$(FONT_DATA) \
		$(SRC_MYSPRINTF)

$(NASMFUNC_O): $(SRC_NASMFUNC)
	nasm -g -f elf $(SRC_NASMFUNC) -o $(NASMFUNC_O) -l $(LST_NASMFUNC)


$(IPL): $(SRC_IPL)
	nasm $(SRC_IPL) -o $(IPL) -l $(LST_IPL)


$(CONVERTER): $(FONT_CONVERTER)
	gcc $(FONT_CONVERTER) -o $(CONVERTER)

$(FONT_DATA): $(FONT_TXT) $(CONVERTER)
	./$(CONVERTER)



build: $(IMG)

run: $(IMG)
	qemu-system-i386 \
		-boot a \
		-drive file=$(IMG),format=raw,if=floppy

clean:
	rm -rf $(OBJS) $(LSTS) $(IMG) $(DATA)

.PHONY: default build run clean