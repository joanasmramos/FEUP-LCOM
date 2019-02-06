#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/driver.h>
#include <machine/int86.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <stdio.h>
#include "video_gr.h"
#include "vbe.h"
#include "timer.h"
#include "i8254.h"
#include "i8042.h"
#include "KBD.h"
#include "test5.h"
#include <math.h>
#include "pixmap.h"
#include "read_xpm.c"
#include "lmlib.h"

#define VBE_MODE 0x105

int video_test_init(unsigned short mode, unsigned short delay) {

	unsigned char *vid_mem = vg_init(mode);

	sleep(delay);

	vg_exit();

	printf("Physical Address of VRAM: 0x%x\n", vid_mem);

	return 0; //(void*)vid_mem;
}

int video_test_square(unsigned short x, unsigned short y, unsigned short size, unsigned long color) {
	int ipc_status, irq_set_kbd = kbd_subscribe_int();
	message msg;
	int r;

	unsigned long scancode = 0;

	if (irq_set_kbd == -1)
		return -1;

	if (vgdrawing_square(x, y, size, color) != OK) {
		if(vg_exit()==1)
			return -1;
		if (kbd_unsubscribe_int() == -1)
			return -1;
		return -1;
	}

	while (scancode != ESC) {
		/* Get a request message. */
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) {
			/* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & irq_set_kbd) { /* subscribed interrupt */
					scancode = reading();
				}
				break;

			default:
				break; /* no other notifications expected: do nothing */
			}
		} else { /* received a standard message, not a notification */
			/* no standard messages expected: do nothing */
		}
	}

	if (kbd_unsubscribe_int() == -1)
		return -1;

	if(vg_exit()==1)
		return -1;

	return 0;
}

int video_test_line(unsigned short xi, unsigned short yi, unsigned short xf, unsigned short yf, unsigned long color) {
	int ipc_status, irq_set_kbd = kbd_subscribe_int();
	message msg;
	int r;

	unsigned long scancode = 0;

	if (irq_set_kbd == -1)
		return -1;

	if(vgdrawing_line(xi, yi, xf, yf, color)==-1){
		if(vg_exit()==1)
			return -1;
		if(kbd_unsubscribe_int()==-1)
			return -1;
		return -1;
	}


	while (scancode != ESC) {
		/* Get a request message. */
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) {
			/* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & irq_set_kbd) { /* subscribed interrupt */
					scancode = reading();
				}
				break;

			default:
				break; /* no other notifications expected: do nothing */
			}
		} else { /* received a standard message, not a notification */
			/* no standard messages expected: do nothing */
		}
	}

	if (kbd_unsubscribe_int() == -1)
		return -1;

	if(vg_exit()==1)
		return -1;

	return 0;
}

int test_xpm(char *xpm[], unsigned short xi, unsigned short yi) {

	int ipc_status, irq_set_kbd = kbd_subscribe_int();
	message msg;
	int r;

	unsigned long scancode = 0;

	if (irq_set_kbd == -1)
		return -1;

	vgsprite_displaying(xpm, xi, yi);
	video_dump_fb();

	while (scancode != ESC) {
		/* Get a request message. */
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) {
			/* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & irq_set_kbd) { /* subscribed interrupt */
					scancode = reading();
				}
				break;

			default:
				break; /* no other notifications expected: do nothing */
			}
		} else { /* received a standard message, not a notification */
			/* no standard messages expected: do nothing */
		}
	}

	if (kbd_unsubscribe_int() == -1)
		return -1;

	if (vg_exit() == 1)
		return -1;

	return 0;
}

int test_move(char *xpm[], unsigned short xi, unsigned short yi,
		unsigned short xf, unsigned short yf, short s, unsigned short f) {

	int ipc_status, irq_set_kbd = kbd_subscribe_int(), irq_set_timer=timer_subscribe_int(),ctr=0, ctrframes=0;
	message msg;
	int r;
	unsigned short x=xi, y=yi;

	unsigned long scancode = 0;

	if (irq_set_kbd == -1)
		return -1;

	if(irq_set_timer==-1)
		return -1;

	if(vg_init(VBE_MODE)==NULL){
		if(kbd_unsubscribe_int()==-1)
			return -1;
		if(timer_unsubscribe_int()==-1)
			return -1;
		return -1;
	}

	if(xi!=xf && yi!=yf){
		printf("Only horizontal/vertical movements allowed. \n");
		if(vg_exit()==1)
			return -1;
		if(timer_unsubscribe_int()==-1)
			return -1;
		if(kbd_unsubscribe_int()==-1)
			return -1;
		return 0;
	}
	vgsprite_displaying2(x,y, xpm);

	if(xi==xf && yi==yf){
		if(vg_exit()==1)
			return -1;
		if(timer_unsubscribe_int()==-1)
			return -1;
		if(kbd_unsubscribe_int()==-1)
			return -1;
		return 0;
	}

	while (scancode != ESC) {
		/* Get a request message. */
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) {
			/* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & irq_set_kbd) { /* subscribed interrupt */
					scancode = reading();
				}
				if(msg.NOTIFY_ARG & irq_set_timer){
					ctr++;
					if(ctr%60==0){
						ctrframes++;
						if(xi==xf){ // movimento vertical
							if(s>0)
								y=y+s;
							if(s<0){
								if(ctrframes%(abs(s)))
									y++;
							}
						}
						if(yi==yf){ // movimento horizontal
							if(s>0)
								x=x+s;
							if(s<0){
								if(ctrframes%(abs(s)))
									x++;
							}

						}
						clear_Screen();
						vgsprite_displaying2(x,y,xpm);
					}

				}
				break;

			default:
				break; /* no other notifications expected: do nothing */
			}
		} else { /* received a standard message, not a notification */
			/* no standard messages expected: do nothing */
		}
	}

	if (kbd_unsubscribe_int() == -1)
		return -1;

	if (vg_exit() == 1)
		return -1;

	return 0;

}

int test_controller() {
	//vbe_get_info_block();

	return 0;
}


