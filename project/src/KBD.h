#ifndef KBD_H
#define KBD_H

#include <minix/sysutil.h>
#include <minix/driver.h>
#include "i8042.h"
#include "game.h"
#include "graphics.h"
#include "KBD.h"
#include "mouse.h"

/** @defgroup KBD KBD
 * @{
 *
 * Functions related to the keyboard
*/

//S & U

/**
 * @brief subscribes keyboard interrupts
 *
 * @return Returns 0 upon success and -1 otherwise
 */
int kbd_subscribe_int(void);

/**
 * @brief unsubscribes keyboard interrupts
 *
 * @return Returns 0 upon success and -1 otherwise
 */
int kbd_unsubscribe_int();

/**
 * @brief Handles keyboard interrupts during the entire game, updates elements pointed to by the parameters accordingly
 *
 * @param game Pointer to Game struct
 * @param bar Pointer to Bar struct
 * @param ball Pointer to Ball struct
 * @param arrow Pointer to Explosion_Arrow struct
 * @param mouse Pointer to Mouse struct
 */
void kbd_handler(Game * game, Bar * bar, Ball * ball, Explosion_Arrow * arrow, Mouse * mouse);

/**
 * @brief Reads the outbuf, unless the information comes from the mouse, then in calls the mouse's handler
 *
 * @param mouse Pointer to Mouse struct
 *
 * @return Returns the scancode
 */
unsigned long reading(Mouse * mouse);

#endif /* KBD_H */
