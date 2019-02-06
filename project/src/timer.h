#ifndef __TIMER_H
#define __TIMER_H

#include "game.h"
#include "graphics.h"

/** @defgroup timer timer
 * @{
 *
 * Functions for using the i8254 timers
 */

/**
 * @brief Subscribes and enables Timer 0 interrupts
 *
 * @return Returns bit order in interrupt mask; negative value on failure
 */
int timer_subscribe_int(void );

/**
 * @brief Unsubscribes Timer 0 interrupts
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int timer_unsubscribe_int();

/**
 * @brief Function to deal with the timer's interrupts, also updates the VRAM at every tick
 *
 * @param game Pointer to Game struct
 * @param bar Pointer to Bar struct
 * @param ball Pointer to Ball struct
 * @param arrow Pointer to Explosion_Arrow struct
 */
void timer_handler(Game * game, Bar * bar, Ball * ball, Explosion_Arrow * arrow);

#endif /* __TIMER_H */
