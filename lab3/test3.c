#include <minix/sysutil.h>
#include <minix/drivers.h>
#include <minix/driver.h>
#include "i8042.h"
#include "KBD.h"
#include "i8254.h"
#include "timer.h"

//PROTOTYPING
int kbd_test_scan(unsigned short assembly);
int kbd_test_poll();
int kbd_test_timed_scan(unsigned short n);

extern unsigned long kbd_assmhandler();
extern unsigned char scancoding;
extern unsigned char reading_error;


int kbd_test_scan(unsigned short assembly) {
	int r, ipc_status, irq_set = kbd_subscribe_int();
	message msg;

	unsigned long scancode=0;
	int onebyte=1;

	if(irq_set==-1)
		return -1;

	while(scancode != ESC)
	{ // during the interval defined by the passed argument
			/* Get a request message. */
			if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
				printf("kbd_assmhandlerdriver_receive failed with: %d", r);
				continue;
			}
			if (is_ipc_notify(ipc_status)) { /* received notification */
				switch (_ENDPOINT_P(msg.m_source)) {
				case HARDWARE: /* hardware interrupt notification */
					if (msg.NOTIFY_ARG & irq_set) { /* subscribed interrupt */
						if (assembly==0){
							scancode = reading();
						}

						else if (assembly==1)
						{
							kbd_assmhandler();
							if(reading_error == -1)
								return -1;
							else
								scancode = scancoding;
						}

						if (scancode == TWO_B){
							onebyte = 0;
						}

						else
							if (onebyte){
								if((scancode & BIT(7))==BIT(7))
									printf("Breakcode: 0x%02x \n\n",scancode);
								else printf("Makecode: 0x%02x \n",scancode);
							}
							else {
								if((scancode & BIT(7))==BIT(7))
									printf("Breakcode: 0xe0 0x%02x \n\n",scancode);
								else printf("Makecode: 0xe0 0x%02x \n",scancode);

								onebyte = 1;
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
	if (assembly == 0) printf("Number of sys_inb kernel calls: %d \n", getCnt());

	if(kbd_unsubscribe_int()==-1)
		return -1;
	else return 0;
}


int kbd_test_poll() {

	unsigned long scancode = 0, data;
	int onebyte = 1;

	while (scancode != ESC) { // during the interval defined by the passed argument
		scancode = reading();

		if (scancode != -1) {

			if (onebyte) {
				if ((scancode & BIT(7)) == BIT(7))
					printf("Breakcode: 0x%02x \n\n", scancode);
				else
					printf("Makecode: 0x%02x \n", scancode);
			} else {
				if (scancode == TWO_B)
					onebyte = 0;
				else if ((scancode & BIT(7)) == BIT(7))
					printf("Breakcode: 0xe0 0x%02x \n\n", scancode);
				else
					printf("Makecode: 0xe0 0x%02x \n", scancode);
			}
		}
	}

	printf("Number of sys_inb kernel calls: %d \n", getCnt());

	kbd_tosend(KBC_CMD_REG, RDCOMMAND);

	if (sys_inb(KBC_CMD_REG, &data) != OK)
		return 1;

	data = (data | BIT(0));
	kbd_tosend(KBC_CMD_REG, WRTCOMMAND);
	kbd_tosend(WRTCOMMAND, data);

	return 0;
}


int kbd_test_timed_scan(unsigned short n) {
	int r, ipc_status, kbd_irq_set = kbd_subscribe_int(), timer_irq_set=timer_subscribe_int();
	message msg;

	unsigned long scancode=0;
	int onebyte=1, counter=0;

	timer_set_frequency(0, FREQ);

	if(kbd_irq_set==-1 || timer_irq_set == -1)
		return -1;

	while((scancode != ESC) & (counter < 60*n))
	{ // during the interval defined by the passed argument
			/* Get a request message. */
			if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
				printf("kbd_assmhandlerdriver_receive failed with: %d", r);
				continue;
			}
			if (is_ipc_notify(ipc_status)) { /* received notification */
				switch (_ENDPOINT_P(msg.m_source)) {
				case HARDWARE: /* hardware interrupt notification */
					if (msg.NOTIFY_ARG & timer_irq_set) { /* timer subscribed interrupt */
						counter++;
					}


					if (msg.NOTIFY_ARG & kbd_irq_set) { /* keyboard subscribed interrupt */
						scancode = reading();
						if (scancode == -1) break;
						counter=0;

						if (scancode == TWO_B){
							onebyte = 0;
						}

						else
							if (onebyte){
								if((scancode & BIT(7))==BIT(7))
									printf("Breakcode: 0x%02x \n\n",scancode);
								else printf("Makecode: 0x%02x \n",scancode);
							}
							else {
								if((scancode & BIT(7))==BIT(7))
									printf("Breakcode: 0xe0 0x%02x \n\n",scancode);
								else printf("Makecode: 0xe0 0x%02x \n",scancode);

								onebyte = 1;
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

	if (scancode==ESC)
		printf("Pressed ESC key - finished test_timed_scan()\n");
	else
		printf("You've been inactive for %d seconds - finished test_timed_scan()\n", n);

	if(kbd_unsubscribe_int()==-1)
		return -1;

	if(timer_unsubscribe_int() ==-1)
		return -1;

	return 0;
}



