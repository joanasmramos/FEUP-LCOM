#include <minix/sysutil.h>
#include <minix/drivers.h>
#include "mouse.h"
#include "i8042.h"
#include "i8254.h"

int mouse_hookID = 4;
int packet[3];

//SUBSCRIBE AND UNSUBSCRIBE FUNCTIONS
int mouse_subscribe_int() {
	int mouse_temphookID = mouse_hookID;
		//because we don't want the original value of hook_id to change, due to it being a global variable, like always

		int stat = sys_irqsetpolicy(MOUSE_IRQ, (IRQ_REENABLE | IRQ_EXCLUSIVE),
				&mouse_hookID);

		if (OK != stat) {
			printf("ERROR executing sys_ieqsetpolicy()");
			return -1;
		}

		stat = sys_irqenable(&mouse_hookID);
		if (OK != stat) {
			printf("ERROR executing sys_irqenable()");
			return -1;
		}
		return BIT(mouse_temphookID); //SUCCESS
	}

int mouse_unsubscribe_int(){
	int stat = sys_irqdisable(&mouse_hookID);

		if (OK != stat) {
			printf("ERROR executing sys_irqdisable()");
			return -1;
		}

		stat = sys_irqrmpolicy(&mouse_hookID);
		if (OK != stat) {
			printf("ERROR executing sys_irqrmpolicy()");
			return -1;
		}

		return 0; //SUCCESS
	}

unsigned long mouse_reading() {
	unsigned long stat;
	unsigned long data;
	int nTries = NUMBER_OF_TRIES;

	while (nTries != 0) { //not doing while(1) in case of it not having something to read
		sys_inb(STAT_REG, &stat); // reading the status register (stored in stat)
		if (stat & OBF) {
			sys_inb(OUT_BUF, &data);
			if ((stat & (PAR_ERR | TO_ERR)) == 0) // parity error/timeout error
				return data;
			else
				 {return -1;}
		}
		nTries--;
		tickdelay(micros_to_ticks(WAIT_KBC));
	}

	return -1;
}

int mouse_writting(unsigned long reg, unsigned long command) {
	unsigned long stat;
	int nTries = NUMBER_OF_TRIES;

	while(nTries != 0)
	{
		sys_inb(KBC_CMD_REG, &stat); // reading the status register (stored in stat)

		if ((stat & IBF)==0) {
			if (sys_outb(reg, command) != OK){
				printf("Failed executing sys_outb...\n");
				return -1;
			}
			else{
				tickdelay(micros_to_ticks(WAIT_KBC));
				return 0;
				break;
			}
		}
		nTries--;
		tickdelay(micros_to_ticks(WAIT_KBC));
	}

	return -1;
}

int mouse_packet_handler(int * cntr) {
	unsigned long byte = mouse_reading();

	if(byte==ACK)
		mouse_reading();

	if (byte == -1)
		return -1;

	if (*cntr == 2){
		packet[*cntr] = byte;
		*cntr=0;
		return 0;
	}

	if (*cntr == 1){
		packet[*cntr] = byte;
		(*cntr)++;
		return 0;
	}

	if (*cntr == 0){
		if (byte & BIT(3)) {
			packet[*cntr] = byte;
			(*cntr)++;
			return 0;
		}
	}

	return -1;
}

void mouse_packet_printer(){

	printf("B1=0x%02x B2=0x%02x B3=0x%02x ", packet[0], packet[1], packet[2]);

	printf("LB=%d MB=%d RB=%d XOV=%d YOV=%d ", ((packet[0] & MOUSE_BTT_L)!= 0), ((packet[0] & MOUSE_BTT_M) != 0), ((packet[0] & MOUSE_BTT_R) != 0), ((packet[0] & MOUSE_X_OVFL) != 0), ((packet[0] & MOUSE_Y_OVFL) != 0));

	//Taking care of the two's complement
	//X
	if (packet[0] & MOUSE_X_SIGN) { //if true, number is negative (sign bit)
		printf("X=%l ", ((-1 << 8) | packet[1]));
	}
	else
		printf("X=%d ", (unsigned int)packet[1]);

	//Y
	if (packet[0] & MOUSE_Y_SIGN) { //if true, number is negative (sign bit)
			printf("Y=%l \n", ((-1 << 8) | packet[2]));
		}
		else
			printf("Y=%d \n", (unsigned int)packet[2]);

	return;
}

int clean_out_buf(){
	unsigned long stat;

	if(sys_inb(STAT_REG, &stat)!=OK) return -1;

	while(stat&OBF){
		if(sys_inb(OUT_BUF,&stat)!=OK) return -1;
		if(sys_inb(STAT_REG,&stat)!=OK) return -1;
	}

	return 0;
}

int poll_register(){
	unsigned long stat;
	int nTries = NUMBER_OF_TRIES, n=0;

	while (nTries != 0) { //not doing while(1) in case of it not having something to read
		sys_inb(STAT_REG, &stat); // reading the status register (stored in stat)
		if (stat & STAT_AUX) {
			mouse_packet_handler(&n);
			mouse_packet_handler(&n);
			mouse_packet_handler(&n);
			mouse_packet_printer();
			return 0;
		}
		nTries--;
		tickdelay(micros_to_ticks(WAIT_KBC));
	}
	return -1;
}

void mouse_stt_handler(int length)
{
	struct sttmachines machine;
	state_t currentstate = INITIAL;

//initializing all members of machines
	machine.length = length;
	machine.x_move = 0;
	machine.y_move = 0;
	machine.slope = NEGSLOPE;

	currentstate = gesture_handling();

	/*to complete*/
	/*what this function should do is take care of the movement we
	 * execute on the mouse, and take care of the slope mechanism,
	 * in case x and y have the same sign (neg or pos), and at the same time, update the current state the stt machine is*/

	return;
}

states gesture_handling(state_t currstate, sttmachines * machine) {
	if(currstate == INITIAL)
	{
		if(machine->rightbutton_type == RBDOWN)
			currstate = DRAWING; //USER IS READY TO DRAW
	}
	else
		if(currstate == DRAW)
		{
			if(machine->rightbutton_type == RBUP)
			{
				machine->x_move = 0;
				machine->y_move = 0;
				currstate = INITIAL;
			}
			else if(machine->rightbutton_type == RBDOWN)
			{
				if(machine->length > 0)
				{
					if((machine->slope_states == POSSLOPE) && (machine->x_move > machine->length)
							currstate = COMPLETION;
				}
				else{
					if((machine->slope_states == POSSLOPE) && (machine->x_move < machine->length)
							currstate = COMPLETION;
				}
			}
		}
	return currstate;
}





















