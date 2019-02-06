#include <minix/sysutil.h>
#include <minix/drivers.h>
#include <minix/driver.h>
#include "i8042.h"
#include "i8254.h"
#include "timer.h"
#include "mouse.h"
#include "test4.h"


int mouse_test_packet(unsigned short cnt){
	int r, ipc_status, irq_set = mouse_subscribe_int(),ctr=0, n=0;
	message msg;


	if(irq_set==-1)
	{
		printf("MOUSE ERROR:: failed mouse_subscribe_int");
		return -1;
	}
	// before issuing a command to the mouse we have to issue
	// command 0xD4 to the KBC

	if(mouse_writting(KBC_CMD_REG, MOUSE_WRITE_BTM) != 0) //The next thing that comes, goes to the mouse
	{return -1;}
	mouse_writting(MOUSE_WRITECMD, MOUSE_ENABLE); //ENABLING MOUSE

	if(mouse_writting(KBC_CMD_REG, MOUSE_WRITE_BTM) != 0)
	{return -1;}
	mouse_writting(MOUSE_WRITECMD, SET_STREAM_MODE); //ENABLING STREAM MODE

	if(mouse_writting(KBC_CMD_REG, MOUSE_WRITE_BTM) != 0)
	{return -1;}
	mouse_writting(MOUSE_WRITECMD, MOUSE_ENABLE_DATA);

	while(n<cnt)
	{
		// during the interval defined by the passed argument
		/* Get a request message. */
		if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 )
		{
			printf("driver_receive failed with: %d", r);
			continue;
		}

		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */

				if (msg.NOTIFY_ARG & irq_set) /* subscribed interrupt */
				{
					if(mouse_packet_handler(&ctr) == 0)
						if(ctr==0){
							mouse_packet_printer();
							n++;
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

	printf("Finished!\n");

	if(mouse_writting(KBC_CMD_REG, MOUSE_WRITE_BTM) != 0)
		{return -1;}
	mouse_writting(MOUSE_WRITECMD, MOUSE_DISABLE_DATA); //DISABLING STREAM MODE

	if(mouse_writting(KBC_CMD_REG, MOUSE_WRITE_BTM) != 0)
		{return -1;}
	mouse_writting(MOUSE_WRITECMD, MOUSE_DISABLE); //DISABLING MOUSE

	if(mouse_unsubscribe_int()==-1)
		return -1;

	if(clean_out_buf()==-1)
		return -1;
	return 0;
}	

int mouse_test_async(unsigned short idle_time){
	int r, ipc_status, ctr=0;

	message msg;

	int count=0;

	int irq_setmouse = mouse_subscribe_int();
	int irq_settimer = timer_subscribe_int();
	if(irq_setmouse==-1)
	{
		printf("MOUSE ERROR:: failed mouse_subscribe_int");
		return -1;
	}
	if(irq_settimer==-1)
	{
		printf("TIMER ERROR:: failed timer_subscribe_int");
		return -1;
	}

	if(mouse_writting(KBC_CMD_REG, MOUSE_WRITE_BTM) != 0) //The next thing that comes, goes to the mouse
	{return -1;}
	mouse_writting(MOUSE_WRITECMD, MOUSE_ENABLE); //ENABLING MOUSE

	if(mouse_writting(KBC_CMD_REG, MOUSE_WRITE_BTM) != 0)
	{return -1;}
	mouse_writting(MOUSE_WRITECMD, SET_STREAM_MODE); //ENABLING STREAM MODE

	if(mouse_writting(KBC_CMD_REG, MOUSE_WRITE_BTM) != 0)
	{return -1;}
	mouse_writting(MOUSE_WRITECMD, MOUSE_ENABLE_DATA);



	while(count < idle_time*60)
	{ // during the interval defined by the passed argument
		/* Get a request message. */
		if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 )
		{
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & irq_setmouse){ /* subscribed interrupt */
					if(mouse_packet_handler(&ctr) == 0){
						if(ctr==0){
							mouse_packet_printer();
						}
					}
				}
				if (msg.NOTIFY_ARG & irq_settimer) /* subscribed interrupt */
					count++;

				break;

			default:
				break; /* no other notifications expected: do nothing */
			}
		} else { /* received a standard message, not a notification */
			/* no standard messages expected: do nothing */
		}
	}

	printf("Finished!\n");

	if(mouse_writting(KBC_CMD_REG, MOUSE_WRITE_BTM) != 0)
		{return -1;}
	mouse_writting(MOUSE_WRITECMD, MOUSE_DISABLE_DATA); //DISABLING STREAM MODE

	if(mouse_writting(KBC_CMD_REG, MOUSE_WRITE_BTM) != 0)
		{return -1;}
	mouse_writting(MOUSE_WRITECMD, MOUSE_DISABLE); //DISABLING MOUSE

	if(mouse_unsubscribe_int()==-1)
		return -1;
	if(timer_unsubscribe_int()==-1)
		return -1;

	if(clean_out_buf()==-1)
		return -1;
	return 0;
}	

