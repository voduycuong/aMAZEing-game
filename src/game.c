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
#define FOV_RADIUS 60

// Colors
#define BLUE 0x0068ADFF
#define BLACK 0x00000000
#define RED 0x00DC143C

Player guts;
// Player griffith;

void game()
{
    Position start_pos1 = {12, MAZE_WIDTH / 2};
    // Position start_pos2 = {200, 400};
    guts.pos = start_pos1;
    // griffith.pos = start_pos2;

    while (1)
    {
        make_fov(guts.pos.x, guts.pos.y, FOV_RADIUS);

        // // Full view
        // for (int y = 0; y <= MAZE_HEIGHT; y++)
        //     for (int x = 0; x <= MAZE_WIDTH; x++)
        //         drawPixelARGB32(guts.pos.x + x, guts.pos.y + y, epd_bitmap_allArray[0][(guts.pos.y + y) * MAZE_WIDTH + (guts.pos.x + x)]);

        drawCircleARGB32(guts.pos.x, guts.pos.y, PLAYER_RADIUS, BLUE);
        // make_fov(griffith.pos.x, griffith.pos.y, FOV_RADIUS);
        // drawCircleARGB32(griffith.pos.x, griffith.pos.y, PLAYER_RADIUS, RED);

        char input = uart_getc();
        handle_input(&guts.pos.x, &guts.pos.y, input);
        // handle_input(&griffith.pos.x, &griffith.pos.y, input);
    }
}

// Function for input directions
void handle_input(int *pos_x, int *pos_y, int input)
{
    switch (input)
    {
    case 'w': // Up
        clear_fov(*pos_x, *pos_y, FOV_RADIUS);
        *pos_y -= PLAYER_STEP;
        break;

    case 's': // Down
        clear_fov(*pos_x, *pos_y, FOV_RADIUS);
        *pos_y += PLAYER_STEP;
        break;

    case 'a': // Left
        clear_fov(*pos_x, *pos_y, FOV_RADIUS);
        *pos_x -= PLAYER_STEP;
        break;

    case 'd': // Right
        clear_fov(*pos_x, *pos_y, FOV_RADIUS);
        *pos_x += PLAYER_STEP;
        break;

    default:
        break;
    }
}

// Draw guts field of view
void make_fov(int pos_x, int pos_y, int rad)
{
    for (int y = 0; y < MAZE_HEIGHT; y++)
        for (int x = 0; x < MAZE_WIDTH; x++)
            if (x * x + y * y <= rad * rad)
            {
                drawPixelARGB32(pos_x + x, pos_y + y, epd_bitmap_allArray[0][(pos_y + y) * MAZE_WIDTH + (pos_x + x)]);
                drawPixelARGB32(pos_x - x, pos_y - y, epd_bitmap_allArray[0][(pos_y - y) * MAZE_WIDTH + (pos_x - x)]);
                drawPixelARGB32(pos_x + x, pos_y - y, epd_bitmap_allArray[0][(pos_y - y) * MAZE_WIDTH + (pos_x + x)]);
                drawPixelARGB32(pos_x - x, pos_y + y, epd_bitmap_allArray[0][(pos_y + y) * MAZE_WIDTH + (pos_x - x)]);
            }
}

// Clear guts field of view
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
