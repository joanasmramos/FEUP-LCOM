#include "video_gr.h"

/* Private global variables */

static unsigned char *video_mem; /* Process (virtual) address to which VRAM is mapped */
static unsigned char *double_buffer;
static unsigned char *triple_buffer;

static unsigned h_res; /* Horizontal screen resolution in pixels */
static unsigned v_res; /* Vertical screen resolution in pixels */
static unsigned bits_per_pixel; /* Number of VRAM bits per pixel */
static unsigned vramSize;

int vg_exit() {
	struct reg86u reg86;

	reg86.u.b.intno = 0x10; /* BIOS video services */

	reg86.u.b.ah = 0x00; /* Set Video Mode function */
	reg86.u.b.al = 0x03; /* 80x25 text mode*/

	if (sys_int86(&reg86) != OK) {
		printf("\tvg_exit(): sys_int86() failed \n");
		return 1;
	} else
		return 0;
}

void* vg_init(unsigned short mode) {
	/*VBE graphics mode 117h, 1024x768@256, linear mode*/
	struct reg86u reg86;
	reg86.u.w.ax = 0x4F02; // VBE call, function 02 -- set VBE mode
	reg86.u.w.bx = 1 << 14 | mode; //set bit 14: linear framebuffer
	reg86.u.b.intno = 0x10;

	if (sys_int86(&reg86) != OK) {
		printf("set_vbe_mode: sys_int86() failed \n");
		return NULL;
	}

	if (reg86.u.b.ah != 0) {
		return NULL;
	}

	vbe_mode_info_t vbe_inf;
	vbe_get_mode_info(mode, &vbe_inf);

	bits_per_pixel = vbe_inf.BitsPerPixel;
	unsigned int bytepixel = bits_per_pixel / 8;

	h_res = vbe_inf.XResolution;
	v_res = vbe_inf.YResolution;
	vramSize = h_res * v_res * bytepixel;

	int r;
	struct mem_range mr;

	/* Allow memory mapping */

	mr.mr_base = (phys_bytes) vbe_inf.PhysBasePtr;

	mr.mr_limit = mr.mr_base + vramSize;

	if ((r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)) != OK)
		panic("sys_privctl (ADD_MEM) failed: %d\n", r);

	/* Map memory */
	video_mem = vm_map_phys(SELF, (void *) mr.mr_base, vramSize);
	if (video_mem == MAP_FAILED)
		panic("couldn't map video memory");

	double_buffer = (unsigned char*) malloc(vramSize);
	triple_buffer = (unsigned char*) malloc(vramSize);

	return video_mem;

}

int pixel_coloring(unsigned short x, unsigned short y, unsigned long color) {

	if (y >= v_res || x>= h_res) //Out of limits!!
			{
		return 1;
	}

	unsigned int bytepixel = bits_per_pixel / 8;
	video_mem += (y* h_res + x)*bytepixel;

	*video_mem = (char) color;
	return 0;
}


int clear_Screen(){
	int i = 0;
	int j = 0;

	for (i = 0; i < V_RES; i++) {
			for (j = 0; j < H_RES; j++) {
				pixel_coloring(i, j, 0); //xi + j - width
			}
		}

	return 0; //SUCCESS
}

void* getVideoMem(){
	return video_mem;
}

void* getDoubleBuffer(){
	return double_buffer;
}

unsigned getHorResolution(){
	return h_res;
}

unsigned getVerResolution(){
	return v_res;
}

unsigned getVramSize(){
	return vramSize;
}

void doubleBuffering(){
	memcpy(video_mem,double_buffer,vramSize);
}
