#ifndef GAME_H
#define GAME_H

#include "bitmap.h"

/** @defgroup game game
 * @{
 *
 * State machines and function related to updating states, functions to run the game
*/

#define PI 3.14159265359

// states and events related to the game

typedef enum {MAINMENU, CHOOSE, PREGAME, GAME, GAMEOVER, VICTORY, CHANGE_CHAR, END, PAUSED} state_g;
typedef enum {SELECT_CHAR, ESC_PRESS, CHOSEN, PLAY, SPACE, NOMORELIVES, LOSTLIFE, LEAVE, P_PRESS, CLEAR} event_g;

typedef struct {

	double initAngle;

	int score;
	int lives;
	int blocksLeft;

	int mouse;

	state_g gameState;

	//BMP'S
	Bitmap * menu;
	Bitmap * explosion_menu;
	Bitmap * background;
	Bitmap * change_char_bg;
	Bitmap * spaceship;
	Bitmap * grids;
	Bitmap * ornaments;
	Bitmap * pinkenergyball;
	Bitmap * drone;
	Bitmap * gameover;
	Bitmap * pick1;
	Bitmap * victory;
	Bitmap * numbers[10];

} Game;

/**
 * @brief Allocates memory for a game struct, and loads all the Bitmaps that will be necessary to the game to that struct
 *
 * @return Returns a pointer to the struct created
 */
Game * load_game_bmp();

/**
 * @brief Function with the interruptions loop which initializes the struct pointed to by the parameter and all elements of the game like the ball, bar, arrow, etc
 *
 * @param CrackofDoom Pointer to Game struct
 */
void runGame(Game * CrackofDoom);

/**
 * @brief Updates the game states depending on the game's current state (member of the Game struct) and the ocurring event passed as a parameter
 *
 *@param game Pointer to Game struct
 *@param gameEvent Ocurring event
 */
void updateGameStates(Game * game, event_g gameEvent);

/**
 * @brief Randomly generates an angle between 30 to 150 degrees, excluding 70 to 110 degrees and converts it to radians
 */
double generateAngle();

#endif /* GAME_H */
