#include "bootpack.h"

extern struct FIFO8 keyfifo;
extern struct FIFO8 mousefifo;

void HariMain(void) {
    struct BOOTINFO *binfo;
    extern char hankaku[4096];
    char txt[30], mcursor[256];
    unsigned char keybuf[32], mousebuf[32];
    int mx, my, i;

    init_gdtidt();
    init_pic();
    io_sti();

    init_keyboard();
    enable_mouse();
    fifo8_init(&keyfifo, keybuf, 32);
    fifo8_init(&mousefifo, mousebuf, 32);

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
        if (keyfifo.count > 0) {
            if (fifo8_get(&keyfifo, (unsigned char *)&i) < 0) {
                continue;
            }
            io_sti();

            sprintf(txt, "code  = %x", i);

            boxfill8(binfo->vram, binfo->scrnx, 30, 30, 190, 46, DARK_MIZU);
            draw_string(binfo->vram, binfo->scrnx, 30, 30, txt, WHITE);
        } else if (mousefifo.count > 0) {
            if (fifo8_get(&mousefifo, (unsigned char *)&i) < 0) {
                continue;
            }
            io_sti();

            sprintf(txt, "mouse = %x", i);
            boxfill8(binfo->vram, binfo->scrnx, 30, 50, 190, 66, DARK_MIZU);
            draw_string(binfo->vram, binfo->scrnx, 30, 50, txt, WHITE);
        } else {
            io_stihlt();
        }
    }
}


void wait_KBC_sendready(void) {
    while (io_in8(PORT_KEYSTA) & KEYSTA_SEND_NOTREADY)
        ;
    return;
}

void init_keyboard(void) {
    wait_KBC_sendready();
    io_out8(PORT_KEYCMD, KEYCMD_WRITE_MODE);
    wait_KBC_sendready();
    io_out8(PORT_KEYDAT, KBC_MODE);
    return;
}

void enable_mouse(void) {
    wait_KBC_sendready();
    io_out8(PORT_KEYCMD, KEYCMD_SENDTO_MOUSE);
    wait_KBC_sendready();
    io_out8(PORT_KEYDAT, MOUSCMD_ENABLE);
    return;
}