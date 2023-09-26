#ifndef _KERNEL_H
#define _KERNEL_H

#include "uart.h"
#include "framebf.h"
#include "font.h"
#include "game.h"
#include "level.h"
#include "tutorial.h"
#include "about.h"
#include "title.h"

extern char *commands[];

#define MAX_CMD_SIZE 100
#define MAX_HISTORY_SIZE 30
#define NUM_IMAGES 3

void cli();
void clear_screen();
void show_main_menu();
void show_title();
void draw_hd_dragon();

#endif