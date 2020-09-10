#include "bootpack.h"

extern struct KEYBUF keybuf;

void HariMain(void) {
    struct BOOTINFO *binfo;
    extern char hankaku[4096];
    char txt[30], mcursor[256];
    int mx, my, i;

    init_gdtidt();
    init_pic();
    io_sti();

    init_palette();

    binfo = (struct BOOTINFO *)ADR_BOOTINFO;

    init_screen(binfo->vram, binfo->scrnx, binfo->scrny);

    draw_string(binfo->vram, binfo->scrnx, 11, 11, "Hello.", BLACK);
    draw_string(binfo->vram, binfo->scrnx, 10, 10, "Hello. mak OS", WHITE);

    sprintf(txt, "disp_W = %d", binfo->scrnx);
    draw_string(binfo->vram, binfo->scrnx, 30, 30, txt, LIGHT_GRAY);

    mx = 70, my = 70;
    init_mouse_cursor8(mcursor, DARK_MIZU);
    putblock8_8(binfo->vram, binfo->scrnx, 16, 16, mx, my, mcursor, 16);

    io_out8(PIC0_IMR, 0xf9); // PIC1とキーボードを許可(11111001)
	io_out8(PIC1_IMR, 0xef); // マウスを許可(11101111)

    while (1) {
        io_cli();
        if (!keybuf.flag) {
            io_stihlt();
        } else {
            i = keybuf.data;
            keybuf.flag = 0;
            io_sti();

            sprintf(txt, "%d", i);

            init_screen(binfo->vram, binfo->scrnx, binfo->scrny);
            boxfill8(binfo->vram, binfo->scrnx, 0, 0, 32 * 8 - 1, 15, BLACK);
            draw_string(binfo->vram, binfo->scrnx, 0, 0, txt, WHITE);
        }
    }
}