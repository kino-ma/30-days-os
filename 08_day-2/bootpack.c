#include "bootpack.h"

extern struct FIFO8 keyfifo;
extern struct FIFO8 mousefifo;

unsigned char key_table_qwerty[] = {
    0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '!', '^', 0, 0,
    'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '@', '[', 0, 0,
    'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ';', ':', 0, 0, ']',
    'Z', 'X', 'C', 'V', 'B', 'N', 'M', ',', '.', '/', '_',
    0, 0, ' '};

unsigned char key_table_dvorak[] = {
    0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '!', '^', 0, 0,
    '\'', ',', '.', 'P', 'Y', 'F', 'G', 'C', 'R', 'L', '@', '[', 0, 0,
    'A', 'O', 'E', 'U', 'I', 'D', 'H', 'T', 'N', 'S', ':', 0, 0, ']',
    ';', 'Q', 'J', 'K', 'X', 'B', 'M', 'W', 'V', 'Z', '_',
    0, 0, ' '};

void HariMain(void) {
    struct BOOTINFO *binfo;
    extern char hankaku[4096];
    char txt[30], mcursor[256], input_txt[32] = "_";
    unsigned char keybuf[32], mousebuf[32];
    int mx, my, i, input_count = 0;

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

    mx = binfo->scrnx / 2 - 16, my = binfo->scrny / 2 - 16;
    init_mouse_cursor8(mcursor, DARK_MIZU);
    putblock8_8(binfo->vram, binfo->scrnx, 16, 16, mx, my, mcursor, 16);
    draw_string(binfo->vram, binfo->scrnx, 10, 30, input_txt, WHITE);

    io_out8(PIC0_IMR, 0xf9); // PIC1とキーボードを許可(11111001)
	io_out8(PIC1_IMR, 0xef); // マウスを許可(11101111)

    while (1) {
        io_cli();
        if (keyfifo.count > 0) {
            if (fifo8_get(&keyfifo, &i) < 0) {
                boxfill8(binfo->vram, binfo->scrnx, 0, 0, binfo->scrnx, binfo->scrny, BLACK);
                continue;
            }
            io_sti();

            if (0x80 & i) {
                continue;
            }

            if (i == 0x0e) {
                if (input_count <= 0)
                    continue;
                input_txt[input_count] = 0;
                input_txt[input_count - 1] = '_';
                input_count -= input_count > 0 ? 1 : 0;
            } else if (input_count < 32) {
                input_txt[input_count] = key_table_dvorak[i];
                input_txt[input_count + 1] = '_';
                input_count += 1;
            }
            boxfill8(binfo->vram, binfo->scrnx, 10, 30, 10 + 16 * 32, 30 + 16, DARK_MIZU);
            draw_string(binfo->vram, binfo->scrnx, 10, 30, input_txt, WHITE);
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

            boxfill8(binfo->vram, binfo->scrnx, mx, my, mx + 16, my + 16, DARK_MIZU);
            mx += mdec.x;
            my += mdec.y;

            if (mx < 0) {
                mx = 0;
            }
            if (my < 0) {
                my = 0;
            }
            if (mx > binfo->scrnx - 16) {
                mx = binfo->scrnx - 16;
            }
            if (my > binfo->scrny - 16) {
                my = binfo->scrny - 16;
            }

            boxfill8(binfo->vram, binfo->scrnx, 30, 70, 190, 86, DARK_MIZU);
            putblock8_8(binfo->vram, binfo->scrnx, 16, 16, mx, my, mcursor, 16);
        }
        else {
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
        return 0;
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