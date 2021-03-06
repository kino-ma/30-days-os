#include "bootpack.h"


void draw_string(char *vram, int display_w, int x, int y, char *text, unsigned char color) {
    extern char hankaku[4096];
    int i;
    for (i = 0; text[i]; i += 1) {
        putfont8(vram, display_w, x + i * 8, y, hankaku + text[i] * 16, color);
    }
}
void putfont8(char *vram, int display_w, int x, int y, char *font, unsigned char color) {
    int i;
    for (i = 0; i < 16; i += 1) {
        draw_byte(vram, display_w, x, y + i, font[i], color);
    }
}

void init_mouse_cursor8(char *mouse, char bc) {
    static char cursor[16][16] = {
        "**************..",
        "*OOOOOOOOOOO*...",
        "*OOOOOOOOOO*....",
        "*OOOOOOOOO*.....",
        "*OOOOOOOO*......",
        "*OOOOOOO*.......",
        "*OOOOOOO*.......",
        "*OOOOOOOO*......",
        "*OOOO**OOO*.....",
        "*OOO*..*OOO*....",
        "*OO*....*OOO*...",
        "*O*......*OOO*..",
        "**........*OOO*.",
        "*..........*OOO*",
        "............*OO*",
        ".............***"
    };
    int x, y;

    for (y = 0; y < 16; y++) {
        for (x = 0; x < 16; x++) {
            if (cursor[y][x] == '*') {
                mouse[y * 16 + x] = BLACK;
            }
            if (cursor[y][x] == 'O') {
                mouse[y * 16 + x] = WHITE;
            }
            if (cursor[y][x] == '.') {
                mouse[y * 16 + x] = bc;
            }
        }
    }
    return;
}

void putblock8_8(
    char *vram, int vxsize,
    int pxsize, int pysize, int px0, int py0, char *buf, int bxsize)
{
    int x, y;
    for (y = 0; y < pysize; y++) {
        for (x = 0; x < pxsize; x++) {
            vram[(py0 + y) * vxsize + (px0 + x)] = buf[y * bxsize + x];
        }
    }
    return;
}

void init_screen(char *vram, int display_w, int display_h) {

    boxfill8(vram,      display_w, 0,              0, display_w - 1, display_h - 29, DARK_MIZU);
    draw_line_hrz(vram, display_w, 0, display_h - 28, display_w - 1,                 LIGHT_GRAY);
    draw_line_hrz(vram, display_w, 0, display_h - 27, display_w - 1,                 WHITE);
    boxfill8(vram,      display_w, 0, display_h - 26, display_w - 1,  display_h - 1, LIGHT_GRAY);

    // 左下の箱
    draw_line_hrz(vram, display_w, 3,  display_h - 24,            59, WHITE);
    draw_line_hrz(vram, display_w, 3,  display_h -  4,            59, DARK_GRAY);
    draw_line_vrt(vram, display_w, 2,  display_h - 24, display_h - 4, WHITE);
    draw_line_vrt(vram, display_w, 59, display_h - 23, display_h - 5, DARK_GRAY);

    //影
    draw_line_hrz(vram, display_w, 2,  display_h - 3,  59,            BLACK);
    draw_line_vrt(vram, display_w, 60, display_h - 24, display_h - 3, BLACK);

    // 右下の箱
    draw_line_hrz(vram, display_w, display_w - 47, display_h - 24, display_w - 4, DARK_GRAY);
    draw_line_hrz(vram, display_w, display_w - 47, display_h - 3,  display_w - 4, WHITE);
    draw_line_vrt(vram, display_w, display_w - 47, display_h - 23, display_h - 4, DARK_GRAY);
    draw_line_vrt(vram, display_w, display_w - 3,  display_h - 24, display_h - 3, WHITE);

}

void boxfill8(char *vram, int display_w, int x, int y, int x2, int y2, unsigned char color)
{
    int cur_x, cur_y;

    for (cur_x = x; cur_x < x2; cur_x += 1) {
        for (cur_y = y; cur_y < y2; cur_y += 1) {
            draw_point(vram, display_w, cur_x, cur_y, color);
        }
    }

    return;
}

void draw_box(char *vram, int display_w, int x, int y, int x2, int y2, unsigned char color)
{
    draw_line_hrz(vram, display_w, x,  y,  x2, color);
    draw_line_hrz(vram, display_w, x,  y2, x2, color);
    draw_line_vrt(vram, display_w, x,  y,  y2, color);
    draw_line_vrt(vram, display_w, x2, y,  y2, color);
}

void draw_line_hrz(char *vram, int display_w, int x, int y, int x2, unsigned char color)
{
    int cur_x;
    for (cur_x = x; cur_x <= x2; cur_x += 1) {
        draw_point(vram, display_w, cur_x, y, color);
    }
    return;
}

void draw_line_vrt(char *vram, int display_w, int x, int y, int y2, unsigned char color)
{
    int cur_y;
    for (cur_y = y; cur_y <= y2; cur_y += 1) {
        draw_point(vram, display_w, x, cur_y, color);
    }
    return;

}

void draw_byte(char *vram, int display_w, int x, int y, char data, unsigned char color) {
    int i;
    for (i = 0; i < 8; i += 1) {
        if ((0x80 >> i) & data) {
            draw_point(vram, display_w, x + i, y, color);
        }
    }
}

void draw_point(char *vram, int display_w, int x, int y, unsigned char color) 
{
    vram[x + y * display_w] = color;
    return;
}

void init_palette(void)
{
    static unsigned char table_rgb[16 * 3] = {
        0x00, 0x00, 0x00, /* 0: 黒 */
        0xff, 0x00, 0x00, /* 1: 明るい赤 */
        0x00, 0xff, 0x00, /* 2: 明るい緑 */
        0xff, 0xff, 0x00, /* 3: 明るい黄 */
        0x00, 0x00, 0xff, /* 4: 明るい青 */
        0xff, 0x00, 0xff, /* 5: 明るい紫 */
        0x00, 0xff, 0xff, /* 6: 明るい水色 */
        0xff, 0xff, 0xff, /* 7: 白 */
        0xc6, 0xc6, 0xc6, /* 8: 明るい灰色 */
        0x84, 0x00, 0x00, /* 9: 暗い赤 */
        0x00, 0x84, 0x00, /* 10: 暗い緑 */
        0x84, 0x84, 0x00, /* 11: 暗い黄色 */
        0x00, 0x00, 0x84, /* 12: 暗い青 */
        0x84, 0x00, 0x84, /* 13: 暗い紫 */
        0x00, 0x84, 0x84, /* 14: 暗い水色 */
        0x84, 0x84, 0x84, /* 15: 暗い灰色 */
    };

    set_palette(0, 15, table_rgb);

    return;
}

void set_palette(int start, int end, unsigned char *rgb)
{
    int i, eflags;
    eflags = io_load_eflags();
    io_cli();

    io_out8(0x03c8, start);

    for (i = start; i <= end; i++) {
        io_out8(0x03c9, rgb[0] / 4);
        io_out8(0x03c9, rgb[1] / 4);
        io_out8(0x03c9, rgb[2] / 4);
        rgb += 3;
    }

    io_store_eflags(eflags);
    return;
}
