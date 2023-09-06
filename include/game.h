#ifndef _GAME_H
#define _GAME_H

#include "uart.h"
#include "mbox.h"
#include "framebf.h"
#include "pic.h"

void game();
void handle_input(int input);

#endif