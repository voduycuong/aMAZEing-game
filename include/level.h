#ifndef _LEVEL_H
#define _LEVEL_H

#include "uart.h"
#include "framebf.h"
#include "font.h"
#include "game.h"
#include "maze.h"
#include "kernel.h"

extern unsigned long *epd_bitmap_level_background[1];

void choose_level();
void show_box(int level);
void load_background();

#endif