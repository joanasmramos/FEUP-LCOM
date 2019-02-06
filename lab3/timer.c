#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/driver.h>
#include "timer.h"
#include "i8254.h"

//counter required for the interruptions
unsigned int counterint = 0;

int tmr_hookID = 2;

int timer_set_frequency(unsigned char timer, unsigned long freq)
{
	unsigned char st;
	unsigned long ctrlword;

	// before we can do anything we have to read the timer's configuration
	timer_get_conf(timer, &st);
	st= 0x0F & st; // makes sure the last 4 lsbits are preserved

	unsigned long div = TIMER_FREQ/freq; // freq = clock/div => div = clock/freq

	switch (timer) {

	case 0: {
		// programming the timer
		ctrlword = (TIMER_SEL0 | TIMER_LSB_MSB | st); // ctrlword=0011xxxx -> selects timer 0,
													  // how the initial value will be loaded (LSB -> MSB)
													  // and maintains the original 4 lsbits
		if (sys_outb(TIMER_CTRL, ctrlword) != OK){
			printf("Failed to program the timer. \n\n");
			return 1;
		}
		else {
			if (sys_outb(TIMER_0, (unsigned char) div)== OK) // loading LSB
				if (sys_outb(TIMER_0, (unsigned char) (div >> 8))==OK) // loading MSB
					return 0;
				else {printf("Failed to load the initial value to the timer. \n\n"); return 1;}
			else {printf("Failed to load the initial value to the timer. \n\n"); return 1;}
		}
	}

	case 1: {
		// programming the timer
		ctrlword = (TIMER_SEL1 | TIMER_LSB_MSB | st); // ctrlword=0111xxxx -> selects timer 1,
													  // how the initial value will be loaded (LSB -> MSB)
													  // and maintains the original 4 lsbits
		if (sys_outb(TIMER_CTRL, ctrlword) != OK){
			printf("Failed to program the timer. \n\n");
			return 1;
		}
		else {
			if (sys_outb(TIMER_1, (unsigned char) div)== OK) // loading LSB
				if (sys_outb(TIMER_1, (unsigned char) (div >> 8))==OK) // loading MSB
					return 0;
				else {printf("Failed to load the initial value to the timer. \n\n"); return 1;}
			else {printf("Failed to load the initial value to the timer. \n\n"); return 1;}
		}
	}

	case 2: {
		// programming the timer
		ctrlword = (TIMER_SEL2 | TIMER_LSB_MSB | st); // ctrlword=1011xxxx -> selects timer 1,
													  // how the initial value will be loaded (LSB -> MSB)
													  // and maintains the original 4 lsbits
		if (sys_outb(TIMER_CTRL, ctrlword) != OK){
			printf("Failed to program the timer. \n\n");
			return 1;
		}
		else {
			if (sys_outb(TIMER_2, (unsigned char) div)== OK) // loading LSB
				if (sys_outb(TIMER_2, (unsigned char) (div >> 8))==OK) // loading MSB
					return 0;
				else {printf("Failed to load the initial value to the timer. \n\n"); return 1;}
			else {printf("Failed to load the initial value to the timer. \n\n"); return 1;}
			return 0;
		}
	}

	default: {
		printf("Only 0, 1 and 2 are valid timers. \n\n");
		return 1;
	}
	}
}

int timer_subscribe_int(void)
{
	int temphookID = tmr_hookID;
	//because we're working with global variables and we don't
	//want it to change
	int stat = sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &tmr_hookID);

	if (OK != stat)
	{
		printf("ERROR executing sys_ieqsetpolicy()");
		return -1;
	}

	stat = sys_irqenable(&tmr_hookID);
	if(OK != stat)
	{
		printf("ERROR executing sys_irqenable()");
		return -1;
	}


	return BIT(temphookID); //SUCCESS
}

int timer_unsubscribe_int()
{
	int stat = sys_irqdisable(&tmr_hookID);

	if(OK != stat)
	{
		printf("ERROR executing sys_irqdisable()");
		return -1;
	}

	stat = sys_irqrmpolicy(&tmr_hookID);
	if (OK != stat)
		{
			printf("ERROR executing sys_ieqsetpolicy()");
			return -1;
		}

	return 0; //SUCCESS
}

void timer_int_handler()
{
counterint++;
}

// Lab2 Part 1 - 1st function to implement
int timer_get_conf(unsigned char timer, unsigned char *st)
{
	// creating a var with the readback command
	unsigned long temprback=(TIMER_RB_CMD|TIMER_RB_COUNT_|TIMER_RB_SEL(timer));

	if ((sys_outb(TIMER_CTRL, temprback)!=OK)) // sys_outb returns 0 if there were no errors
	{
		printf("System failed writing to control register.\n\n");
		return 1;
	}
	else
	{
		switch (timer)
		{
		case 0: {
			if (sys_inb(TIMER_0, &temprback) != OK) // sys_inb also returns 0 if there were no errors
			{
				printf("Access to timer 0 configurations failed.\n\n");
				return 1;
			}
			break;
		}

		case 1: {
			if (sys_inb(TIMER_1, &temprback) != OK){
				printf("Access to timer 1 configurations failed.\n\n");
				return 1;
			}
			break;
		}

		case 2: {
			if (sys_inb(TIMER_2, &temprback) != OK){
				printf("Access to timer 2 configurations failed.\n\n");
				return 1;
			}
			break;
		}

		default: {
			printf("Timer is not valid.\n\n");
			return 1;
		}
		}

		*st = (unsigned char) temprback; //Shortening it to char because it's 8 bits
		return 0;
	}
}

