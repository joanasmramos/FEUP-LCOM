#ifndef __VIDEO_GR_H
#define __VIDEO_GR_H

#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include <sys/mman.h>
#include <sys/types.h>
#include "i8042.h"
#include "vbe.h"
#include <math.h>

/** @defgroup video_gr video_gr
 * @{
 *
 * Functions for outputing data to screen in graphics mode
 */

/**
 * @brief Initializes the video module in graphics mode
 * 
 * Uses the VBE INT 0x10 interface to set the desired
 *  graphics mode, maps VRAM to the process' address space and
 *  initializes static global variables with the resolution of the screen, 
 *  and the number of colors
 * 
 * @param mode 16-bit VBE mode to set
 * @return Virtual address VRAM was mapped to. NULL, upon failure.
 */
void *vg_init(unsigned short mode);

#define VRAM_PHYS_ADDR	0xF0000000
// the screen is a set of V_RES lines each of which with H_RES pixels
#define H_RES           1024
#define V_RES		  	768
#define BITS_PER_PIXEL	16
#define VBE_MODE	0x117

int pixel_coloring(unsigned short x, unsigned short y, unsigned long color);

int clear_Screen();

/**
 * @brief Returns address of video memory
 *
 * @return Address of video memory
 */
void* getVideoMem();

/**
 * @brief Returns Horizonal Resolution
 */
unsigned getHorResolution();

/**
 * @brief Returns Vertical Resolution
 */
unsigned getVerResolution();

/**
 * @brief Returns VRAM's size
 */
unsigned getVramSize();

/**
 * @brief Returns the second buffer's address
 */
void* getDoubleBuffer();

/**
 * @brief Copies contents of the second buffer to the video memory
 */
void doubleBuffering();


/**
 * @brief Returns to default Minix 3 text mode (0x03: 25 x 80, 16 colors)
 * 
 * @return 0 upon success, non-zero upon failure
 */
int vg_exit();

 /** @} end of video_gr */
 
#endif /* __VIDEO_GR_H */
