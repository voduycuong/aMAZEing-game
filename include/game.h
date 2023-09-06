#ifndef _GAME_H
#define _GAME_H

#include "uart.h"
#include "mbox.h"
#include "framebf.h"
#include "maze.h"
#include "exit.h"

void game();
void handle_input(int *pos_x, int *pos_y, int input);
void make_fov(int pos_x, int pos_y, int rad);
void clear_fov(int pos_x, int pos_y, int rad);
void increase_fov();
void decrease_fov();
int win(int pos_x, int pos_y, int win_x, int win_y);
int walkable(int pos_x, int pos_y);

#endif