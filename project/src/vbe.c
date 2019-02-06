#include <stdint.h>
#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include "vbe.h"

#define LINEAR_MODEL_BIT 14

#define PB2BASE(x) (((x) >> 4) & 0x0F000)
#define PB2OFF(x) ((x) & 0x0FFFF)

int vbe_get_mode_info(unsigned short mode, vbe_mode_info_t *vmi_p) {
	mmap_t buffer;
	struct reg86u r;

	lm_init();

	lm_alloc(sizeof(vbe_mode_info_t), &buffer);

	r.u.w.ax = 0x4F01; /* VBE get mode info*/
	/*translate the buffer linear address to a far pointer */
	r.u.w.es = PB2BASE(buffer.phys); /* set a segment base*/
	r.u.w.di = PB2OFF(buffer.phys); /*set the offset accordingly */
	r.u.w.cx = mode;

	r.u.b.intno = 0x10; 		//Interrupt

	if (sys_int86(&r) == OK) {
		*vmi_p = *((vbe_mode_info_t *) buffer.virtual);
		lm_free(&buffer);
		return 0;
	}

	return 1;
}

