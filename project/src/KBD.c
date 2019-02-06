#include "KBD.h"
#include <math.h>
#include "bitmap.h"
int kbd_hookID = 3;

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

unsigned long reading(Mouse * mouse) {
	unsigned long stat;
	unsigned long data;

	while (1) {
		sys_inb(STAT_REG, &stat);
		if (stat & OBF) {
			if( (stat & STAT_AUX) == 0) {
				sys_inb(OUT_BUF, &data);
				if ((stat & (PAR_ERR | TO_ERR)) == 0) // parity error/timeout error
					return data;
				else {
					return -1;
				}
			} else
				mouse_packet_handler(mouse);

		}
		tickdelay(micros_to_ticks(WAIT_KBC));
	}
}

void kbd_handler(Game * game, Bar * bar, Ball * ball, Explosion_Arrow * arrow, Mouse * mouse) {
	unsigned long scancode = reading(mouse);

	switch (game->gameState) {

	case MAINMENU:
		if (scancode == W_MAKE) {
			if (arrow->option == INITOPT && arrow->y_pos < LOCATION_ONEY) {
				break; // does nothing, can't go up anymore
			} else {
				if (arrow->option == 1) {
					arrow->y_pos = LOCATION_ONEY;
					arrow->option--;
				} else if (arrow->option == 2) {
					arrow->y_pos = LOCATION_TWOY;
					arrow->option--;
				}
			}
		}

		if (scancode == S_MAKE) {
			if (arrow->option == 2 && arrow->y_pos > LOCATION_THREEY) {
				break; // does nothing, can't go down anymore
			} else {
				if (arrow->option == INITOPT) {
					arrow->y_pos = LOCATION_TWOY;
					arrow->option++;
				} else if (arrow->option == 1) {
					arrow->y_pos = LOCATION_THREEY;
					arrow->option++;
				}
			}
		}

		if (scancode == SPACE_MAKE || scancode == ENTER_MAKE) {

			if (arrow->option == INITOPT) {
				updateGameStates(game, PLAY);
				arrow->option = INITOPT;
				arrow->x_pos = KBD_X;
				arrow->y_pos = PICK_Y;
			}

			if (arrow->option == 1) {
				updateGameStates(game, SELECT_CHAR);
				arrow->option = INITOPT;
				arrow->x_pos = ARRCOL1_X;
				arrow->y_pos = LINE1_Y;

			}

			if (arrow->option == 2)
				updateGameStates(game, LEAVE);
		}

		if (scancode == ESC)
			updateGameStates(game, ESC_PRESS);
		break;

	case CHANGE_CHAR:

		if (scancode == A_MAKE) {

			if (arrow->option == 1)
				arrow->x_pos = ARRCOL1_X;

			else if (arrow->option == 2)
				arrow->x_pos = ARRCOL2_X;

			else if (arrow->option == 3) {
				arrow->x_pos = ARRCOL3_X;
				arrow->y_pos = LINE1_Y;;
			}

			else if (arrow->option == 4)
				arrow->x_pos = ARRCOL1_X;

			else if (arrow->option == 5)
				arrow->x_pos = ARRCOL2_X;

			arrow->option--;
		}

		if (scancode == D_MAKE) {

			if (arrow->option == INITOPT)
				arrow->x_pos = ARRCOL2_X;

			else if (arrow->option == 1)
				arrow->x_pos = ARRCOL3_X;

			else if (arrow->option == 2){
				arrow->x_pos = ARRCOL1_X;
				arrow->y_pos = LINE2_Y;
			}

			else if (arrow->option == 3)
				arrow->x_pos = ARRCOL2_X;

			else if (arrow->option == 4)
				arrow->x_pos = ARRCOL3_X;

			arrow->option++;
		}

		if(scancode == W_MAKE) {

			if(arrow->y_pos == LINE2_Y){
				arrow->option-=3;
				arrow->y_pos = LINE1_Y;
			}

		}

		if(scancode == S_MAKE){

			if(arrow->y_pos == LINE1_Y){
				arrow->option+=3;
				arrow->y_pos = LINE2_Y;
			}

		}

		if(scancode == SPACE_MAKE || scancode == ENTER_MAKE){

			if (arrow->option == INITOPT) {
				deleteBMP(bar->bmp);
				game->spaceship = loadBitmap("/home/lcom/proj/res/BAR1.bmp");
				bar->bmp=game->spaceship;
			}

			else if (arrow->option == 1) {
				deleteBMP(bar->bmp);
				game->spaceship = loadBitmap("/home/lcom/proj/res/BAR3.bmp");
				bar->bmp=game->spaceship;
			}

			else if (arrow->option == 2) {
				deleteBMP(bar->bmp);
				game->spaceship = loadBitmap("/home/lcom/proj/res/BAR4.bmp");
				bar->bmp=game->spaceship;
			}
			else if (arrow->option == 3) {
				deleteBMP(ball->bmp);
				game->pinkenergyball = loadBitmap("/home/lcom/proj/res/ORANGEBALL2.bmp");
				ball->bmp=game->pinkenergyball;
			}

			else if (arrow->option == 4) {
				deleteBMP(ball->bmp);
				game->pinkenergyball = loadBitmap("/home/lcom/proj/res/BLUEBALL2.bmp");
				ball->bmp=game->pinkenergyball;
			}

			else if (arrow->option == 5) {
				deleteBMP(ball->bmp);
				game->pinkenergyball = loadBitmap("/home/lcom/proj/res/PINKBALL1.bmp");
				ball->bmp=game->pinkenergyball;
			}

			arrow->x_pos=LOCATION_X;
			arrow->y_pos=LOCATION_ONEY;
			arrow->option=INITOPT;

			updateGameStates(game, ESC_PRESS);
		}

		if (scancode == ESC){
			updateGameStates(game, ESC_PRESS);
			arrow->x_pos=LOCATION_X;
			arrow->y_pos=LOCATION_ONEY;
			arrow->option=INITOPT;
		}
		break;

	case CHOOSE:

		if (scancode == A_MAKE){
			if (arrow->option == 1){
				arrow->x_pos=KBD_X;
				arrow->option--;
			}
		}

		if (scancode == D_MAKE){
			if(arrow->option == INITOPT){
				arrow->x_pos = MOUSE_X;
				arrow->option++;
			}
		}

		if (scancode == SPACE_MAKE || scancode == ENTER_MAKE){
			if(arrow->option == 1) {
				game->mouse = 1;
			}
			arrow->x_pos=LOCATION_X;
			arrow->y_pos=LOCATION_ONEY;
			arrow->option=INITOPT;
			updateGameStates(game, CHOSEN);

		}

		if (scancode == ESC){
			updateGameStates(game, ESC_PRESS);
			arrow->x_pos=LOCATION_X;
			arrow->y_pos=LOCATION_ONEY;
			arrow->option=INITOPT;
		}

		break;

	case PREGAME:
		if (scancode == A_MAKE)
			if ((bar->x_pos - (int) (bar->width / 2)) > XLEFTBORDER) { // verificar x à esquerda
				bar->x_pos -= bar->vx;
				ball->x_pos = bar->x_pos;
			}

		if (scancode == D_MAKE)
			if ((bar->x_pos + (int) (bar->width / 2)) < XRIGHTBORDER) { // verificar x à direita
				bar->x_pos += bar->vx;
				ball->x_pos = bar->x_pos;
			}

		if (scancode == SPACE_MAKE) {
			updateGameStates(game, SPACE); // game started
			ball->deltax = cos(game->initAngle) * ball->v;
			ball->deltay = -sin(game->initAngle) * ball->v;
		}

		if (scancode == ESC){
			updateGameStates(game, ESC_PRESS);
			game->mouse = 0;
		}


		break;

	case GAME:

		if(game->mouse == 0){
			if (scancode == A_MAKE)
				if ((bar->x_pos - (int) (bar->width / 2)) > XLEFTBORDER) // verificar x à esquerda
					bar->x_pos -= bar->vx;

			if (scancode == D_MAKE)
				if ((bar->x_pos + (int) (bar->width / 2)) < XRIGHTBORDER) // verificar x à direita
					bar->x_pos += bar->vx;
		}

		if(scancode == P_MAKE)
			updateGameStates(game, P_PRESS);

		if (scancode == ESC){
			updateGameStates(game, ESC_PRESS);
			bar->x_pos=XLEFTBORDER + (int) ((XRIGHTBORDER - XLEFTBORDER) / 2);
			bar->y_pos=600;
			ball->x_pos=XLEFTBORDER + (int) ((XRIGHTBORDER - XLEFTBORDER) / 2);
			ball->y_pos=600 - ball->bmp->bitmapInfoHeader.height;
			fill_matrix();
			allocate_matrix_drones(game);
			game->initAngle=generateAngle();
			game->score=0;
			game->lives=3;
			game->blocksLeft = 30;
			game->mouse=0;
		}

		break;

	case PAUSED:
		if(scancode == P_MAKE)
			updateGameStates(game, P_PRESS);
		break;

	case GAMEOVER:
		if (scancode == ESC){
			updateGameStates(game, ESC_PRESS);
			bar->x_pos=XLEFTBORDER + (int) ((XRIGHTBORDER - XLEFTBORDER) / 2);
			bar->y_pos=600;
			ball->x_pos=XLEFTBORDER + (int) ((XRIGHTBORDER - XLEFTBORDER) / 2);
			ball->y_pos=600 - ball->bmp->bitmapInfoHeader.height;
			fill_matrix();
			allocate_matrix_drones(game);
			game->initAngle=generateAngle();
			game->score=0;
			game->lives=3;
			game->blocksLeft = 30;
			game->mouse=0;
		}
		break;

	case VICTORY:
		if (scancode == ESC){
			updateGameStates(game, ESC_PRESS);
			bar->x_pos=XLEFTBORDER + (int) ((XRIGHTBORDER - XLEFTBORDER) / 2);
			bar->y_pos=600;
			ball->x_pos=XLEFTBORDER + (int) ((XRIGHTBORDER - XLEFTBORDER) / 2);
			ball->y_pos=600 - ball->bmp->bitmapInfoHeader.height;
			fill_matrix();
			allocate_matrix_drones(game);
			game->initAngle=generateAngle();
			game->score=0;
			game->lives=3;
			game->blocksLeft = 30;
			game->mouse=0;
		}
		break;

	default:
		break;
	}

}

