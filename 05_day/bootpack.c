#define BLACK			0
#define LIGHT_RED		1
#define LIGHT_GREEN 	2
#define LIGHT_YELLOW	3
#define LIGHT_BLUE		4
#define LIGHT_PURPLE	5
#define LIGHT_MIZU		6
#define WHITE			7
#define LIGHT_GRAY		8
#define DARK_RED		9
#define DARK_GREEN		10
#define DARK_YELLOW		11
#define DARK_BLUE		12
#define DARK_PURPLE		13
#define DARK_MIZU		14
#define DARK_GRAY		15

void io_hlt(void);
void io_cli(void);
void io_out8(int port, int data);
int io_load_eflags(void);
void io_store_eflags(int eflags);

void init_screen(unsigned char *vram, int display_w, int display_h);

void draw_point(unsigned char *vram, int display_w, int x, int y, unsigned char color);
void draw_line_vrt(unsigned char *vram, int display_w, int x, int y, int y2, unsigned char color);
void draw_line_hrz(unsigned char *vram, int display_w, int x, int y, int x2, unsigned char color);
void boxfill8(unsigned char *vram, int display_w, int x, int y, int w, int h, unsigned char color);

void init_palette(void);
void set_palette(int start, int end, unsigned char *rgb);

struct BOOTINFO {
	char cyls, leds, vmode, reserve;
	short scrnx, scrny;
	char *vram;
};

void HariMain(void)
{
	char *vram;
	struct BOOTINFO *binfo;

	init_palette();

	binfo = (struct BOOTINFO *)0x0ff0;

	init_screen(binfo->vram, binfo->scrnx, binfo->scrny);

	while (1) {
		io_hlt();
	}
}

void init_screen(unsigned char *vram, int display_w, int display_h) {

	// debug
	boxfill8(vram, display_w, 0, 0, display_w - 1, display_h - 1, LIGHT_RED);

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

void boxfill8(unsigned char *vram, int display_w, int x, int y, int x2, int y2, unsigned char color)
{
	int cur_x, cur_y;

	for (cur_x = x; cur_x < x2; cur_x += 1) {
		for (cur_y = y; cur_y < y2; cur_y += 1) {
			draw_point(vram, display_w, cur_x, cur_y, color);
		}
	}

	return;
}

void draw_box(unsigned char *vram, int display_w, int x, int y, int x2, int y2, unsigned char color)
{
	draw_line_hrz(vram, display_w, x,  y,  x2, color);
	draw_line_hrz(vram, display_w, x,  y2, x2, color);
	draw_line_vrt(vram, display_w, x,  y,  y2, color);
	draw_line_vrt(vram, display_w, x2, y,  y2, color);
}

void draw_line_hrz(unsigned char *vram, int display_w, int x, int y, int x2, unsigned char color)
{
	int cur_x;
	for (cur_x = x; cur_x <= x2; cur_x += 1) {
		draw_point(vram, display_w, cur_x, y, color);
	}
	return;
}

void draw_line_vrt(unsigned char *vram, int display_w, int x, int y, int y2, unsigned char color)
{
	int cur_y;
	for (cur_y = y; cur_y <= y2; cur_y += 1) {
		draw_point(vram, display_w, x, cur_y, color);
	}
	return;

}

void draw_point(unsigned char *vram, int display_w, int x, int y, unsigned char color) 
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