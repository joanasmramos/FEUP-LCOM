#include "graphics.h"
#include "bitmap.h"
#include "video_gr.h"

Drone * matrix[ROWS][COLS];
Bar * spaceships;
Ball * energy_balls;
Explosion_Arrow * arrows;

void drawInitialChangeCharBMPs(Game * CrackofDoom)
{
	drawBitmap(CrackofDoom->change_char_bg, 0, 0, ALIGN_LEFT);
}
void drawInitialMenuBMPs(Game * CrackofDoom) {
	drawBitmap(CrackofDoom->menu, 0, 0, ALIGN_LEFT);
}
void drawInitialGameBMPs(Game * CrackofDoom) {
	drawBitmap(CrackofDoom->background, 0, 0, ALIGN_LEFT);
	drawBitmap(CrackofDoom->grids, 0, 0, ALIGN_LEFT);
	drawBitmap(CrackofDoom->ornaments, 820, 35, ALIGN_LEFT);
}

void drawArrowBMP(Explosion_Arrow * arrow) {
	drawBitmap(arrow->bmp, arrow->x_pos, arrow->y_pos, ALIGN_LEFT);
}
void drawBarBMP(Bar * bar) {
	drawBitmap(bar->bmp, bar->x_pos, bar->y_pos, ALIGN_CENTER);
}

void drawBallBMP(Ball * ball) {
	drawBitmap(ball->bmp, ball->x_pos, ball->y_pos, ALIGN_CENTER);
}

void drawPickBMP(Game * game){
	drawBitmap(game->pick1, 0, 0, ALIGN_LEFT);
}

void drawGameOverBMP(Game * game){
	drawBitmap(game->gameover, 0, 768/2, ALIGN_LEFT);
}

void drawVictoryBMP(Game * game){
	drawBitmap(game->victory, 0, 768/2, ALIGN_LEFT);
}

void drawScoreGO(Game * game){
	int n0,n1,n2,n3, scorecopy = game->score;
	n0=scorecopy%10;
	scorecopy=scorecopy/10;
	n1=scorecopy%10;
	scorecopy=scorecopy/10;
	n2=scorecopy%10;
	n3=scorecopy/10;

	drawBitmap(game->numbers[n0], NUM_X0, 760/2 + NUM_Y, ALIGN_LEFT);
	drawBitmap(game->numbers[n1], NUM_X1, 760/2 + NUM_Y, ALIGN_LEFT);
	drawBitmap(game->numbers[n2], NUM_X2, 760/2 + NUM_Y, ALIGN_LEFT);
	drawBitmap(game->numbers[n3], NUM_X3, 760/2 + NUM_Y, ALIGN_LEFT);
}

void drawScoreVictory(Game * game){
	int n0,n1,n2,n3, scorecopy = game->score;
	n0=scorecopy%10;
	scorecopy=scorecopy/10;
	n1=scorecopy%10;
	scorecopy=scorecopy/10;
	n2=scorecopy%10;
	n3=scorecopy/10;

	drawBitmap(game->numbers[n0], NUM0_X, 760/2 + NUMB_Y, ALIGN_LEFT);
	drawBitmap(game->numbers[n1], NUM1_X, 760/2 + NUMB_Y, ALIGN_LEFT);
	drawBitmap(game->numbers[n2], NUM2_X, 760/2 + NUMB_Y, ALIGN_LEFT);
	drawBitmap(game->numbers[n3], NUM3_X, 760/2 + NUMB_Y, ALIGN_LEFT);
}

void drawScore(Game * game){
	int n0,n1,n2,n3, scorecopy = game->score;

	n0=scorecopy%10;
	scorecopy=scorecopy/10;
	n1=scorecopy%10;
	scorecopy=scorecopy/10;
	n2=scorecopy%10;
	n3=scorecopy/10;

	drawBitmap(game->numbers[n0], NUMBERS_X0, NUMBERS_Y, ALIGN_LEFT);
	drawBitmap(game->numbers[n1], NUMBERS_X1, NUMBERS_Y, ALIGN_LEFT);
	drawBitmap(game->numbers[n2], NUMBERS_X2, NUMBERS_Y, ALIGN_LEFT);
	drawBitmap(game->numbers[n3], NUMBERS_X3, NUMBERS_Y, ALIGN_LEFT);
}

void drawLives(Game * game){
	drawBitmap(game->numbers[game->lives], NUMBERS_X2, LIVES_Y, ALIGN_LEFT);
}

