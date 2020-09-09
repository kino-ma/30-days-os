#define BLACK        0
#define LIGHT_RED    1
#define LIGHT_GREEN  2
#define LIGHT_YELLOW 3
#define LIGHT_BLUE   4
#define LIGHT_PURPLE 5
#define LIGHT_MIZU   6
#define WHITE        7
#define LIGHT_GRAY   8
#define DARK_RED     9
#define DARK_GREEN   10
#define DARK_YELLOW  11
#define DARK_BLUE    12
#define DARK_PURPLE  13
#define DARK_MIZU    14
#define DARK_GRAY    15

/* asm */
void io_hlt(void);
void io_cli(void);
void io_out8(int port, int data);
int io_load_eflags(void);
void io_store_eflags(int eflags);
void load_gdtr(int limit, int addr);
void load_idtr(int limit, int addr);


/* graphic.c */
void init_screen(char *vram, int display_w, int display_h);

void init_mouse_cursor8(char *mouse, char bc);
void draw_string(char *vram, int display_w, int x, int y, char *text, char *font_data, unsigned char color);
void putfont8(char *vram, int display_w, int x, int y, char *font, unsigned char color);
void putblock8_8(
    char *vram, int vxsize,
    int pxsize, int pysize, int px0, int py0, char *buf, int bxsize);

void draw_point(char *vram, int display_w, int x, int y, unsigned char color);
void draw_byte(char *vram, int display_w, int x, int y, char font, unsigned char color);
void draw_line_vrt(char *vram, int display_w, int x, int y, int y2, unsigned char color);
void draw_line_hrz(char *vram, int display_w, int x, int y, int x2, unsigned char color);
void boxfill8(char *vram, int display_w, int x, int y, int w, int h, unsigned char color);

void init_palette(void);
void set_palette(int start, int end, unsigned char *rgb);

struct BOOTINFO {
    char cyls, leds, vmode, reserve;
    short scrnx, scrny;
    char *vram;
};

struct SEGMENT_DESCRIPTOR {
    short limit_low, base_low;
    char base_mid, access_right;
    char limit_high, base_high;
};

struct GATE_DESCRIPTOR {
    short offset_low, selector;
    char dw_count, access_right;
    short offset_high;
};


void init_gdtidt(void);
void set_segmdesc(struct SEGMENT_DESCRIPTOR *sd, unsigned int limit, int base, int ar);
void set_gatedesc(struct GATE_DESCRIPTOR *gd, int offset, int selector, int ar);


void HariMain(void) {
    struct BOOTINFO *binfo;
    extern char hankaku[4096];
    char txt[30], mcursor[256];
    int mx, my;

    init_gdtidt();
    init_palette();

    binfo = (struct BOOTINFO *)0x0ff0;

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