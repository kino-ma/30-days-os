#include "bootpack.h"

void HariMain(void) {
    struct BOOTINFO *binfo;
    extern char hankaku[4096];
    char txt[30], mcursor[256];
    int mx, my;

    init_gdtidt();
    init_palette();

    binfo = (struct BOOTINFO *)ADR_BOOTINFO;

    init_screen(binfo->vram, binfo->scrnx, binfo->scrny);

    draw_string(binfo->vram, binfo->scrnx, 11, 11, "Hello.", hankaku, BLACK);
    draw_string(binfo->vram, binfo->scrnx, 10, 10, "Hello. mak OS", hankaku, WHITE);

    sprintf(txt, "disp_W = %d", binfo->scrnx);
    draw_string(binfo->vram, binfo->scrnx, 30, 30, txt, hankaku, LIGHT_GRAY);

    mx = 70, my = 70;
    init_mouse_cursor8(mcursor, DARK_MIZU);
    putblock8_8(binfo->vram, binfo->scrnx, 16, 16, mx, my, mcursor, 16);

    while (1) {
        io_hlt();
    }
}