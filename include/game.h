#ifndef _GAME_H
#define _GAME_H

#include "uart.h"
#include "mbox.h"
#include "framebf.h"
#include "maze.h"
#include "math.h"
#include "exit.h"
#include "animation.h"
#include "icon.h"
#include "font.h"

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

typedef enum
{
    GUTS_FRONT_IDLE,
    GUTS_FRONT_WALK1,
    GUTS_FRONT_WALK2,
    GUTS_BACK_IDLE,
    GUTS_BACK_WALK1,
    GUTS_BACK_WALK2,
    GUTS_RIGHT_IDLE,
    GUTS_RIGHT_WALK1,
    GUTS_RIGHT_WALK2,
    GUTS_LEFT_IDLE,
    GUTS_LEFT_WALK1,
    GUTS_LEFT_WALK2,
    GRIFFITH_FRONT_IDLE,
    GRIFFITH_FRONT_WALK1,
    GRIFFITH_FRONT_WALK2,
    GRIFFITH_BACK_IDLE,
    GRIFFITH_BACK_WALK1,
    GRIFFITH_BACK_WALK2,
    GRIFFITH_RIGHT_IDLE,
    GRIFFITH_RIGHT_WALK1,
    GRIFFITH_RIGHT_WALK2,
    GRIFFITH_LEFT_IDLE,
    GRIFFITH_LEFT_WALK1,
    GRIFFITH_LEFT_WALK2,
} AnimationState;

typedef enum
{
    TRAP_FRAME,
    STAR_FRAME,
    KEY_FRAME,
    BOMB_FRAME,
    LEVER_FRAME,
} IconFrame;

typedef struct
{
    Box box;
    int FOV_radius;
    AnimationState currentFrame;
    IconFrame iconFrame;
} Entity;

void game(int *level);
void handle_input(Entity *entity, int input, int level);
void make_fov(Position pos, int radius, int level);
void clear_fov(Position pos, int radius);
void increase_fov(Position pos, int *radius);
void decrease_fov(Position pos, int *radius);
int reach_exit_gate(Position pos, Position win);
int walkable(Box character);
int detect_collision(Box a, Box b);
void clear_maze();
void check_entity(Entity *entity1, Entity *entity2, int *flag);
void set_maze_entity_position(int level, int *path, Position *start2, Position *star, Position *bomb, Position *key, Position *trap, Position *trapless, int *fov);
void set_level();
void drawCharacterFrame(Position pos, AnimationState state);
void clearCharacterFrame(Position pos);
void handleAndAnimateCharacterMovement(Entity *entity, int input, int level);
void drawIconFrame(Position pos, IconFrame frame);
int in_FOV(Entity entity1, Entity entity2);

#endif