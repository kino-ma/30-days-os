#include "bootpack.h"

extern struct FIFO8 keyfifo;
extern struct FIFO8 mousefifo;

void HariMain(void) {
    struct BOOTINFO *binfo;
    extern char hankaku[4096];
    char txt[30], mcursor[256];
    unsigned char keybuf[32], mousebuf[32];
    int mx, my, i;

    struct MOUSE_DEC mdec;

    init_gdtidt();
    init_pic();
    io_sti();

    init_keyboard();
    enable_mouse(&mdec);
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
            if (fifo8_get(&keyfifo, &i) < 0) {
                continue;
            }
            io_sti();

            sprintf(txt, "code  = %x", i);

            boxfill8(binfo->vram, binfo->scrnx, 30, 30, 190, 46, DARK_MIZU);
            draw_string(binfo->vram, binfo->scrnx, 30, 30, txt, WHITE);
        } else if (mousefifo.count > 0) {
            if (fifo8_get(&mousefifo, &i) < 0) {
                continue;
            }
            io_sti();

            if (mouse_decode(&mdec, i) != 1) {
                continue;
            }

            sprintf(txt, "[lcr %d %d]", mdec.x, mdec.y);
            if (mdec.btn & 0x01) {
                txt[1] = 'L';
            } else if (mdec.btn & 0x02) {
                txt[3] = 'R';
            } else if (mdec.btn & 0x04) {
                txt[2] = 'C';
            }
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

void enable_mouse(struct MOUSE_DEC *mdec) {
    wait_KBC_sendready();
    io_out8(PORT_KEYCMD, KEYCMD_SENDTO_MOUSE);
    wait_KBC_sendready();
    io_out8(PORT_KEYDAT, MOUSCMD_ENABLE);

    return;
}

int mouse_decode(struct MOUSE_DEC *mdec, unsigned char dat) {
    if (mdec->phase == 0) {
        if (dat == 0xfa) {
            mdec->phase = 1;
        }
        return 0;
    }
    else if (mdec->phase == 1) {
        if ((dat & 0xc8) == 0x08) {
            mdec->buf[0] = dat;
            mdec->phase = 2;
        }
        return 0;
    }
    else if (mdec->phase == 2) {
        mdec->buf[1] = dat;
        mdec->phase = 3;
        return 1;
    }
    else if (mdec->phase == 3) {
        mdec->buf[2] = dat;
        mdec->phase = 1;
        mdec->btn = mdec->buf[0] & 0x07;
        mdec->x = mdec->buf[1];
        mdec->y = mdec->buf[2];

        if (mdec->buf[0] & 0x10) {
            mdec->x |= 0xffffff00;
        }
        if (mdec->buf[0] & 0x20) {
            mdec->y |= 0xffffff00;
        }
        mdec->y *= -1;

        return 1;
    }
    return -1;
}