#include "game.h"

#define PLAYER_STEP 24
#define PLAYER_RADIUS 5

// Colors
#define GUTS 0x0068ADFF
#define GRIFFITH 0x007030A0
#define WALL 0x00000000
#define PATH 0x00FFFFFF
#define BOMB 0x00DC143C
#define STAR 0x00A459D1
#define KEY 0x0070AD47

int32_t FOV_RADIUS = 60;
int32_t star_flag = 1;
int32_t bomb_flag = 1;
int32_t key_flag = 1;

Entity guts;
Entity griffith;
Entity star;
Entity bomb;
Entity key;

void game()
{
    Position start_pos1 = {PLAYER_STEP / 2, MAZE_WIDTH / 2 - PLAYER_STEP};
    Position start_pos2 = {MAZE_WIDTH / 2 - PLAYER_STEP, PLAYER_STEP / 2 + PLAYER_STEP};
    Position end_pos = {MAZE_WIDTH - (PLAYER_STEP / 2), MAZE_WIDTH / 2 + PLAYER_STEP};
    Position star_pos = {0, 0};
    Position bomb_pos = {0, 0};
    Position key_pos = {0, 0};

    Box guts_box = {start_pos1, 5, 5};
    Box griffith_box = {start_pos2, 5, 5};
    Box star_box = {star_pos, 5, 5};
    Box bomb_box = {bomb_pos, 5, 5};
    Box key_box = {key_pos, 5, 5};

    guts.box = guts_box;
    griffith.box = griffith_box;
    star.box = star_box;
    bomb.box = bomb_box;
    key.box = key_box;

    load_full_maze();

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
                FOV_RADIUS = 60;
                star_flag = 1;
                bomb_flag = 1;
                key_flag = 1;
                break;
            }
            else if (input == 'n')
                exit();
        }

        else
        {
            make_fov(guts.box.pos, FOV_RADIUS);
            make_fov(griffith.box.pos, FOV_RADIUS);
            drawCircleARGB32(guts.box.pos.x, guts.box.pos.y, PLAYER_RADIUS, GUTS);
            drawCircleARGB32(griffith.box.pos.x, griffith.box.pos.y, PLAYER_RADIUS, GRIFFITH);

            if (star_flag == 1)
                drawCircleARGB32(star.box.pos.x, star.box.pos.y, PLAYER_RADIUS, STAR);
            if (bomb_flag == 1)
                drawCircleARGB32(bomb.box.pos.x, bomb.box.pos.y, PLAYER_RADIUS, BOMB);
            if (key_flag == 1)
                drawCircleARGB32(key.box.pos.x, key.box.pos.y, PLAYER_RADIUS, KEY);

            char input = uart_getc();
            handle_input(&guts.box.pos, input);
            handle_input(&griffith.box.pos, input);
        }
    }
}

// Function for input directions
void handle_input(Position *pos, int input)
{
    switch (input)
    {
    case 'w': // Up
        if (interact(pos->x, pos->y - PLAYER_STEP) == 's')
            star_flag = 0;
        if (interact(pos->x, pos->y - PLAYER_STEP) == 'b')
            bomb_flag = 2;
        if (interact(pos->x, pos->y - PLAYER_STEP) == 'k')
            key_flag = 3;
        if (interact(pos->x, pos->y - PLAYER_STEP) != 'w')
        {
            clear_fov(*pos, FOV_RADIUS);
            if (pos->y - PLAYER_STEP > 0)
            {
                pos->y -= PLAYER_STEP;
                check_entity(*pos, &star_flag);
                check_entity(*pos, &bomb_flag);
                check_entity(*pos, &key_flag);
            }
        }
        break;

    case 's': // Down
        if (interact(pos->x, pos->y + PLAYER_STEP) == 's')
            star_flag = 0;
        if (interact(pos->x, pos->y + PLAYER_STEP) == 'b')
            bomb_flag = 2;
        if (interact(pos->x, pos->y + PLAYER_STEP) == 'k')
            key_flag = 3;
        if (interact(pos->x, pos->y + PLAYER_STEP) != 'w')
        {
            clear_fov(*pos, FOV_RADIUS);
            if (pos->y + PLAYER_STEP < MAZE_WIDTH)
            {
                pos->y += PLAYER_STEP;
                check_entity(*pos, &star_flag);
                check_entity(*pos, &bomb_flag);
                check_entity(*pos, &key_flag);
            }
        }
        break;

    case 'a': // Left
        if (interact(pos->x - PLAYER_STEP, pos->y) == 's')
            star_flag = 0;
        if (interact(pos->x - PLAYER_STEP, pos->y) == 'b')
            bomb_flag = 2;
        if (interact(pos->x - PLAYER_STEP, pos->y) == 'k')
            key_flag = 3;
        if (interact(pos->x - PLAYER_STEP, pos->y) != 'w')
        {
            clear_fov(*pos, FOV_RADIUS);
            if (pos->x - PLAYER_STEP > 0)
            {
                pos->x -= PLAYER_STEP;
                check_entity(*pos, &star_flag);
                check_entity(*pos, &bomb_flag);
                check_entity(*pos, &key_flag);
            }
        }
        break;

    case 'd': // Right
        if (interact(pos->x + PLAYER_STEP, pos->y) == 's')
            star_flag = 0;
        if (interact(pos->x + PLAYER_STEP, pos->y) == 'b')
            bomb_flag = 2;
        if (interact(pos->x + PLAYER_STEP, pos->y) == 'k')
            key_flag = 3;
        if (interact(pos->x + PLAYER_STEP, pos->y) != 'w')
        {
            clear_fov(*pos, FOV_RADIUS);
            if (pos->x + PLAYER_STEP < MAZE_HEIGHT)
            {
                pos->x += PLAYER_STEP;
                check_entity(*pos, &star_flag);
                check_entity(*pos, &bomb_flag);
                check_entity(*pos, &key_flag);
            }
        }
        break;

    case 'o': // Star
        increase_fov(*pos);
        break;

    case 'p': // Bomb
        decrease_fov(*pos);
        break;

    default:
        break;
    }
}