void deleteBMP(Bitmap * bmp){
	deleteBitmap(bmp);
}

void deleteGameBMPs(Game * CrackofDoom) {
	deleteBitmap(CrackofDoom->background);
	deleteBitmap(CrackofDoom->grids);
	deleteBitmap(CrackofDoom->drone);
	deleteBitmap(CrackofDoom->ornaments);
	deleteBitmap(CrackofDoom->menu);
	deleteBitmap(CrackofDoom->explosion_menu);
	deleteBitmap(CrackofDoom->change_char_bg);
	deleteBitmap(CrackofDoom->gameover);
	deleteBitmap(CrackofDoom->pick1);
	deleteBitmap(CrackofDoom->victory);

	int i;
	for(i=0; i<10; i++)
		deleteBitmap(CrackofDoom->numbers[i]);
}

Explosion_Arrow* new_arrow(unsigned short x, unsigned short y, unsigned int opt, Bitmap * explosion_arrow) {
	// allocating memory
	Explosion_Arrow * arrow = (Explosion_Arrow *) malloc(
			sizeof(Explosion_Arrow));

	if (arrow == NULL)
		return NULL;

	// member assignments
	arrow->x_pos = x;

	arrow->y_pos = y;

	arrow->option = opt;

	arrow->bmp = explosion_arrow;

	return arrow;
}

Bar* new_spaceship(unsigned short x, unsigned short y, Bitmap * ship) {
	// allocating memory
	Bar * spaceship = (Bar *) malloc(sizeof(Bar));

	if (spaceship == NULL)
		return NULL;

	// member assignments
	spaceship->x_pos = x;

	spaceship->y_pos = y;

	spaceship->height = ship->bitmapInfoHeader.height;

	spaceship->width = ship->bitmapInfoHeader.width;

	spaceship->bmp = ship;

	spaceship->vx = 8;

	return spaceship;
}

Ball* new_energyBall(unsigned short x, unsigned short y, Bitmap * gameBall) {
	// allocating memory
	Ball * energyball = (Ball *) malloc(sizeof(Ball));

	if (energyball == NULL)
		return NULL;

	// member assignments
	energyball->x_pos = x;

	energyball->y_pos = y;

	energyball->height = gameBall->bitmapInfoHeader.height;

	energyball->width = gameBall->bitmapInfoHeader.width;

	energyball->bmp = gameBall;

	energyball->v = INITV;

	energyball->acel = 2;

	return energyball;
}

Drone* new_drone(unsigned short x, unsigned short y, Bitmap * drone) {
	// allocating memory
	Drone * drones = (Drone *) malloc(sizeof(Drone));

	// member assignments
	drones->x_pos = x;

	drones->y_pos = y;

	drones->height = drone->bitmapInfoHeader.height;

	drones->width = drone->bitmapInfoHeader.width;

	drones->bmp = drone;

	return drones;
}

void fill_matrix() {
	unsigned int i, j;
	i = 0;
	j = 0;

	for (i = 0; i < ROWS; i++)
		for (j = 0; j < COLS; j++)
			matrix[i][j] = NULL;

}

void allocate_matrix_drones(Game * CrackofDoom) {
	int x, y;
	x = XLEFTBORDER;
	y = YTOPBORDER; // blocks start at top left
	unsigned int i, j;
	i = 0;
	j = 0;

	for (i = 0; i < ROWS; i++) {
		for (j = 0; j < COLS; j++) {
			matrix[i][j] = new_drone(x, y, CrackofDoom->drone);
			x += matrix[i][j]->width + SPACE_BTW_DRONElvl1; // within columns of the same row y remains the same so we only update x
			CrackofDoom->blocksLeft++;
		}
		y += CrackofDoom->drone->bitmapInfoHeader.height + 20;
		x = XLEFTBORDER;
	}
}

void draw_matrix_drones(Game * CrackofDoom) {
	unsigned int i, j;
	i = 0;
	j = 0;

	for (i = 0; i < ROWS; i++) {
		for (j = 0; j < COLS; j++) {
			if (matrix[i][j] != NULL)
				drawBitmap(CrackofDoom->drone, matrix[i][j]->x_pos,
						matrix[i][j]->y_pos, ALIGN_LEFT);
		}
	}
}

