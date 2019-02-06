#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "bitmap.h"
#include "game.h"

/** @defgroup graphics graphics
 * @{
 *
 * Functions to deal with drawing, creation of elements of the game and motion of the ball
 */

//Explosion as Arrow in MENU
#define INITOPT 0
#define LOCATION_X 656
#define LOCATION_ONEY 558
#define LOCATION_TWOY 630
#define LOCATION_THREEY 690

//Explosion as Arrow in CHANGE_CHAR and BMP'S for spaceship and ball
#define LINE1_Y 564
#define LINE2_Y 658
#define ARRCOL1_X 402
#define ARRCOL2_X 584
#define ARRCOL3_X 750

// Explosion arrow in Pick menu
#define KBD_X 248
#define MOUSE_X 695
#define PICK_Y 500

// Score
#define NUMBERS_Y 355
#define NUMBERS_X0 935
#define NUMBERS_X1 915
#define NUMBERS_X2 895
#define NUMBERS_X3 875

// Score game over
#define NUM_Y 185
#define NUM_X0 560
#define NUM_X1 540
#define NUM_X2 520
#define NUM_X3 500

// Score victory

#define NUMB_Y 185
#define NUM0_X 555
#define NUM1_X 535
#define NUM2_X 515
#define NUM3_X 495

// Lives
#define LIVES_Y 95

// grid
#define XLEFTBORDER 21
#define XRIGHTBORDER 804
#define YTOPBORDER 23
#define YBOTTOMBORDER 747

#define VMAX 10 // velocidade máxima: 10 pixeis por frame assumindo frame rate de 60/s
#define INITV 5 // velocidade inicial

// Drones
#define ROWS 5
#define COLS 6
#define SPACE_BTW_DRONElvl1	53

typedef struct {
	int x_pos;
	int y_pos;
	int width;
	int height;
	int vx;

	Bitmap * bmp;
} Bar;

typedef struct {
	int x_pos;
	int y_pos;
	int x0_pos;
	int y0_pos;
	int deltax;
	int deltay;
	int width;
	int height;
	int v;
	int acel;
	double angle;

	Bitmap * bmp;
} Ball;

typedef struct {
	int x_pos;
	int y_pos;
	int width;
	int height;

	Bitmap * bmp;
} Drone;

typedef struct {
	int x_pos;
	int y_pos;

	unsigned int option;

	Bitmap * bmp;

} Explosion_Arrow;

extern Drone * matrix[ROWS][COLS];
extern Bar * spaceship;
extern Ball * energy_ball;
extern Explosion_Arrow * arrow;

/**
 * @brief Creates and initializes a drone (block)
 *
 * @param x X coordinate
 * @param y Y coordinate
 * @param drone Pointer to the bitmap of the drone
 *
 * @return Returns a pointer to the drone created
 */
Drone* new_drone(unsigned short x, unsigned short y, Bitmap * drone);

/**
 * @brief Creates and initializes an arrow (used to choose options in the menus)
 *
 * @param x X coordinate
 * @param y Y coordinate
 * @param opt Option selected by the arrow
 * @param explosion_arrow Pointer to the bitmap of the arrow
 *
 * @return Returns a pointer to the arrow created
 */
Explosion_Arrow* new_arrow(unsigned short x, unsigned short y, unsigned int opt, Bitmap * explosion_arrow);

/**
 * @brief Creates and initializes a bar
 *
 * @param x X coordinate
 * @param y Y coordinate
 * @param ship Pointer to the bitmap of the drone
 *
 * @return Returns a pointer to the bar created
 */
Bar* new_spaceship(unsigned short x, unsigned short y, Bitmap * ship);

/**
 * @brief Creates and initializes a ball
 *
 * @param x X coordinate
 * @param y Y coordinate
 * @param gameBall Pointer to the bitmap of the ball
 *
 * @return Returns a pointer to the ball created
 */
Ball* new_energyBall(unsigned short x, unsigned short y, Bitmap * gameBall);

/**
 * @brief Draws the game's background BMP for the change character menu
 *
 * @param CrackofDoom Pointer to Game struct
 */
void drawInitialChangeCharBMPs(Game * CrackofDoom);

/**
 * @brief Draws the game's background BMP for the main menu
 *
 * @param CrackofDoom Pointer to Game struct
 */
void drawInitialMenuBMPs(Game * CrackofDoom);

/**
 * @brief Draws the actual game0s background BMPs such as the background, the grids and ornaments
 *
 * @param CrackofDoom Pointer to Game struct
 */
void drawInitialGameBMPs(Game * CrackofDoom);

/**
 * @brief Draws an arrow at the coordinates specified in the struct passed as a parameter
 *
 * @param arrow Arrow struct
 */
void drawArrowBMP(Explosion_Arrow * arrow);

/**
 * @brief Draws a bar at the coordinates specified in the struct passed as a parameter
 *
 * @param bar Bar struct
 */
void drawBarBMP(Bar * bar);

/**
 * @brief Draws a ball at the coordinates specified in the struct passed as a parameter
 *
 * @param Ball struct
 */
void drawBallBMP(Ball * ball);

/**
 * @brief Draws the "Pick one" menu
 *
 * @param game Game struct
 */
void drawPickBMP(Game * game);

/**
 * @brief Draws the "game over" BMP of when the user looses
 *
 * @param game Pointer to Game struct
 */
void drawGameOverBMP(Game * game);

/**
 * @brief Draws the "game over" BMP of when the user wins
 *
 * @param game Pointer to Game struct
 */
void drawVictoryBMP(Game * game);

/**
 * @brief Draws the score at the location it should be when the user is playing
 *
 * @param game Pointer to Gamestruct
 */
void drawScore(Game * game);

/**
 * @brief Draws the remaining lives at the location it should be when the user is playing
 *
 * @param game Pointer to Game struct
 */
void drawLives(Game * game);

/**
 * @brief Draws the score at the location it should be when the user looses
 *
 * @para game Pointer to Game struct
 */
void drawScoreGO(Game * game);

/**
 * @brief Draws the score at the location it should be when the wins
 *
 * @param game Pointer to Game struct
 */
void drawScoreVictory(Game * game);


/**
 * @brief Releases the memory allocated to store all the game's BMPs in the game structure
 *
 * @param CrackofDoom Pointer to Game struct
 */
void deleteGameBMPs(Game * CrackofDoom);

/**
 * @brief Releases the memory allocated to store the BMP passed as a parameter
 *
 * @param bmp Pointer to a Bitmap struct
 */
void deleteBMP(Bitmap * bmp);

/**
 * @brief Computes a ball trajectory, detects collisions and updates her position
 *
 * @param game Pointer to Game struct. Used to update game members like score, number of blocks left, etc
 * @param bar Pointer to Bar struct. Used to detect collisions
 * @param ball Pointer to Ball struct. Used to have access to the ball's coordinates, width, heigh, and displacement values
 */
void ballMovement(Game * game, Bar * bar, Ball * ball);

/**
 * @brief Fill's the matrix (2D array) of drones (blocks) pointers with NULL pointers
 */
void fill_matrix();

/**
 * @brief Draw's the bitmap's pointed by each position of the array named matrix  except if it the pointer is NULL
 *
 * @param CrackofDoom Pointer to Game struct which contains the drone bmp pointer
 */
void draw_matrix_drones(Game * CrackofDoom);

/**
 * @brief Assigns Drone pointer's to each position in the matrix, updates number of blocks on the game as he does it
 *
 * @param CrackofDoom Pointer to Game struct
 */
void allocate_matrix_drones(Game * CrackofDoom);

#endif /* GRAPHICS_H */