// Draw field of view
void make_fov(Position pos, int rad)
{
    for (int y = 0; y < MAZE_HEIGHT; y++)
        for (int x = 0; x < MAZE_WIDTH; x++)
            if (x * x + y * y < rad * rad)
            {
                if (pos.x + x < MAZE_WIDTH && pos.y + y < MAZE_HEIGHT)
                    drawPixelARGB32(pos.x + x, pos.y + y, epd_bitmap_allArray[0][(pos.y + y) * MAZE_WIDTH + (pos.x + x)]);
                if (pos.x - x > 0 && pos.y - y > 0)
                    drawPixelARGB32(pos.x - x, pos.y - y, epd_bitmap_allArray[0][(pos.y - y) * MAZE_WIDTH + (pos.x - x)]);
                if (pos.x + x < MAZE_HEIGHT && pos.y - y > 0)
                    drawPixelARGB32(pos.x + x, pos.y - y, epd_bitmap_allArray[0][(pos.y - y) * MAZE_WIDTH + (pos.x + x)]);
                if (pos.x - x > 0 && pos.y + y < MAZE_HEIGHT)
                    drawPixelARGB32(pos.x - x, pos.y + y, epd_bitmap_allArray[0][(pos.y + y) * MAZE_WIDTH + (pos.x - x)]);
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
void increase_fov(Position pos)
{
    drawCircleARGB32(pos.x, pos.y, FOV_RADIUS, WALL);
    if (FOV_RADIUS < 200)
        FOV_RADIUS += 60;
}

// Decrease field of view
void decrease_fov(Position pos)
{
    drawCircleARGB32(pos.x, pos.y, FOV_RADIUS, WALL);
    if (FOV_RADIUS > 30)
        FOV_RADIUS -= 20;
}

// Check if player has escaped or not
int win(Position pos, Position win, int flag)
{
    if (pos.x == win.x && pos.y == win.y && flag == -1)
        return 1;
    else
        return 0;
}

int interact(int pos_x, int pos_y)
{
    if (getPixelARGB32(pos_x, pos_y) == WALL)
        return 'w';
    else if (detect_collision(guts.box, star.box) == 1)
    {
        uart_puts("Star found\n");
        return 's';
    }
    else if (detect_collision(guts.box, bomb.box) == 1)
    {
        uart_puts("Bomb found\n");
        return 'b';
    }
    else if (detect_collision(guts.box, key.box) == 1)
    {
        uart_puts("Key found\n");
        return 'k';
    }
    else
        return 'n';
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

void load_full_maze()
{
    for (int y = 0; y <= MAZE_HEIGHT; y++)
        for (int x = 0; x <= MAZE_WIDTH; x++)
            drawPixelARGB32(x, y, epd_bitmap_allArray[0][y * MAZE_WIDTH + x]);
}

void clear_maze()
{
    for (int y = 0; y <= MAZE_HEIGHT; y++)
        for (int x = 0; x <= MAZE_WIDTH; x++)
            drawPixelARGB32(x, y, WALL);
}

void check_entity(Position pos, int *flag)
{
    int temp = *flag;

    if (temp == 0)
    {
        drawCircleARGB32(pos.x, pos.y, PLAYER_RADIUS, WALL);
        increase_fov(pos);
        temp = -1;
    }

    else if (temp == 2)
    {
        drawCircleARGB32(pos.x, pos.y, PLAYER_RADIUS, WALL);
        decrease_fov(pos);
        temp = -1;
    }

    else if (temp == 3)
    {
        drawCircleARGB32(pos.x, pos.y, PLAYER_RADIUS, WALL);
        temp = -1;
    }

    *flag = temp;
}