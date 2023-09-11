#ifndef _FONT_H
#define _FONT_H

#include "framebf.h"
#include "mbox.h"
#include "uart.h"

void drawCharARGB32(unsigned char ch, int x, int y, unsigned int attr, int zoom);
void drawStringARGB32(int x, int y, char *s, unsigned int attr, int zoom);

#endif