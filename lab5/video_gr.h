#ifndef __VIDEO_GR_H
#define __VIDEO_GR_H

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

int vgdrawing_square(unsigned short x, unsigned short y, unsigned long color, unsigned long size);

int vgdrawing_line(unsigned short xi, unsigned short yi, unsigned short xf, unsigned short yf, unsigned long color);

int vgsprite_displaying(char *xpm[],unsigned short xi, unsigned short yi);

int vgsprite_displaying2(unsigned short xi, unsigned short yi, char *xpm[]);

int pixel_coloring(unsigned short x, unsigned short y, unsigned long color);

int clear_Screen();


/**
 * @brief Returns to default Minix 3 text mode (0x03: 25 x 80, 16 colors)
 * 
 * @return 0 upon success, non-zero upon failure
 */
int vg_exit(void);

 /** @} end of video_gr */
 
#endif /* __VIDEO_GR_H */
