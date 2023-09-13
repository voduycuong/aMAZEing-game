#include "game.h"

#define PLAYER_STEP MAZE_HEIGHT / 19
#define ENTITY_RADIUS 10

// Colors
#define GUTS 0x0068ADFF     // 1st character
#define GRIFFITH 0x007030A0 // 2nd character
#define WALL 0x00000000
#define PATH 0x00FFFFFF
#define BOMB 0x00FF0101 // Red
#define STAR 0x00FFC000 // Yellow == Battery
#define KEY 0x0070AD47  // Green
#define TRAP 0x00FF28F0 // Pink
#define TRAPLESS 0x0027FFDC // Cyan

int default_fov = 60;
int star_flag = 1;
int bomb_flag = 2;
int key_flag = 3;
int trap_flag = 4;
int trapless_flag = 5;

Entity guts;
Entity griffith;
Entity star;
Entity bomb;
Entity key;
Entity trap;
Entity trapless;


void game(int *level)
{
    // Default position of main character and exit gate
    Position start_pos1 = {PLAYER_STEP / 2, PLAYER_STEP * 10 - PLAYER_STEP / 2};                 // No modify
    Position end_pos = {PLAYER_STEP * 19 - PLAYER_STEP / 2, PLAYER_STEP * 10 - PLAYER_STEP / 2}; // No modify

    // Position of other entities based on maze level
    Position start_pos2;
    Position star_pos;
    Position bomb_pos;
    Position key_pos;
    Position trap_pos;
    Position trapless_pos;

    set_maze_entity_position(*level, &start_pos2, &star_pos, &bomb_pos, &key_pos, &trap_pos, &trapless_pos, &default_fov);

    // Initialize hitbox
    Box guts_box = {start_pos1, ENTITY_RADIUS, ENTITY_RADIUS};
    Box griffith_box = {start_pos2, ENTITY_RADIUS, ENTITY_RADIUS};
    Box star_box = {star_pos, ENTITY_RADIUS, ENTITY_RADIUS};
    Box bomb_box = {bomb_pos, ENTITY_RADIUS, ENTITY_RADIUS};
    Box key_box = {key_pos, ENTITY_RADIUS, ENTITY_RADIUS};
    Box trap_box = {trap_pos, ENTITY_RADIUS, ENTITY_RADIUS};
    Box trapless_box = {trapless_pos, ENTITY_RADIUS, ENTITY_RADIUS};

    // Initialize entity
    guts.box = guts_box;
    guts.FOV_radius = default_fov;
    griffith.box = griffith_box;
    griffith.FOV_radius = default_fov;
    star.box = star_box;
    bomb.box = bomb_box;
    key.box = key_box;
    trap.box = trap_box;
    trapless.box = trapless_box;

    clear_maze();
    switch (*level)
    {
    case 0:
        drawStringARGB32(200, 400, "Level 1", 0x00ffffff, 4);
        break;
    case 1:
        drawStringARGB32(200, 400, "Level 2", 0x00ffffff, 4);
        break;
    case 2:
        drawStringARGB32(200, 400, "Level 3", 0x00ffffff, 4);
        break;
    case 3:
        drawStringARGB32(200, 400, "Level 4", 0x00ffffff, 4);
        break;
    case 4:
        drawStringARGB32(200, 400, "Level 5", 0x00ffffff, 4);
        break;
    default:
        break;
    }
    wait_msec(1000000);
    clear_maze();

    while (1)
    {
        if (win(guts.box.pos, end_pos, key_flag))
        {
            clear_maze();
            drawStringARGB32(200, 400, "ok you win!", 0x00ffffff, 3);
            wait_msec(1000000);
            if (*level < 5)
            {
                // Next level
                int temp_level = *level;
                temp_level++;
                *level = temp_level;

                // Set FOV
                guts.FOV_radius = default_fov;
                griffith.FOV_radius = default_fov;

                // Reset flags
                star_flag = 1;
                bomb_flag = 2;
                key_flag = 3;
                trap_flag = 4;
                trapless_flag = 5;
                break;
            }
        }

        else
        {
            make_fov(guts.box.pos, guts.FOV_radius, *level);
            make_fov(griffith.box.pos, griffith.FOV_radius, *level);
            drawCircleARGB32(guts.box.pos.x, guts.box.pos.y, ENTITY_RADIUS, GUTS);
            drawCircleARGB32(griffith.box.pos.x, griffith.box.pos.y, ENTITY_RADIUS, GRIFFITH);

            if (star_flag == 1)
                drawCircleARGB32(star.box.pos.x, star.box.pos.y, ENTITY_RADIUS, STAR);
            if (bomb_flag == 2)
                drawCircleARGB32(bomb.box.pos.x, bomb.box.pos.y, ENTITY_RADIUS, BOMB);
            if (key_flag == 3)
                drawCircleARGB32(key.box.pos.x, key.box.pos.y, ENTITY_RADIUS, KEY);
            if (trap_flag == 4)
                drawCircleARGB32(trap.box.pos.x, trap.box.pos.y, ENTITY_RADIUS, TRAP);
            if (trapless_flag == 5)
                drawCircleARGB32(trapless.box.pos.x, trapless.box.pos.y, ENTITY_RADIUS, TRAPLESS);

            char input = uart_getc();
            handle_input(&guts, input);
            handle_input(&griffith, input);
        }
    }
}