int mouse_test_remote(unsigned long period, unsigned short cnt){
	period=period*1000; // period is now in micro seconds
	int ctr=0;

//	if(mouse_writting(KBC_CMD_REG,MOUSE_WRITE_BTM)==-1){
//			printf("Error writing command to mouse... \n");
//			return -1;
//		}
//	if(mouse_writting(MOUSE_WRITECMD,MOUSE_ENABLE_DATA)==-1){
//		printf("Error writing command to mouse... \n");
//		return -1;
//	}
//	// disabling mouse interrupts
//
//	if(mouse_writting(KBC_CMD_REG,MOUSE_WRITE_BTM)==-1){
//		printf("Error writing command to mouse... \n");
//		return -1;
//	}
//	if(mouse_writting(MOUSE_WRITECMD,MOUSE_DISABLE_DATA)==-1){
//		printf("Error writing command to mouse... \n");
//		return -1;
//	}

	if(mouse_subscribe_int()==-1) return -1;

	// setting remote mode
	if(mouse_writting(KBC_CMD_REG,MOUSE_WRITE_BTM)==-1){
		printf("Error writing command to mouse... \n");
		return -1;
	}

	if(mouse_writting(MOUSE_WRITECMD, SET_REMOTE_MODE)==-1){
		printf("Error writing command to mouse... \n");
		return -1;
	}

	clean_out_buf();
	while(ctr<cnt){
		printf("1 \n");
		if(mouse_writting(KBC_CMD_REG,MOUSE_WRITE_BTM)==-1){
				printf("Error writing command to mouse... \n");
				return -1;
			}
		clean_out_buf();
		if(mouse_writting(MOUSE_WRITECMD,MOUSE_READ_DATA)==-1){ //sending data request
			printf("Error writing command to mouse... \n");
			return -1;
		}
		poll_register();
		ctr++;
		tickdelay(micros_to_ticks(period));
		printf("2 \n");
	}

	// setting stream mode
	if(mouse_writting(KBC_CMD_REG,MOUSE_WRITE_BTM)==-1){
		printf("Error writing command to mouse... \n");
		return -1;
	}
	if(mouse_writting(MOUSE_WRITECMD, SET_STREAM_MODE)==-1){
		printf("Error writing command to mouse... \n");
		return -1;
	}

//	// enabling data
//	if(mouse_writting(KBC_CMD_REG,MOUSE_WRITE_BTM)==-1){
//			printf("Error writing command to mouse... \n");
//			return -1;
//		}
//	if(mouse_writting(MOUSE_WRITECMD,MOUSE_ENABLE_DATA)==-1){
//		printf("Error writing command to mouse... \n");
//		return -1;
//	}

	printf("Finished! \n");

	if(mouse_unsubscribe_int()==-1)
		return -1;

	clean_out_buf();

	return 0;
}	

int mouse_test_gesture(short length){
	int r, ipc_status, irq_set = mouse_subscribe_int(),ctr=0;

	message msg;

	struct sttmachines machine;
		state_t currentstate = INITIAL;

	//initializing all members of machines
		machine.length = length;
		machine.x_move = 0;
		machine.y_move = 0;
		machine.slope = NEGSLOPE;

	int count=0;

	int irq_setmouse = mouse_subscribe_int();
	if(irq_setmouse==-1)
	{
		printf("MOUSE ERROR:: failed mouse_subscribe_int");
		return -1;
	}
	if(mouse_writting(KBC_CMD_REG, MOUSE_WRITE_BTM) != 0) //The next thing that comes, goes to the mouse
	{return -1;}
	mouse_writting(MOUSE_WRITECMD, MOUSE_ENABLE); //ENABLING MOUSE

	if(mouse_writting(KBC_CMD_REG, MOUSE_WRITE_BTM) != 0)
	{return -1;}
	mouse_writting(MOUSE_WRITECMD, SET_STREAM_MODE); //ENABLING STREAM MODE

	if(mouse_writting(KBC_CMD_REG, MOUSE_WRITE_BTM) != 0)
	{return -1;}
	mouse_writting(MOUSE_WRITECMD, MOUSE_ENABLE_DATA);



	while(currentstate != COMPLETION)
	{ // during the interval defined by the passed argument
		/* Get a request message. */
		if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 )
		{
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & irq_setmouse){ /* subscribed interrupt */
					if(mouse_packet_handler(&ctr) == 0){
						if(ctr==0){

							mouse_packet_printer();
							mouse_stt_handler();
						}
					}
				}
				if (msg.NOTIFY_ARG & irq_settimer) /* subscribed interrupt */
				break;

			default:
				break; /* no other notifications expected: do nothing */
			}
		} else { /* received a standard message, not a notification */
			/* no standard messages expected: do nothing */
		}
	}

	printf("Finished!\n");

	if(mouse_writting(KBC_CMD_REG, MOUSE_WRITE_BTM) != 0)
		{return -1;}
	mouse_writting(MOUSE_WRITECMD, MOUSE_DISABLE_DATA); //DISABLING STREAM MODE

	if(mouse_writting(KBC_CMD_REG, MOUSE_WRITE_BTM) != 0)
		{return -1;}
	mouse_writting(MOUSE_WRITECMD, MOUSE_DISABLE); //DISABLING MOUSE

	if(mouse_unsubscribe_int()==-1)
		return -1;

	if(clean_out_buf()==-1)
		return -1;
	return 0;
}	

















