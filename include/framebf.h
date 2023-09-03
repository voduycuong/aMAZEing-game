#ifndef _FRAMEBF_H
#define _FRAMEBF_H

#include "mbox.h"
#include "uart.h"

// Use RGBA32 (32 bits for each pixel)
#define COLOR_DEPTH 32

// Pixel Order: BGR in memory order (little endian --> RGB in byte order)
#define PIXEL_ORDER 0

void framebf_init();
void drawPixelARGB32(int x, int y, unsigned int attr);
void drawRectARGB32(int x1, int y1, int x2, int y2, unsigned int attr, int fill);
void drawALineARGB32(int x1, int y, int x2, unsigned int attr);
void drawACircleARGB32(int center_x, int center_y, int radius, unsigned int attr);
void display_image(int image_num);

#endif