// Function for input directions
void handle_input(Entity *entity, int input)
{
    switch (input)
    {
    case 'w': // Up
        if (!walkable(entity->box.pos.x, entity->box.pos.y - PLAYER_STEP))
        {
            clear_fov(entity->box.pos, entity->FOV_radius);
            if (entity->box.pos.y - PLAYER_STEP > 0)
            {
                entity->box.pos.y -= PLAYER_STEP;
                check_entity(entity, &star, &star_flag);
                check_entity(entity, &bomb, &bomb_flag);
                check_entity(entity, &key, &key_flag);
                check_entity(entity, &trap, &trap_flag);
                check_entity(entity, &trapless, &trapless_flag);
            }
        }
        break;

    case 's': // Down
        if (!walkable(entity->box.pos.x, entity->box.pos.y + PLAYER_STEP))
        {
            clear_fov(entity->box.pos, entity->FOV_radius);
            if (entity->box.pos.y + PLAYER_STEP < MAZE_WIDTH)
            {
                entity->box.pos.y += PLAYER_STEP;
                check_entity(entity, &star, &star_flag);
                check_entity(entity, &bomb, &bomb_flag);
                check_entity(entity, &key, &key_flag);
                check_entity(entity, &trap, &trap_flag);
                check_entity(entity, &trapless, &trapless_flag);
            }
        }
        break;

    case 'a': // Left
        if (!walkable(entity->box.pos.x - PLAYER_STEP, entity->box.pos.y))
        {
            clear_fov(entity->box.pos, entity->FOV_radius);
            if (entity->box.pos.x - PLAYER_STEP > 0)
            {
                entity->box.pos.x -= PLAYER_STEP;
                check_entity(entity, &star, &star_flag);
                check_entity(entity, &bomb, &bomb_flag);
                check_entity(entity, &key, &key_flag);
                check_entity(entity, &trap, &trap_flag);
                check_entity(entity, &trapless, &trapless_flag);
            }
        }
        break;

    case 'd': // Right
        if (!walkable(entity->box.pos.x + PLAYER_STEP, entity->box.pos.y))
        {
            clear_fov(entity->box.pos, entity->FOV_radius);
            if (entity->box.pos.x + PLAYER_STEP < MAZE_HEIGHT)
            {
                entity->box.pos.x += PLAYER_STEP;
                check_entity(entity, &star, &star_flag);
                check_entity(entity, &bomb, &bomb_flag);
                check_entity(entity, &key, &key_flag);
                check_entity(entity, &trap, &trap_flag);
                check_entity(entity, &trapless, &trapless_flag);
            }
        }
        break;

        // Cheat
    case 'o':
        increase_fov(entity->box.pos, &entity->FOV_radius);
        break;
    case 'p':
        decrease_fov(entity->box.pos, &entity->FOV_radius);
        break;
    case 'm':
        decrease_fov(entity->box.pos, &entity->FOV_radius);
        break;

    default:
        break;
    }
}

// Draw field of view
void make_fov(Position pos, int radius, int level)
{
    for (int y = 0; y < MAZE_HEIGHT; y++)
        for (int x = 0; x < MAZE_WIDTH; x++)
            if (x * x + y * y < radius * radius)
            {
                if (pos.x + x < MAZE_WIDTH && pos.y + y < MAZE_HEIGHT)
                    drawPixelARGB32(pos.x + x, pos.y + y, epd_bitmap_allArray[level][(pos.y + y) * MAZE_WIDTH + (pos.x + x)]);
                if (pos.x - x > 0 && pos.y - y > 0)
                    drawPixelARGB32(pos.x - x, pos.y - y, epd_bitmap_allArray[level][(pos.y - y) * MAZE_WIDTH + (pos.x - x)]);
                if (pos.x + x < MAZE_HEIGHT && pos.y - y > 0)
                    drawPixelARGB32(pos.x + x, pos.y - y, epd_bitmap_allArray[level][(pos.y - y) * MAZE_WIDTH + (pos.x + x)]);
                if (pos.x - x > 0 && pos.y + y < MAZE_HEIGHT)
                    drawPixelARGB32(pos.x - x, pos.y + y, epd_bitmap_allArray[level][(pos.y + y) * MAZE_WIDTH + (pos.x - x)]);
            }
}

