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


#define ADR_IDT			0x0026f800
#define LIMIT_IDT		0x000007ff
#define ADR_GDT			0x00270000
#define LIMIT_GDT		0x0000ffff
#define ADR_BOTPAK		0x00280000
#define LIMIT_BOTPAK	0x0007ffff
#define AR_DATA32_RW	0x4092
#define AR_CODE32_ER	0x409a
#define ADR_BOOTINFO    0x0ff0
#define AR_INTGATE32	0x008e


/* asm */
void io_hlt(void);
void io_cli(void);
void io_sti(void);
void io_out8(int port, int data);
int io_in8(int port);
int io_load_eflags(void);
void io_store_eflags(int eflags);
void load_gdtr(int limit, int addr);
void load_idtr(int limit, int addr);
void asm_inthandler21(void);
void asm_inthandler27(void);
void asm_inthandler2c(void);


/* graphic.c */
void init_screen(char *vram, int display_w, int display_h);

void init_mouse_cursor8(char *mouse, char bc);
void draw_string(char *vram, int display_w, int x, int y, char *text, unsigned char color);
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


/* dsctbl.c */
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


/* extern */
void sprintf(char *str, char *fmt, ...);


/* int.c */
void init_pic(void);
void inthandler21(int *esp);
void inthandler27(int *esp);
void inthandler2c(int *esp);
#define PIC0_ICW1		0x0020
#define PIC0_OCW2		0x0020
#define PIC0_IMR		0x0021
#define PIC0_ICW2		0x0021
#define PIC0_ICW3		0x0021
#define PIC0_ICW4		0x0021
#define PIC1_ICW1		0x00a0
#define PIC1_OCW2		0x00a0
#define PIC1_IMR		0x00a1
#define PIC1_ICW2		0x00a1
#define PIC1_ICW3		0x00a1
#define PIC1_ICW4		0x00a1
#define PORT_KEYDAT     0x0060

void init_pic(void);