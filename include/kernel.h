#ifndef _KERNEL_H
#define _KERNEL_H

#include "uart.h"
#include "framebf.h"
#include "font.h"

// Commands
#include "level.h"
#include "tutorial.h"
#include "about.h"
#include "exit.h"

#include "game.h"

extern char *commands[];

#define MAX_CMD_SIZE 100
#define MAX_HISTORY_SIZE 30
#define NUM_IMAGES 3

void cli();
void show_welcome_screen();
void clear_screen();

#endif