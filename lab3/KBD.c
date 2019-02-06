#include <minix/sysutil.h>
#include <minix/driver.h>
#include "KBD.h"
#include "i8042.h"

#define LAB3

int kbd_hookID = 3;
unsigned int cnt = 0;

//SUBSCRIBE AND UNSUBSCRIBE FUNCTIONS
int kbd_subscribe_int() {
	int kbd_temphookID = kbd_hookID;
	//because we don't want the original value of hook_id to change, due to it being a global variable.

	int stat = sys_irqsetpolicy(KBD_IRQ, (IRQ_REENABLE | IRQ_EXCLUSIVE),
			&kbd_hookID);

	if (OK != stat) {
		printf("ERROR executing sys_ieqsetpolicy()");
		return -1;
	}

	stat = sys_irqenable(&kbd_hookID);
	if (OK != stat) {
		printf("ERROR executing sys_irqenable()");
		return -1;
	}
	return BIT(kbd_temphookID); //SUCCESS
}

int kbd_unsubscribe_int() {
	int stat = sys_irqdisable(&kbd_hookID);

	if (OK != stat) {
		printf("ERROR executing sys_irqdisable()");
		return -1;
	}

	stat = sys_irqrmpolicy(&kbd_hookID);
	if (OK != stat) {
		printf("ERROR executing sys_ieqsetpolicy()");
		return -1;
	}

	return 0; //SUCCESS
}

unsigned long reading() {
	unsigned long stat;
	unsigned long data;

	while (1) {
		sys_inb(STAT_REG, &stat);
#ifdef LAB3
			cnt++;
#endif
		if (stat & OBF) {
#define LAB3
			sys_inb(OUT_BUF, &data);
#ifdef LAB3
				cnt++;
#endif
			if ((stat & (PAR_ERR | TO_ERR)) == 0) // parity error/timeout error
				return data;
			else
				 {return -1;}
		}
		tickdelay(micros_to_ticks(WAIT_KBC));
	}
}

unsigned long kbd_tosend(unsigned long reg, unsigned long command) {
	unsigned long stat;

	while (1) {

		sys_inb(STAT_REG, &stat);

		if (stat & IBF) {

			if (sys_outb(reg, command) != OK){
				printf("Failed executing sys_outb...\n");
				return -1;
			}
			else
				return 0;
		}
		tickdelay(micros_to_ticks(WAIT_KBC));
	}
}

unsigned int getCnt()
{
	return cnt;
}
