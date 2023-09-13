#ifndef _GAME_H
#define _GAME_H

#include "uart.h"
#include "mbox.h"
#include "framebf.h"
#include "rand.h"
#include "level.h"
#include "maze.h"
#include "printf.h"
#include "math.h"
#include "exit.h"

extern int level;

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
    int FOV_radius;
} Entity;

void game(int *level);
void handle_input(Entity *entity, int input);
void make_fov(Position pos, int radius, int level);
void clear_fov(Position pos, int radius);
void increase_fov(Position pos, int *radius);
void decrease_fov(Position pos, int *radius);
int win(Position pos, Position win, int flag);
int walkable(int pos_x, int pos_y);
int detect_collision(Box a, Box b);
void load_full_maze(int level);
void clear_maze();
void check_entity(Entity *entity1, Entity *entity2, int *flag);
void set_maze_entity_position(int level, Position *start2, Position *star, Position *bomb, Position *key, int *fov);

#endif