void ballMovement(Game * game, Bar * bar, Ball * ball) {

	int i, j;

	ball->x0_pos=ball->x_pos;
	ball->y0_pos=ball->y_pos;

	ball->x_pos = (int) (ball->x_pos + ball->deltax);
	ball->y_pos = (int) (ball->y_pos + ball->deltay);

	if (ball->x_pos + ball->width / 2 > XRIGHTBORDER
			|| ball->x_pos - ball->width / 2 < XLEFTBORDER)
		ball->deltax = -ball->deltax;

	if (ball->y_pos < YTOPBORDER)
		ball->deltay = -ball->deltay;

	for(i=0;i<5; i++) {
		for(j=0;j<6;j++) {

			if(matrix[i][j]!= NULL) {

				// ball hit drone from below
				if( (ball->y0_pos >= (matrix[i][j]->y_pos + matrix[i][j]->height) ) &&
						(ball->y_pos <= (matrix[i][j]->y_pos + matrix[i][j]->height) ) &&
						( (ball->x_pos + ball->width / 2) >= matrix[i][j]->x_pos) &&
						( (ball->x_pos - ball->width / 2) <= (matrix[i][j]-> x_pos + matrix[i][j]->width) ) ) { // collision
					ball->deltay= - ball->deltay;
					free(matrix[i][j]);
					matrix[i][j] = NULL;
					game->blocksLeft--;
					if(game->blocksLeft==0)
						updateGameStates(game, CLEAR);
					game->score+=150;
					break;
				}

				// ball hit drone from above
				if( ( (ball->y0_pos+ball->height) <= matrix[i][j]->y_pos) &&
						( (ball->y_pos+ball->height) >= matrix[i][j]->y_pos) &&
						( (ball->x_pos + ball->width / 2) >= matrix[i][j]->x_pos) &&
						( (ball->x_pos - ball->width / 2) <= (matrix[i][j]-> x_pos + matrix[i][j]->width) ) ){ // collision
					ball->deltay = - ball->deltay;
					free(matrix[i][j]);
					matrix[i][j] = NULL;
					game->blocksLeft--;
					if(game->blocksLeft==0)
						updateGameStates(game, CLEAR);
					game->score+=150;
					break;
				}

				// ball hit drone from the left
				if( ( (ball->x0_pos+ball->width/2) <= matrix[i][j]->x_pos) &&
						( (ball->x_pos+ball->width/2) >= matrix[i][j]->x_pos ) &&
						( (ball->y_pos+ball->height) >= matrix[i][j]->y_pos) &&
						( ball->y_pos <= (matrix[i][j]->y_pos+matrix[i][j]->height) ) ){ // collision
					ball->deltax = - ball->deltax;
					free(matrix[i][j]);
					matrix[i][j] = NULL;
					game->blocksLeft--;
					if(game->blocksLeft==0)
						updateGameStates(game, CLEAR);
					game->score+=150;
					break;
				}

				// ball hit drone from the right
				if( ( (ball->x0_pos - ball->width/2) >= (matrix[i][j]->x_pos + matrix[i][j]->width) ) &&
						( (ball->x_pos - ball->width/2) <= (matrix[i][j]->x_pos + matrix[i][j]->width) ) &&
						( (ball->y_pos+ball->height) >= matrix[i][j]->y_pos) &&
						( ball->y_pos <= (matrix[i][j]->y_pos+matrix[i][j]->height) ) ) { // collision
					ball->deltax = - ball->deltax;
					free(matrix[i][j]);
					matrix[i][j] = NULL;
					game->blocksLeft--;
					if(game->blocksLeft==0)
						updateGameStates(game, CLEAR);
					game->score+=150;
					break;
				}

			}
		}
	}


	if (ball->y_pos > getVerResolution()){
		game->lives--;
		if(game->lives == 0){
			updateGameStates(game, NOMORELIVES);
			return;
		}
		bar->x_pos=XLEFTBORDER + (int) ((XRIGHTBORDER - XLEFTBORDER) / 2);
		bar->y_pos=600;
		ball->x_pos=XLEFTBORDER + (int) ((XRIGHTBORDER - XLEFTBORDER) / 2);
		ball->y_pos=600 - ball->bmp->bitmapInfoHeader.height;
		game->initAngle=generateAngle();
		updateGameStates(game, LOSTLIFE);
	}


	if (( (ball->x_pos + ball->width / 2) >= (bar->x_pos - bar->width / 2))
			&& ( (ball->x_pos - ball->width / 2) <= (bar->x_pos + bar->width / 2) )
			&& ( (ball->y0_pos+ball->height) <= bar->y_pos)
			&& ( (ball->y_pos + ball->height) >= bar->y_pos) ){
		ball->deltay = -ball->deltay;

	}
}



