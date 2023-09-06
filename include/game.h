#ifndef _GAME_H
#define _GAME_H

#include "uart.h"
#include "mbox.h"
#include "framebf.h"
#include "rand.h"

#include "maze.h"
#include "exit.h"

typedef struct
{
    int x;
    int y;
} Position;

typedef struct
{
    Position pos;
} Entity;

void game();
void handle_input(Position *pos, int input);
void make_fov(Position pos, int rad);
void clear_fov(Position pos, int rad);
void increase_fov();
void decrease_fov();
int win(Position pos, Position win);
char interact(int pos_x, int pos_y);
Position set_random_position();
void check_entity(int pos_x, int pos_y, int *flag);

#endif