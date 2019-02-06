#include <minix/drivers.h>
#include <minix/driver.h>
#include "game.h"
#include "i8042.h"
#include "handler.h"
#include "video_gr.h"
#include "vbe.h"
#include "timer.h"
#include "mouse.h"
#include "KBD.h"
#include "graphics.h"

Game * load_game_bmp() {
	Game* CrackofDoom = (Game*) malloc(sizeof(Game));

	CrackofDoom->background = loadBitmap("/home/lcom/proj/res/backgroundJOGO.bmp");

	CrackofDoom->grids = loadBitmap("/home/lcom/proj/res/GRIDJOGO.bmp");

	CrackofDoom->spaceship = loadBitmap("/home/lcom/proj/res/BAR3.bmp");

	CrackofDoom->pinkenergyball = loadBitmap("/home/lcom/proj/res/PINKBALL1.bmp");

	CrackofDoom->drone = loadBitmap("/home/lcom/proj/res/DRONE.bmp");

	CrackofDoom->ornaments = loadBitmap("/home/lcom/proj/res/GAME_ORNAMENTS.bmp");

	CrackofDoom->menu = loadBitmap("/home/lcom/proj/res/imagemMENU.bmp");

	CrackofDoom->explosion_menu = loadBitmap("/home/lcom/proj/res/EXPLOSION.bmp");

	CrackofDoom->change_char_bg = loadBitmap("/home/lcom/proj/res/CHANGE_CHAR_BMP.bmp");

	CrackofDoom->gameover = loadBitmap("/home/lcom/proj/res/GAME_OVER.bmp");

	CrackofDoom->pick1 = loadBitmap("/home/lcom/proj/res/HOW_WILL_YOU_PLAY.bmp");

	CrackofDoom->victory = loadBitmap("/home/lcom/proj/res/GAME_WON.bmp");

	CrackofDoom->numbers[0] = loadBitmap("/home/lcom/proj/res/0.bmp");

	CrackofDoom->numbers[1] = loadBitmap("/home/lcom/proj/res/1.bmp");

	CrackofDoom->numbers[2] = loadBitmap("/home/lcom/proj/res/2.bmp");

	CrackofDoom->numbers[3] = loadBitmap("/home/lcom/proj/res/3.bmp");

	CrackofDoom->numbers[4] = loadBitmap("/home/lcom/proj/res/4.bmp");

	CrackofDoom->numbers[5] = loadBitmap("/home/lcom/proj/res/5.bmp");

	CrackofDoom->numbers[6] = loadBitmap("/home/lcom/proj/res/6.bmp");

	CrackofDoom->numbers[7] = loadBitmap("/home/lcom/proj/res/7.bmp");

	CrackofDoom->numbers[8] = loadBitmap("/home/lcom/proj/res/8.bmp");

	CrackofDoom->numbers[9] = loadBitmap("/home/lcom/proj/res/9.bmp");

	if (CrackofDoom->menu == NULL
			|| CrackofDoom->explosion_menu == NULL
			|| CrackofDoom->background == NULL
			|| CrackofDoom->grids == NULL
			|| CrackofDoom->spaceship == NULL
			|| CrackofDoom->pinkenergyball == NULL
			|| CrackofDoom->drone == NULL
			|| CrackofDoom->ornaments == NULL
			|| CrackofDoom->change_char_bg == NULL
			|| CrackofDoom->gameover == NULL
			|| CrackofDoom->pick1 == NULL
			|| CrackofDoom->victory == NULL)
		printf("Erro ao dar load dos bitmaps \n");

	int i;

	for(i=0;i<10;i++)
		if(CrackofDoom->numbers[i]==NULL)
			printf("Erro ao dar load dos bitmaps \n");

	return CrackofDoom;
}

