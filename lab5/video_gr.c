#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include <sys/mman.h>
#include <sys/types.h>
#include "i8042.h"
#include "pixmap.h"
#include "read_xpm.h"
#include "vbe.h"
#include "timer.h"
#include "KBD.h"
#include "video_gr.h"



/* Constants for VBE 0x105 mode */

#define VRAM_PHYS_ADDR	0xF0000000
// the screen is a set of V_RES lines each of which with H_RES pixels
#define H_RES           1024
#define V_RES		  	768
#define BITS_PER_PIXEL	8
#define VBE_MODE		0x105

/* Private global variables */

static char *video_mem; /* Process (virtual) address to which VRAM is mapped */

static unsigned h_res; /* Horizontal screen resolution in pixels */
static unsigned v_res; /* Vertical screen resolution in pixels */
static unsigned bits_per_pixel; /* Number of VRAM bits per pixel */

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
	unsigned int vramSize;

	/*VBE graphics mode 105h, 1024x768@256, linear mode*/
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
	unsigned int bitpixel = bits_per_pixel / 8;

	h_res = vbe_inf.XResolution;
	v_res = vbe_inf.YResolution;
	vramSize = h_res * v_res * bitpixel;

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

	return video_mem;

}

int vgdrawing_square(unsigned short x, unsigned short y, unsigned long size, unsigned long color)
{
	int i, j;

	video_mem = vg_init(VBE_MODE);

	if(x < 0 || y < 0 || y + size > v_res || x + size > h_res) //Out of limits!!
	{
		return -1;
	}

	x = h_res/2 + x - size/2;
	y = v_res/2 + y - size/2;
	for(i = 0; i < size; i++)
	{
		for(j = 0; j < size; j++)
		{
			video_mem[(y + i)*h_res + (x + j)] = color;
		}
	}
	video_dump_fb();
	return 0; //SUCCESS
}

static void swap(unsigned long* a, unsigned long* b)
{
	int t=*a;
	*a=*b;
	*b=t;
}

int vgdrawing_line(unsigned short xi, unsigned short yi, unsigned short xf, unsigned short yf, unsigned long color){
	// y=m*x

	vg_init(VBE_MODE); // changing to video mode 0x105

	// como as variaveis xi, yi, xf, yf são unsigned, são interpretadas como positivas sempre

	if(xi>h_res-1||yi>v_res-1||xf>h_res-1||yf>v_res-1) // considera-se que (0, 0) é o canto esquerdo superior e
		return -1;									   // (hres-1, vres-1) o canto inferior direito


	xi=(float) xi;yi=(float)yi; xf=(float)xf; yf=(float)yf; float temp;
	float dx=xf-xi, dy=yf-yi, steps, xinc, yinc, x=xi,y=yi;

	if(xi==xf){ // linha vertical
		if(yi>yf){
			temp=yi;
			yi=yf;
			yf=temp;
		}
		for (yi=yi;yi<=yf;yi++){
			pixel_coloring(xi,yi,color);
		}
		video_dump_fb();
		return 0;
	}

	if(yi==yf){ // linha horizontal
		if(xi>xf){
			temp=xi;
			xi=xf;
			xf=temp;
		}
		for(xi=xi;xi<=xf;xi++){
			pixel_coloring(xi,yi,color);
		}
		video_dump_fb();
		return 0;
	}

	if(xi==xf && yi==yf){ // apenas um ponto
		pixel_coloring(xi,yi,color);
		video_dump_fb();
		return 0;
	}

	float declive = dy/dx; // declive de uma reta

	for(xi=xi;xi<=xf;xi++){
		yi=xi*declive;
		pixel_coloring(xi,yi,color);
	}

//	if(abs(dx)>abs(dy))
//		steps=abs(dx);
//	else steps=abs(dy);
//
//	xinc=dx/(float)steps;
//	yinc=dy/(float)steps;
//
//	int i;
//	for(i=0;i<steps;i++){
//		x+=xinc;
//		y+=yinc;
//		if(x>0)
//			x=(long) (x+0.5);
//		else x=(long)(x-0.5);
//		if(y>0)
//			y=(long)(y+0.5);
//		else y=(long)(y-0.5);
//		pixel_coloring(x,y,color);
//	}

	video_dump_fb();
	return 0;
}


int pixel_coloring(unsigned short x, unsigned short y, unsigned long color) {

	if (y >= v_res || x>= h_res) //Out of limits!!
			{
		return 1;
	}

	char *sprite = video_mem;
	unsigned int bitpixel = bits_per_pixel / 8;
	sprite += (y* h_res + x)*bitpixel;

	*sprite = (char) color;
	return 0;
}

int vgsprite_displaying(char *xpm[],unsigned short xi, unsigned short yi){
	vg_init(VBE_MODE);

	int i = 0;
	int j = 0;

	int width, height;

	char* sprt = read_xpm(xpm, &width, &height);

	for (i = 0; i < height; i++) {
			for (j = 0; j < width; j++) {
				char* color = sprt + i * width + j;
				pixel_coloring(j+xi, i+yi, *color); //xi + j - width
			}
		}

	free(sprt);
	return 0; //SUCCESS
}

int vgsprite_displaying2(unsigned short xi, unsigned short yi, char *xpm[]){
	int i = 0;
	int j = 0;

	int width, height;

	char* sprt = read_xpm(xpm, &width, &height);

	for (i = 0; i < height; i++) {
			for (j = 0; j < width; j++) {
				char* color = sprt + i * width + j;
				pixel_coloring(j+xi, i+yi, *color); //xi + j - width
			}
		}

	free(sprt);
	return 0; //SUCCESS
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


