#include <minix/sysutil.h>
#include <minix/drivers.h>
#include "mouse.h"
#include "i8042.h"
#include "i8254.h"
#include <math.h>

int mouse_hookID = 4;
unsigned long IRQSET_MOUSE;

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

int subscribeMouse(){
	IRQSET_MOUSE = mouse_subscribe_int();

	clean_out_buf();

//	write_mouse_command(MOUSE_DISABLE_DATA);
//
//	if(mouse_writting(KBC_CMD_REG, MOUSE_WRITE_BTM) != 0)
//		{return -1;}
//
//	mouse_writting(MOUSE_WRITECMD, MOUSE_ENABLE);
//
//	write_mouse_command(SET_STREAM_MODE);
//
//	write_mouse_command(MOUSE_ENABLE_DATA);

	if(mouse_writting(KBC_CMD_REG, MOUSE_WRITE_BTM) != 0) //The next thing that comes, goes to the mouse
	{return -1;}
	mouse_writting(MOUSE_WRITECMD, MOUSE_ENABLE); //ENABLING MOUSE

	if(mouse_writting(KBC_CMD_REG, MOUSE_WRITE_BTM) != 0)
	{return -1;}
	mouse_writting(MOUSE_WRITECMD, SET_STREAM_MODE); //ENABLING STREAM MODE

	if(mouse_writting(KBC_CMD_REG, MOUSE_WRITE_BTM) != 0)
	{return -1;}
	mouse_writting(MOUSE_WRITECMD, MOUSE_ENABLE_DATA);

	return 0;
}

int unsubscribeMouse(){
	clean_out_buf();

//	write_mouse_command(MOUSE_DISABLE_DATA);
//
//	if(mouse_writting(KBC_CMD_REG, MOUSE_WRITE_BTM) != 0)
//		{return -1;}
//
//	mouse_writting(MOUSE_WRITECMD, MOUSE_DISABLE);
//
//	write_mouse_command(MOUSE_DISABLE_DATA);


	if(mouse_writting(KBC_CMD_REG, MOUSE_WRITE_BTM) != 0)
		{return -1;}
	mouse_writting(MOUSE_WRITECMD, MOUSE_DISABLE_DATA); //DISABLING STREAM MODE

	if(mouse_writting(KBC_CMD_REG, MOUSE_WRITE_BTM) != 0)
		{return -1;}
	mouse_writting(MOUSE_WRITECMD, MOUSE_DISABLE); //DISABLING MOUSE

	if(mouse_unsubscribe_int()==-1)
		return -1;

	clean_out_buf();

	return 0;
}

unsigned long mouse_reading() {
	unsigned long stat;
	unsigned long data;
	int nTries = NUMBER_OF_TRIES;

	while (nTries != 0) { //not doing while(1) in case of it not having something to read
		sys_inb(STAT_REG, &stat); // reading the status register (stored in stat)
		if (stat & OBF) {
			//printf("stat: %x \n", stat);
			//printf("stat e aux: %x \n", stat & STAT_AUX);
			if((stat & STAT_AUX) == 0x20) {

				sys_inb(OUT_BUF, &data);
				if ((stat & (PAR_ERR | TO_ERR)) == 0){ // parity error/timeout error
					return data;
				}
				else {
					//printf("failed here \n");
					 return -1;
				}
			}
			else KBC_discard();
		}
		nTries--;
		tickdelay(micros_to_ticks(WAIT_KBC));
	}

	//printf("failed down here \n");
	return -1;
}

int mouse_writting(unsigned char reg, unsigned char command) {
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

int mouse_packet_handler(Mouse * mouse) {
	unsigned long byte = mouse_reading();

	if(byte==ACK)
		mouse_reading();

	if (byte == -1) {
		//printf("mouse_packet_handler() failed \n");
		return -1;
	}

	if (mouse->counter == 2){
		mouse->packet[mouse->counter] = byte;
		mouse->counter=0;
		return 0;
	}

	if (mouse->counter == 1){
		mouse->packet[mouse->counter] = byte;
		mouse->counter=2;
		return 0;
	}

	if (mouse->counter == 0){
		if (byte & BIT(3)) {
			mouse->packet[mouse->counter] = byte;
			mouse->counter=1;
			return 0;
		}
	}

	return -1;
}

void KBC_discard(){
	unsigned long stat;
	unsigned long data;

	sys_inb(STAT_REG, &stat);
	if(stat & OBF)
		sys_inb(OUT_BUF, &data);
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

Mouse * initMouse(){
	Mouse * mouse = (Mouse*) malloc(sizeof(Mouse));

	mouse->deltax = 0;
	mouse->deltay = 0;
	mouse->counter = 0;
	mouse->packet[0]=0;
	mouse->packet[1]=0;
	mouse->packet[2]=0;

	return mouse;
}

void mouse_handler(Mouse * mouse, Game * game, Bar * bar, Ball * ball){
	mouse_packet_handler(mouse);
	setDeltas(mouse);

	switch(game->gameState){
	case PREGAME:

		if(game->mouse == 1){
			if(mouse->deltax != 0){
				if(mouse->xSign == 1){
					if ((bar->x_pos - (int) (bar->width / 2)) > XLEFTBORDER){ // verificar x à esquerda
									bar->x_pos -= 2;
									ball->x_pos = bar->x_pos;
					}
				}
				else if ((bar->x_pos + (int) (bar->width / 2)) < XRIGHTBORDER){ // verificar x à direita
					bar->x_pos += 2;
					ball->x_pos = bar->x_pos;
				}
			}
		}
		break;

	case GAME:
		if(game->mouse == 1){
			if(mouse->deltax != 0){
				if(mouse->xSign == 1){
					if ((bar->x_pos - (int) (bar->width / 2)) > XLEFTBORDER) // verificar x à esquerda
									bar->x_pos -= 2;
				}
				else if ((bar->x_pos + (int) (bar->width / 2)) < XRIGHTBORDER) // verificar x à direita
					bar->x_pos += 2;
			}
		}
		break;

	default:
		break;
	}
}

void setDeltas(Mouse * mouse){
	unsigned long deltax=mouse->packet[1], deltay=mouse->packet[2];

	if( (mouse->packet[0] & MOUSE_X_SIGN) == MOUSE_X_SIGN){
		mouse->xSign=1;
	}
	else mouse->xSign=0;


	if(mouse->packet[1]!=0 && mouse->packet[2]!=0) {
		deltax = ~ deltax;
		deltax++;
		mouse->deltax = deltax;

		deltay = ~ deltay;
		deltay++;
		mouse->deltay = deltay;
	}

}




















