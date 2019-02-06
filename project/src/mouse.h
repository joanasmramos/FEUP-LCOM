#ifndef _MOUSE_H
#define _MOUSE_H

#include "graphics.h"
#include "game.h"
#include <stdlib.h>

/** @defgroup mouse mouse
 * @{
 *
 * Functions related to the mouse
*/

extern unsigned long IRQSET_MOUSE;

#define NRPACKETS 3

typedef struct {
	int xSign, ySign;
	unsigned char deltax;
	unsigned char deltay;
	unsigned char counter;
	unsigned char packet[NRPACKETS];
} Mouse;

//S & U

/**
 * @brief subscribes mouse interrupts
 *
 * @return Returns 0 upon success and -1 otherwise
 */
int mouse_subscribe_int(void);

/**
 * @brief unsubscribes mouse interrupts
 *
 * @return Returns 0 upon success and -1 otherwise
 */
int mouse_unsubscribe_int();

/**
 * @brief subscribes mouse interrupts, sets stream mode, enables data reporting, cleans Out Buf
 *
 * @return Returns 0 upon success and -1 otherwise
 */
int subscribeMouse();

/**
 * @brief unsubscribes mouse interrupts, disables stream mode, disables data reporting, cleans Out BUfe
 *
 * @return Returns 0 upon success and -1 otherwise
 */
int unsubscribeMouse();

//OTHERS

/**
 * @brief Reads the out buf, except if the information comes from the keyboard
 *
 * @return Returns the byte read
 */
unsigned long mouse_reading();

/**
 * @brief Writes the command command to port reg
 *
 * @param reg Port to write to
 * @param command Command to issue
 *
 * @return Returns 0 upon success and -1 otherwise
 */
int mouse_writting(unsigned char reg, unsigned char command);

/**
 * @brief stores the bytes read in mouse's packet array depending on the counter
 *
 * @param mouse Pointer to Mouse struct
 */
int mouse_packet_handler(Mouse * mouse);

/**
 * @brief Cleans out buf
 *
 * @return Returns 0 upon success and -1 otherwise
 */
int clean_out_buf();

/**
 * @brief Function that deals with all the mouse interrupts while the game is running, updates elements pointed to by the parameters accordingly
 *
 * @param mouse Pointer to Mouse struct
 * @param game Pointer to Game struct
 * @param bar Pointer to Bar struct
 * @param ball Pointer to Ball struct
 */
void mouse_handler(Mouse * mouse, Game * game, Bar * bar, Ball * ball);

/**
 * @brief Allocates memory for a mouse struct and initializes it
 *
 * @return Returns pointer to the mouse created
 */
Mouse * initMouse();

/**
 * @brief Converts deltas, and stores x Sign
 *
 * @param mouse Pointer to Mouse struct
 */
void setDeltas(Mouse * mouse);

/**
 * @brief Discards one byte from the Out Buf
 */
void KBC_discard();

#endif /* MOUSE_H */
