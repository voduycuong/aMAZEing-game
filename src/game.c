#include "game.h"

typedef struct
{
    int x;
    int y;
} Position;

typedef struct
{
    Position pos;
} Player;

#define PLAYER_STEP 24
#define PLAYER_RADIUS 5

// Colors
#define BLUE 0x0068ADFF
#define BLACK 0x00000000
#define RED 0x00DC143C

int32_t FOV_RADIUS = 60;

Player guts;
// Player griffith;

void game()
{
    Position start_pos1 = {PLAYER_STEP / 2, MAZE_WIDTH / 2 - PLAYER_STEP};
    // Position start_pos2 = {200, 400};
    Position end_pos = {MAZE_WIDTH - (PLAYER_STEP / 2), MAZE_WIDTH / 2 + PLAYER_STEP};

    guts.pos = start_pos1;
    // griffith.pos = start_pos2;

    while (1)
    {
        if (win(guts.pos.x, guts.pos.y, end_pos.x, end_pos.y))
        {
            uart_puts("ok you win! want some more?\n");
            char input = uart_getc();
            if (input == 'y')
            {
                FOV_RADIUS = 60;
                break;
            }
            else if (input == 'n')
                exit();
        }

        else
        {
            // // Full view
            // for (int y = 0; y <= MAZE_HEIGHT; y++)
            //     for (int x = 0; x <= MAZE_WIDTH; x++)
            //         drawPixelARGB32(guts.pos.x + x, guts.pos.y + y, epd_bitmap_allArray[0][(guts.pos.y + y) * MAZE_WIDTH + (guts.pos.x + x)]);

            make_fov(guts.pos.x, guts.pos.y, FOV_RADIUS);
            drawCircleARGB32(guts.pos.x, guts.pos.y, PLAYER_RADIUS, BLUE);

            // make_fov(griffith.pos.x, griffith.pos.y, FOV_RADIUS);
            // drawCircleARGB32(griffith.pos.x, griffith.pos.y, PLAYER_RADIUS, RED);

            // uart_puts("\nColor: ");
            // uart_hex(getPixelARGB32(guts.pos.x, guts.pos.y));

            // uart_puts("\nWalkable: ");
            // uart_dec(walkable(guts.pos.x, guts.pos.y - PLAYER_STEP));

            char input = uart_getc();
            handle_input(&guts.pos.x, &guts.pos.y, input);
        } // handle_input(&griffith.pos.x, &griffith.pos.y, input);
    }
}

// Function for input directions
void handle_input(int *pos_x, int *pos_y, int input)
{
    switch (input)
    {
    case 'w': // Up
        if (walkable(*pos_x, *pos_y - PLAYER_STEP))
        {
            clear_fov(*pos_x, *pos_y, FOV_RADIUS);
            if (*pos_y - PLAYER_STEP > 0)
                *pos_y -= PLAYER_STEP;
        }
        break;

    case 's': // Down
        if (walkable(*pos_x, *pos_y + PLAYER_STEP))
        {
            clear_fov(*pos_x, *pos_y, FOV_RADIUS);
            if (*pos_y + PLAYER_STEP < MAZE_WIDTH)
                *pos_y += PLAYER_STEP;
        }
        break;

    case 'a': // Left
        if (walkable(*pos_x - PLAYER_STEP, *pos_y))
        {
            clear_fov(*pos_x, *pos_y, FOV_RADIUS);
            if (*pos_x - PLAYER_STEP > 0)
                *pos_x -= PLAYER_STEP;
        }
        break;

    case 'd': // Right
        if (walkable(*pos_x + PLAYER_STEP, *pos_y))
        {
            clear_fov(*pos_x, *pos_y, FOV_RADIUS);
            if (*pos_x + PLAYER_STEP < MAZE_HEIGHT)
                *pos_x += PLAYER_STEP;
        }
        break;

    case 'o': // Star
        increase_fov();
        break;

    case 'p': // Trap
        decrease_fov();
        break;

    default:
        break;
    }
}

// Draw field of view
void make_fov(int pos_x, int pos_y, int rad)
{
    for (int y = 0; y < MAZE_HEIGHT; y++)
        for (int x = 0; x < MAZE_WIDTH; x++)
            if (x * x + y * y < rad * rad)
            {
                if (pos_x + x < MAZE_WIDTH && pos_y + y < MAZE_HEIGHT)
                    drawPixelARGB32(pos_x + x, pos_y + y, epd_bitmap_allArray[0][(pos_y + y) * MAZE_WIDTH + (pos_x + x)]);
                if (pos_x - x > 0 && pos_y - y > 0)
                    drawPixelARGB32(pos_x - x, pos_y - y, epd_bitmap_allArray[0][(pos_y - y) * MAZE_WIDTH + (pos_x - x)]);
                if (pos_x + x < MAZE_HEIGHT && pos_y - y > 0)
                    drawPixelARGB32(pos_x + x, pos_y - y, epd_bitmap_allArray[0][(pos_y - y) * MAZE_WIDTH + (pos_x + x)]);
                if (pos_x - x > 0 && pos_y + y < MAZE_HEIGHT)
                    drawPixelARGB32(pos_x - x, pos_y + y, epd_bitmap_allArray[0][(pos_y + y) * MAZE_WIDTH + (pos_x - x)]);
            }
}

// Clear field of view
void clear_fov(int pos_x, int pos_y, int rad)
{
    for (int y = 0; y < MAZE_HEIGHT; y++)
        for (int x = 0; x < MAZE_WIDTH; x++)
            if (x * x + y * y <= rad * rad)
            {
                drawPixelARGB32(pos_x + x, pos_y + y, BLACK);
                drawPixelARGB32(pos_x - x, pos_y - y, BLACK);
                drawPixelARGB32(pos_x + x, pos_y - y, BLACK);
                drawPixelARGB32(pos_x - x, pos_y + y, BLACK);
            }
}

// Increase field of view
void increase_fov()
{
    if (FOV_RADIUS < 200)
        FOV_RADIUS += 10;
}

// Decrease field of view
void decrease_fov()
{
    if (FOV_RADIUS > 30)
        FOV_RADIUS -= 10;
}

// Check if player has escaped or not
int win(int pos_x, int pos_y, int win_x, int win_y)
{
    if (pos_x == win_x && pos_y == win_y)
        return 1;
    else
        return 0;
}

int walkable(int pos_x, int pos_y)
{
    if (getPixelARGB32(pos_x, pos_y) == BLACK)
        return 0;
    else
        return 1;
}