//Lab2 Part 1 - 2nd function to implement
int timer_display_conf(unsigned char conf)
{
	//NOTES:
	// creating a var and use it for each of the options to display, regarding the timer.
	unsigned char tempconf = conf;

	//DISPLAYING CONFIGURATIONS ACCORDING TO THE CONTROL WORD AND ITS FUNCTIONS
	//Status of Timer
	printf("Timer Status Byte: 0x%x \n\n", conf); //Printing, in hexadecimal, the status of the corresponding timer

	//OUTPUT (bit 7)
	switch(tempconf & OUTPUT1) // -XXXXXXX
	{
	case OUTPUT1:
		printf("Output pin is 1. \n\n");
		break;

	case OUTPUT0:
		printf("Output pin is 0. \n\n");
		break;

	default:
		return 1;
		break;
	}


	//NULL COUNT (bit 6)
	switch(tempconf & NULLCOUNT) // X-XXXXXXX
	{
	case NULLCOUNT:
		printf("Null count. \n\n");
		break;

	case AVAILABLE:
		printf("Count available for reading. \n\n");
		break;

	default:
			return 1;
			break;

	}

	//TIMER TYPE OF ACCESS (bits 5 and 4)
	printf("Timer Type of Access:\n");

	switch(tempconf & TIMER_LSB_MSB) // XX--XXXX
	{
	case TIMER_LSB:
		printf("LSB: loads the initial value of the counter with the LSB \n\n");
		break;

	case TIMER_MSB:
		printf("MSB: loads the initial value of the counter with the MSB \n\n");
		break;

	case TIMER_LSB_MSB:
		printf("LSB->MSB: loads the initial value of the counter with the LSB followed by the MSB \n\n"); 	//Initializes LSB first and MSB afterwards
		break;
	case CTRLATCH:
		printf("Counter latch command. \n\n");
		break;

	default:
			return 1;
			break;

	}
	
	//OPERATING MODE (bits 3, 2 and 1)
	printf("Timer Operating Mode:\n");
	switch(tempconf & TIMER_GET_MODE) // XXXX---X
	{
	case TIMER_INTER_TERM_COUNT:
		printf("Mode 0: Timer interrupt on terminal count. \n\n");
		break;

	case TIMER_HARDWARE_ONESHOT:
		printf("Mode 1: Hardware retriggerable one-shot. \n\n");
		break;

	case TIMER_RATE_GEN:
		printf("Mode 2: Rate generator (divide-by-N counter). \n\n");
		break;

	case TIMER_RATEGEN:
		printf("Mode 2: Rate generator (divide-by-N counter). \n\n");
		break;

	case TIMER_SQR_WAVE:
		printf("Mode 3: Square wave generator. \n\n");
		break;

	case TIMER_SQRWAVE:
		printf("Mode 3: Square wave generator. \n\n");
		break;

	case TIMER_SOFTSTRB:
		printf("Mode 4: Software triggered strobe. \n\n");
		break;

	case TIMER_HARDSTRB:
		printf("Mode 5: Hardware triggered strobe. \n\n");
		break;

	default:
		return 1;
		break;
	}

	//COUNTING MODE
	printf("Timer Counting Mode:\n");
	if ((tempconf & TIMER_BCD) == TIMER_BIN) // equivalent to ((tempconf & 0x01) == 0x00)
		printf("Binary. \n\n");
	else if ((tempconf & TIMER_BCD) == TIMER_BCD) // equivalent to ((tempconf & 0x01) == 0x01)
		printf("BCD. \n\n");
	else
	{
		return 1;
	}

	return 0;
}

//Lab2 Part 1 - 4th function to implement
int timer_test_time_base(unsigned long freq) {
	if (freq<=0) {printf("Not a valid frequency. \n\n"); return 1;}

	if (timer_set_frequency(0, freq) != 0) {
		printf("Error occurred, unable to set frequency. \n\n");
		return 1;
	}
	else {
		printf("Frequency set to %d. Give the command date after a few time to check the differences \n", freq);
		return 0;
	}
}

int timer_test_int(unsigned long time) {
	int ipc_status, r;
	message msg;
	int irq_set = BIT(tmr_hookID);

	timer_set_frequency(0, FREQ); // in order to make sure the timer is normalized

	if (timer_subscribe_int()==-1){
		return -1;
	}

	while( counterint/60 < time ) { // during the interval defined by the passed argument
		/* Get a request message. */
		if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & irq_set) { /* subscribed interrupt */
					if(counterint%60 ==0)
						{printf("One more second \n");}
				timer_int_handler();
				}
				break;
			default:
				break; /* no other notifications expected: do nothing */
			}
		} else { /* received a standard message, not a notification */
			/* no standard messages expected: do nothing */
		}
	}

	if(timer_unsubscribe_int()==-1){
		return -1;
	}

	return 0;
}

//Lab2 Part 1 - 3rd function to implement
int timer_test_config(unsigned char timer)
{
	unsigned char conf;


	if (((int)timer!=0) && ((int)timer!=1) && ((int)timer!=2))
	{
		printf("Not a valid timer, must be 0, 1 or 2. \n\n");
		return 1;
	}
	
	if (timer_get_conf(timer, &conf) != 0) // timer_get_conf returns 0 upon success
	{
		printf("Obtaining configuration of Timer (%u) - FAILED \n\n", timer);
		return 1;
	} else
		if (timer_display_conf(conf) != 0) // timer_display_conf returns 0 upon success
			{
				printf("DISPLAY OF Timer (%u) - FAILED\n\n", timer);
				return 1;
			}
	return 0;
}
