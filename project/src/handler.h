#ifndef __HANDLER_H
#define __HANDLER_H

/** @defgroup Handler Handler
 * @{
 *
 * This Module takes care of events like subscribing and unsubscribing
 *
 */

#include "timer.h"
#include "KBD.h"
#include "mouse.h"
#include "game.h"
#include "bitmap.h"
#include "video_gr.h"

extern unsigned long IRQSET_KBD;
extern int IRQSET_TIMER;

/**
 * @brief Function with the subscription of the peripherals
 *
 * @return 0 if succeded
 */
int totalsubscribe();
/**
 * @brief Function with the unsubscription of the peripherals
 *
 * @return 0 if succeded
 */
int totalunsubscribe();
/**
 * @brief Function with the
 *
 * @param CrackofDoom Pointer to Game struct, Bar pointer to bar struct, ball pointer to ball struct, arrow pointer to arrow struct
 */
void freeMemory(Game * game, Bar * bar, Ball * ball, Explosion_Arrow * arrow);

#endif /* HANDLER_H */
