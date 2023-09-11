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
    int width;
    int height;
} Box;

typedef struct
{
    Box box;
} Entity;

void game();
void handle_input(Position *pos, int input);
void make_fov(Position pos, int rad);
void clear_fov(Position pos, int rad);
void increase_fov(Position pos);
void decrease_fov(Position pos);
int win(Position pos, Position win, int flag);
int interact(int pos_x, int pos_y);
int detect_collision(Box a, Box b);
Position set_random_position();
void load_full_maze();
void clear_maze();
void check_entity(Position pos, int *flag);

#endif