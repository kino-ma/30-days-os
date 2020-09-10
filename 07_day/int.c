#include "bootpack.h"

void init_pic(void) {
    io_out8(PIC0_IMR, 0xff); //割り込み拒否
    io_out8(PIC1_IMR, 0xff); //割り込み拒否

    io_out8(PIC0_ICW1, 0x11);   // エッジトリガモード
    io_out8(PIC0_ICW2, 0x20);   // IRQ0-7はINT20-27で受ける
    io_out8(PIC0_ICW3, 1 << 2); // PIC1はIRQ2で接続
    io_out8(PIC0_ICW4, 0x01);   // ノンバッファモード

    io_out8(PIC1_ICW1, 0x11);   // エッジトリガモード
    io_out8(PIC1_ICW2, 0x28);   // IRQ8-15はINT28-2fで受ける
    io_out8(PIC1_ICW3, 2);      // PIC1はIRQ2で接続
    io_out8(PIC1_ICW4, 0x01);   // ノンバッファモード

    io_out8(PIC0_IMR, 0xfb);    // 11111011 PIC1以外の割込みを受け付けない
    io_out8(PIC1_IMR, 0xff);    // 11111111 全ての割込みを受け付けない

    return;
}

// キーボードの割り込み
void inthandler21(int *esp) {
    struct BOOTINFO *binfo = (struct BOOTINFO *)ADR_BOOTINFO;
    unsigned char data, s[4];
    io_out8(PIC0_OCW2, 0x61);
    data = io_in8(PORT_KEYDAT);

    sprintf(s, "%d", data);

    init_screen(binfo->vram, binfo->scrnx, binfo->scrny);
    boxfill8(binfo->vram, binfo->scrnx, 0, 0, 32 * 8 - 1, 15, BLACK);
    draw_string(binfo->vram, binfo->scrnx, 0, 0, s, WHITE);

    return;
}

// マウスの割り込み
void inthandler2c(int *esp) {
    struct BOOTINFO *binfo = (struct BOOTINFO *)ADR_BOOTINFO;
    init_screen(binfo->vram, binfo->scrnx, binfo->scrny);
    boxfill8(binfo->vram, binfo->scrnx, 0, 16, 15, 31, DARK_MIZU);
    draw_string(binfo->vram, binfo->scrnx, 0, 0, "INT C2 (IRQ-12) : PS/2 mouse", WHITE);
    while (1) {
        io_hlt();
    }
}

void inthandler27(int *esp) {
    io_out8(PIC0_OCW2, 0x67);
    return;
}