void runGame(Game * CrackofDoom){
	message msg;
	int r, ipc_status;

	vg_init(VBE_MODE);

	totalsubscribe();

	// initialising
	CrackofDoom->gameState = MAINMENU; // initial state
	CrackofDoom->initAngle = generateAngle();
	CrackofDoom->score = 0;
	CrackofDoom->mouse = 0;
	CrackofDoom->lives = 3;

	Mouse * mouse = initMouse();

	// all game objects
	Explosion_Arrow * explosion_arrow = new_arrow(LOCATION_X, LOCATION_ONEY, INITOPT, CrackofDoom->explosion_menu);
	Bar * spaceship_play = new_spaceship(XLEFTBORDER + (int) ((XRIGHTBORDER - XLEFTBORDER) / 2), 600,CrackofDoom->spaceship);
	Ball * energy_ball = new_energyBall(XLEFTBORDER + (int) ((XRIGHTBORDER - XLEFTBORDER) / 2),
				600 - (int) CrackofDoom->spaceship->bitmapInfoHeader.height / 2 - (int) CrackofDoom->pinkenergyball->bitmapInfoHeader.height/2,
								CrackofDoom->pinkenergyball);
	fill_matrix();
	allocate_matrix_drones(CrackofDoom);

	// drawing main menu
	drawInitialMenuBMPs(CrackofDoom);
	drawArrowBMP(explosion_arrow);
	doubleBuffering();

	while(CrackofDoom->gameState != END){
		/* Get a request message. */
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) {
			/* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {

			case HARDWARE: /* hardware interrupt notification */

				if (msg.NOTIFY_ARG & IRQSET_TIMER) {
					timer_handler(CrackofDoom, spaceship_play, energy_ball, explosion_arrow);
				}

				if (msg.NOTIFY_ARG & IRQSET_KBD) {
					kbd_handler(CrackofDoom, spaceship_play, energy_ball, explosion_arrow, mouse);
				}

				if (msg.NOTIFY_ARG & IRQSET_MOUSE) {
					mouse_handler(mouse, CrackofDoom, spaceship_play, energy_ball);
				}

				break;

			default:
				break; /* no other notifications expected: do nothing */
			}

		}
		else { /* received a standard message, not a notification */
				/* no standard messages expected: do nothing */
		}
	}

	totalunsubscribe();

	freeMemory(CrackofDoom, spaceship_play, energy_ball, explosion_arrow);
}

void updateGameStates(Game * game, event_g gameEvent) {
	switch (game->gameState) {

	case MAINMENU:
		if (gameEvent == PLAY)
			game->gameState = CHOOSE;

		if(gameEvent == SELECT_CHAR)
			game->gameState = CHANGE_CHAR;

		if(gameEvent == ESC_PRESS)
			game->gameState = END;

		if(gameEvent == LEAVE)
			game->gameState = END;

		break;

	case CHOOSE:
		if(gameEvent == CHOSEN)
			game->gameState = PREGAME;

		if(gameEvent == ESC_PRESS)
			game->gameState = MAINMENU;

		break;

	case PREGAME:
		if (gameEvent == SPACE)
			game->gameState = GAME;
		if(gameEvent == ESC_PRESS)
			game->gameState=MAINMENU;
		break;

	case GAME:
		if (gameEvent == NOMORELIVES)
			game->gameState = GAMEOVER;
		if(gameEvent == LOSTLIFE)
			game->gameState = PREGAME;
		if(gameEvent == ESC_PRESS)
			game->gameState = MAINMENU;
		if(gameEvent == P_PRESS)
			game->gameState = PAUSED;
		if(gameEvent == CLEAR)
			game->gameState = VICTORY;
		break;

	case VICTORY:
		if(gameEvent == ESC_PRESS)
			game->gameState = MAINMENU;
		break;

	case PAUSED:
		if(gameEvent==P_PRESS)
			game->gameState = GAME;
		break;

	case GAMEOVER:
		if (gameEvent == ESC_PRESS)
			game->gameState = MAINMENU;
		break;

	case CHANGE_CHAR:
		if(gameEvent == ESC_PRESS)
			game->gameState=MAINMENU;
		break;

	case END:
		break;

	default:
		break;
	}
}

double generateAngle(){
	int angdegrees = rand() % 120 + 30;

	while(angdegrees>70 && angdegrees<110)
		angdegrees = rand() % 120 + 30;

	return ( PI * angdegrees / 180 );
}
