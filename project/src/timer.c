#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/driver.h>
#include "timer.h"
#include "i8254.h"
#include "video_gr.h"

int hookID = 2;

int timer_subscribe_int(void) {
	int temphookID = hookID;
	//because we're working with global variables and we don't
	//want it to change
	int stat = sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hookID);

	if (OK != stat) {
		printf("ERROR executing sys_ieqsetpolicy()");
		return -1;
	}

	stat = sys_irqenable(&hookID);
	if (OK != stat) {
		printf("ERROR executing sys_irqenable()");
		return -1;
	}

	return BIT(temphookID); //SUCCESS
}

int timer_unsubscribe_int() {
	int stat = sys_irqdisable(&hookID);

	if (OK != stat) {
		printf("ERROR executing sys_irqdisable()");
		return -1;
	}

	stat = sys_irqrmpolicy(&hookID);
	if (OK != stat) {
		printf("ERROR executing sys_ieqsetpolicy()");
		return -1;
	}

	return 0; //SUCCESS
}

void timer_handler(Game * game, Bar * bar, Ball * ball, Explosion_Arrow * arrow) {

	switch (game->gameState) {

	case MAINMENU:
		drawInitialMenuBMPs(game);
		drawArrowBMP(arrow);
		doubleBuffering();
		break;

	case CHANGE_CHAR:
		drawInitialChangeCharBMPs(game);
		drawArrowBMP(arrow);
		doubleBuffering();
		break;

	case CHOOSE:
		drawPickBMP(game);
		drawArrowBMP(arrow);
		doubleBuffering();
		break;

	case PREGAME:
		drawInitialGameBMPs(game); // background + grids
		drawBarBMP(bar);
		drawBallBMP(ball);
		draw_matrix_drones(game);
		drawScore(game);
		drawLives(game);
		doubleBuffering();
		break;

	case GAME:
		drawInitialGameBMPs(game); // background + grids
		drawBarBMP(bar);

		ballMovement(game, bar, ball);

		drawBallBMP(ball);
		draw_matrix_drones(game);
		drawScore(game);
		drawLives(game);
		doubleBuffering();
		break;

	case PAUSED:
		drawInitialGameBMPs(game); // background + grids
		drawBarBMP(bar);
		drawBallBMP(ball);
		draw_matrix_drones(game);
		drawScore(game);
		drawLives(game);
		doubleBuffering();
		break;

	case GAMEOVER:
		drawInitialGameBMPs(game); // background + grids
		drawScore(game);
		drawLives(game);
		drawBarBMP(bar);
		drawBallBMP(ball);
		draw_matrix_drones(game);
		drawGameOverBMP(game);
		drawScoreGO(game);
		doubleBuffering();
		break;

	case VICTORY:
		drawInitialGameBMPs(game);
		drawBarBMP(bar);
		drawBallBMP(ball);
		drawVictoryBMP(game);
		drawScoreVictory(game);
		doubleBuffering();
		break;

	default:
		break;
	}

}