// Clear field of view
void clear_fov(Position pos, int radius)
{
    for (int y = 0; y < MAZE_HEIGHT; y++)
        for (int x = 0; x < MAZE_WIDTH; x++)
            if (x * x + y * y <= radius * radius)
            {
                drawPixelARGB32(pos.x + x, pos.y + y, WALL);
                drawPixelARGB32(pos.x - x, pos.y - y, WALL);
                drawPixelARGB32(pos.x + x, pos.y - y, WALL);
                drawPixelARGB32(pos.x - x, pos.y + y, WALL);
            }
}

// Increase field of view
void increase_fov(Position pos, int *radius)
{

    drawCircleARGB32(pos.x, pos.y, *radius, WALL);
    if (*radius < 200)
        *radius += 40;
}

// Decrease field of view
void decrease_fov(Position pos, int *radius)
{
    drawCircleARGB32(pos.x, pos.y, *radius, WALL);
    if (*radius > 60)
        *radius -= 40;
}

// Check if player has escaped or not
int win(Position pos, Position win, int flag)
{
    if (pos.x == win.x && pos.y == win.y && flag == 0)
        return 1;
    else
        return 0;
}

int walkable(int pos_x, int pos_y)
{
    if (getPixelARGB32(pos_x, pos_y) == WALL)
        return 1;

    else
        return 0;
}

int detect_collision(Box a, Box b)
{
    if (a.pos.x < b.pos.x + b.width &&
        a.pos.x + a.width > b.pos.x &&
        a.pos.y < b.pos.y + b.height &&
        a.pos.y + a.height > b.pos.y)
    {
        return 1; // Collision detected
    }
    return 0; // No collision
}

void load_full_maze(int level)
{
    for (int y = 0; y <= MAZE_HEIGHT; y++)
        for (int x = 0; x <= MAZE_WIDTH; x++)
            drawPixelARGB32(x, y, epd_bitmap_allArray[level][y * MAZE_WIDTH + x]);
}

void clear_maze()
{
    for (int y = 0; y <= MAZE_HEIGHT; y++)
        for (int x = 0; x <= MAZE_WIDTH; x++)
            drawPixelARGB32(x, y, WALL);
}

void check_entity(Entity *entity1, Entity *entity2, int *flag)
{
    int temp = *flag;

    if (detect_collision(entity1->box, entity2->box))
    {
        if (temp == 1) // Star
        {
            increase_fov(entity1->box.pos, &entity1->FOV_radius);
            drawCircleARGB32(entity2->box.pos.x, entity2->box.pos.y, ENTITY_RADIUS, PATH);
            temp = 0;
        }

        else if (temp == 2) // Bomb
        {
            decrease_fov(entity1->box.pos, &entity1->FOV_radius);
            drawCircleARGB32(entity2->box.pos.x, entity2->box.pos.y, ENTITY_RADIUS, PATH);
            temp = 0;
        }

        else if (temp == 3) // Key
        {
            drawCircleARGB32(entity2->box.pos.x, entity2->box.pos.y, ENTITY_RADIUS, PATH);
            temp = 0;
        }

        else if (temp == 4) // Trap
        {
            drawCircleARGB32(entity2->box.pos.x, entity2->box.pos.y, ENTITY_RADIUS, PATH);
            temp = 0;
        }

        else if (temp == 5) // Trapless
        {
            drawCircleARGB32(entity2->box.pos.x, entity2->box.pos.y, ENTITY_RADIUS, PATH);
            temp = 0;        }
    }

    *flag = temp;
}

void set_maze_entity_position(int level, Position *start2, Position *star, Position *bomb, Position *key, Position *trap, Position *trapless, int *fov)
{
    switch (level)
    {
    case 0:
        start2->x = PLAYER_STEP * 3 - PLAYER_STEP / 2;
        start2->y = PLAYER_STEP * 18 - PLAYER_STEP / 2;

        star->x = PLAYER_STEP * 4 - PLAYER_STEP / 2,
        star->y = PLAYER_STEP * 8 - PLAYER_STEP / 2;

        bomb->x = PLAYER_STEP * 7 - PLAYER_STEP / 2;
        bomb->y = PLAYER_STEP * 10 - PLAYER_STEP / 2;

        key->x = PLAYER_STEP * 16 - PLAYER_STEP / 2;
        key->y = PLAYER_STEP * 12 - PLAYER_STEP / 2;

        trap->x = PLAYER_STEP * 3 - PLAYER_STEP / 2;
        trap->y = PLAYER_STEP * 6 - PLAYER_STEP / 2;

        trapless->x = PLAYER_STEP * 18 - PLAYER_STEP / 2;
        trapless->y = PLAYER_STEP * 18 - PLAYER_STEP / 2;

        *fov = 60;

        break;

    case 1:
        // Level 2

        break;
    case 2:
        // Level 3

        break;
    case 3:
        // Level 4

        break;
    case 4:
        // Level 5

        break;

    default:
        break;
    }
}