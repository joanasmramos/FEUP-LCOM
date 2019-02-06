#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>

#include "vbe.h"
#include "lmlib.h"

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
		lm_free(&r);
		*vmi_p = *((vbe_mode_info_t *) buffer.virtual);
		return 0;
	}

	return 1;
}

//int vbe_get_info_block() {
//
//	mmap_t buffer;
//	struct reg86u r;
//
//	lm_init();
//
//	lm_alloc(sizeof(vbe_info_block_t), &buffer);
//
//	VbeInfoBlock cntrllr;
//
//	r.u.w.ax = 0x4F00; /* VBE get mode info */
//	/* translate the buffer linear address to a far pointer */
//	r.u.w.es = PB2BASE(buf.phys); /* set a segment base */
//	r.u.w.di = PB2OFF(buf.phys); /* set the offset accordingly */
//	r.u.b.intno = 0x10;
//
//	if (sys_int86(&r) == OK) {
//		lm_free(&r);
//		*vmi_p = *((vbe_mode_info_t *) buffer.virtual);
//
//		cntrllr = ((VbeInfoBlock) buffer.virtual);
//
//		printf("%d.%d\n", cntrllr.VbeVersion >> 8, cntrllr.VbeVersion & 0x0FF); // due to it being a doubleword, we need to shift
//
//		uint16_t *ptrVideoModes = (uint16_t *) cntrllr.Reserved;
//
//		while (*ptrVideoModes != 0x0FFFF) {
//			printf("0x%x:", *ptrVideoModes);
//			ptrVideoModes++;
//		}
//		printf("\n");
//
//		printf("%d\n", cntrllr.TotalMemory * 64);
//
//		lm_free(&buffer);
//
//		return 0;
//	}
//
//	return 1;
//
//}
