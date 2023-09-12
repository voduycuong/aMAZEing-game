#include "game.h"

#define PLAYER_STEP 24
#define PLAYER_RADIUS 5
#define DEFAULT_FOV_RADIUS 60;

// Colors
#define GUTS 0x0068ADFF
#define GRIFFITH 0x007030A0
#define WALL 0x00000000
#define PATH 0x00FFFFFF
#define BOMB 0x00DC143C
#define STAR 0x00FFC000
#define KEY 0x0070AD47

int star_flag = 1;
int bomb_flag = 2;
int key_flag = 3;

Entity guts;
Entity griffith;
Entity star;
Entity bomb;
Entity key;

void game(int level)
{
    // Initialize position
    Position start_pos1 = {PLAYER_STEP / 2, MAZE_WIDTH / 2 - PLAYER_STEP};
    Position start_pos2 = {MAZE_WIDTH / 2 - PLAYER_STEP, PLAYER_STEP / 2 + PLAYER_STEP};
    Position end_pos = {MAZE_WIDTH - (PLAYER_STEP / 2), MAZE_WIDTH / 2 + PLAYER_STEP};
    Position star_pos = {PLAYER_STEP / 2 + PLAYER_STEP, MAZE_WIDTH / 2};
    Position bomb_pos = {PLAYER_STEP / 2 + PLAYER_STEP, MAZE_WIDTH / 2 + PLAYER_STEP};
    Position key_pos = {PLAYER_STEP / 2 + PLAYER_STEP, MAZE_WIDTH / 2 + PLAYER_STEP + PLAYER_STEP};

    // Initialize hitbox
    Box guts_box = {start_pos1, PLAYER_RADIUS, PLAYER_RADIUS};
    Box griffith_box = {start_pos2, PLAYER_RADIUS, PLAYER_RADIUS};
    Box star_box = {star_pos, PLAYER_RADIUS, PLAYER_RADIUS};
    Box bomb_box = {bomb_pos, PLAYER_RADIUS, PLAYER_RADIUS};
    Box key_box = {key_pos, PLAYER_RADIUS, PLAYER_RADIUS};

    guts.box = guts_box;
    griffith.box = griffith_box;
    star.box = star_box;
    bomb.box = bomb_box;
    key.box = key_box;

    guts.FOV_radius = DEFAULT_FOV_RADIUS;
    griffith.FOV_radius = DEFAULT_FOV_RADIUS;

    load_full_maze(level);

    // Randomize
    do
    {
        star.box.pos = set_random_position();
        bomb.box.pos = set_random_position();
        key.box.pos = set_random_position();
    } while (getPixelARGB32(star.box.pos.x, star.box.pos.y) == WALL ||
             getPixelARGB32(bomb.box.pos.x, bomb.box.pos.y) == WALL ||
             getPixelARGB32(key.box.pos.x, key.box.pos.y) == WALL ||
             star.box.pos.x == bomb.box.pos.x ||
             star.box.pos.y == bomb.box.pos.y);

    clear_maze();

    while (1)
    {
        if (win(guts.box.pos, end_pos, key_flag))
        {
            clear_maze();
            uart_puts("ok you win! more?\n");
            char input = uart_getc();
            if (input == 'y')
            {
                // Reset FOV
                guts.FOV_radius = DEFAULT_FOV_RADIUS;
                griffith.FOV_radius = DEFAULT_FOV_RADIUS;

                // Reset flags
                star_flag = 1;
                bomb_flag = 2;
                key_flag = 3;

                break;
            }

            else if (input == 'n')
                exit();
        }

        else
        {
            make_fov(guts.box.pos, guts.FOV_radius, level);
            make_fov(griffith.box.pos, griffith.FOV_radius, level);
            drawCircleARGB32(guts.box.pos.x, guts.box.pos.y, PLAYER_RADIUS, GUTS);
            drawCircleARGB32(griffith.box.pos.x, griffith.box.pos.y, PLAYER_RADIUS, GRIFFITH);

            if (star_flag == 1)
                drawCircleARGB32(star.box.pos.x, star.box.pos.y, PLAYER_RADIUS, STAR);
            if (bomb_flag == 2)
                drawCircleARGB32(bomb.box.pos.x, bomb.box.pos.y, PLAYER_RADIUS, BOMB);
            if (key_flag == 3)
                drawCircleARGB32(key.box.pos.x, key.box.pos.y, PLAYER_RADIUS, KEY);

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
            }
        }
        break;

    case 'o': // Star
        increase_fov(entity->box.pos, &entity->FOV_radius);
        break;

    case 'p': // Bomb
        decrease_fov(entity->box.pos, &entity->FOV_radius);
        break;

    default:
        break;
    }
}

// Draw field of view
void make_fov(Position pos, int rad, int level)
{
    for (int y = 0; y < MAZE_HEIGHT; y++)
        for (int x = 0; x < MAZE_WIDTH; x++)
            if (x * x + y * y < rad * rad)
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
void clear_fov(Position pos, int rad)
{
    for (int y = 0; y < MAZE_HEIGHT; y++)
        for (int x = 0; x < MAZE_WIDTH; x++)
            if (x * x + y * y <= rad * rad)
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
        *radius += 30;
}

// Decrease field of view
void decrease_fov(Position pos, int *radius)
{
    drawCircleARGB32(pos.x, pos.y, *radius, WALL);
    if (*radius > 40)
        *radius -= 30;
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

Position set_random_position()
{
    Position temp_pos;

    temp_pos.x = rand(5, 30) * PLAYER_STEP + PLAYER_STEP / 2;
    temp_pos.y = rand(5, 30) * PLAYER_STEP + PLAYER_STEP / 2;

    return temp_pos;
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
            drawCircleARGB32(entity2->box.pos.x, entity2->box.pos.y, PLAYER_RADIUS, PATH);
            temp = 0;
        }

        else if (temp == 2) // Bomb
        {
            decrease_fov(entity1->box.pos, &entity1->FOV_radius);
            drawCircleARGB32(entity2->box.pos.x, entity2->box.pos.y, PLAYER_RADIUS, PATH);
            temp = 0;
        }

        else if (temp == 3) // Key
        {
            drawCircleARGB32(entity2->box.pos.x, entity2->box.pos.y, PLAYER_RADIUS, PATH);
            temp = 0;
        }
    }

    *flag = temp;
}