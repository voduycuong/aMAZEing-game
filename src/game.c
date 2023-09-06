#include "game.h"

typedef struct
{
    int y;
    int x;
} Position;

typedef struct
{
    Position pos;
} Player;

#define PLAYER_STEP 10

#define RADIUS 10
#define VISION_RADIUS 50

#define BLUE_COLOR 0x0068adff
#define BLACK_COLOR 0x00000000

Player player;

int max_x = 700, max_y = 400;
int starting = 0;

void game()
{
    Position start_pos = {20, 20};
    player.pos = start_pos;

    drawCircleARGB32(player.pos.x, player.pos.y, RADIUS, BLUE_COLOR);

    while (1)
    {
        for (int y = 0; y < max_y; y++)
            for (int x = starting; x < max_x; x++)
                if (x * x + y * y < (RADIUS + VISION_RADIUS) * (RADIUS + VISION_RADIUS))
                    drawPixelARGB32(player.pos.x + x, player.pos.y + y,
                                    epd_bitmap_allArray[0][(player.pos.y + y) * max_x + (player.pos.x + x) - starting]);

        char input = uart_getc();
        handle_input(input);

        drawCircleARGB32(player.pos.x, player.pos.y, RADIUS, BLUE_COLOR);
    }
}

void handle_input(int input)
{
    switch (input)
    {
    case 'w': // Up
        player.pos.y -= PLAYER_STEP;
        break;

    case 's': // Down
        player.pos.y += PLAYER_STEP;
        break;

    case 'a': // Left
        player.pos.x -= PLAYER_STEP;
        break;

    case 'd': // Right
        player.pos.x += PLAYER_STEP;
        break;

    default:
        break;
    }
}