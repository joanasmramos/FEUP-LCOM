#include "handler.h"

unsigned long IRQSET_KBD;
int IRQSET_TIMER;

int totalsubscribe()
{
	IRQSET_TIMER = timer_subscribe_int();
	subscribeMouse();
	IRQSET_KBD = kbd_subscribe_int();

	return 0;
}

int totalunsubscribe(){

	if(kbd_unsubscribe_int() == 0)
	{
		if(timer_unsubscribe_int() == 0)
		{

			unsubscribeMouse();
			return 0;
		}
	}

	return -1;
}

void freeMemory(Game * game, Bar * bar, Ball * ball, Explosion_Arrow * arrow){
	deleteGameBMPs(game);
	deleteBMP(bar->bmp);
	deleteBMP(ball->bmp);
	free(bar);
	free(ball);
	free(arrow);
	free(game);
}
