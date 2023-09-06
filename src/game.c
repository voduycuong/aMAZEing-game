#include "game.h"

#define PLAYER_STEP 24
#define PLAYER_RADIUS 5

// Colors
#define BLUE 0x0068ADFF
#define BLACK 0x00000000
#define RED 0x00DC143C
#define WHITE 0x00FFFFFF
#define YELLOW 0xFFC000

int32_t FOV_RADIUS = 60;
int32_t star_flag = 1;

Entity guts;
Entity star;
// Entity bomb;
// Entity griffith;

void game()
{
    Position start_pos1 = {PLAYER_STEP / 2, MAZE_WIDTH / 2 - PLAYER_STEP};
    // Position start_pos2 = {200, 400};
    Position start_pos3 = {MAZE_WIDTH / 2 + PLAYER_STEP, MAZE_WIDTH / 2};
    Position end_pos = {MAZE_WIDTH - (PLAYER_STEP / 2), MAZE_WIDTH / 2 + PLAYER_STEP};

    guts.pos = start_pos1;
    star.pos = start_pos3;
    // star.pos = set_random_position();
    // griffith.pos = start_pos2;

    while (1)
    {
        uart_dec(rand(1, 10));
        if (win(guts.pos, end_pos))
        {
            uart_puts("ok you win! want some more?\n");
            char input = uart_getc();
            if (input == 'y')
            {
                FOV_RADIUS = 60;
                star_flag = 0;
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

            make_fov(guts.pos, FOV_RADIUS);
            drawCircleARGB32(guts.pos.x, guts.pos.y, PLAYER_RADIUS, BLUE);

            if (star_flag == 1)
                drawCircleARGB32(star.pos.x, star.pos.y, PLAYER_RADIUS, YELLOW);

            // make_fov(griffith.pos.x, griffith.pos.y, FOV_RADIUS);
            // drawCircleARGB32(griffith.pos.x, griffith.pos.y, PLAYER_RADIUS, RED);
            // uart_puts("\nColor: ");
            // uart_hex(getPixelARGB32(guts.pos.x, guts.pos.y));

            // uart_puts("\nWalkable: ");
            // uart_dec(interact(guts.pos.x, guts.pos.y - PLAYER_STEP));

            char input = uart_getc();
            handle_input(&guts.pos, input);
        } // handle_input(&griffith.pos.x, &griffith.pos.y, input);
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
        // if (interact(pos->x, pos->y - PLAYER_STEP) != 'w')
        {
            clear_fov(*pos, FOV_RADIUS);
            if (pos->y - PLAYER_STEP > 0)
            {
                pos->y -= PLAYER_STEP;
                if (star_flag == 0)
                {
                    FOV_RADIUS += 60;
                    star_flag = -1;
                    drawCircleARGB32(star.pos.x, star.pos.y, PLAYER_RADIUS, BLACK);
                }
            }
        }
        break;

    case 's': // Down
        if (interact(pos->x, pos->y + PLAYER_STEP) == 's')
            star_flag = 0;
        // if (interact(pos->x, pos->y + PLAYER_STEP) != 'w')
        {
            clear_fov(*pos, FOV_RADIUS);
            if (pos->y + PLAYER_STEP < MAZE_WIDTH)
            {
                pos->y += PLAYER_STEP;
                if (star_flag == 0)
                {
                    FOV_RADIUS += 60;
                    star_flag = -1;
                    drawCircleARGB32(star.pos.x, star.pos.y, PLAYER_RADIUS, BLACK);
                }
            }
        }
        break;

    case 'a': // Left
        if (interact(pos->x - PLAYER_STEP, pos->y) == 's')
            star_flag = 0;
        // if (interact(pos->x - PLAYER_STEP, pos->y) != 'w')
        {
            clear_fov(*pos, FOV_RADIUS);
            if (pos->x - PLAYER_STEP > 0)
            {
                pos->x -= PLAYER_STEP;
                if (star_flag == 0)
                {
                    FOV_RADIUS += 60;
                    star_flag = -1;
                    drawCircleARGB32(star.pos.x, star.pos.y, PLAYER_RADIUS, BLACK);
                }
            }
        }
        break;

    case 'd': // Right
        if (interact(pos->x + PLAYER_STEP, pos->y) == 's')
            star_flag = 0;
        // if (interact(pos->x + PLAYER_STEP, pos->y) != 'w')
        {
            clear_fov(*pos, FOV_RADIUS);
            if (pos->x + PLAYER_STEP < MAZE_HEIGHT)
            {
                pos->x += PLAYER_STEP;
                if (star_flag == 0)
                {
                    FOV_RADIUS += 60;
                    star_flag = -1;
                    drawCircleARGB32(star.pos.x, star.pos.y, PLAYER_RADIUS, BLACK);
                }
            }
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
                drawPixelARGB32(pos.x + x, pos.y + y, BLACK);
                drawPixelARGB32(pos.x - x, pos.y - y, BLACK);
                drawPixelARGB32(pos.x + x, pos.y - y, BLACK);
                drawPixelARGB32(pos.x - x, pos.y + y, BLACK);
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
int win(Position pos, Position win)
{
    if (pos.x == win.x && pos.y == win.y)
        return 1;
    else
        return 0;
}

char interact(int pos_x, int pos_y)
{
    if (getPixelARGB32(pos_x, pos_y) == BLACK)
        return 'w';
    else if (getPixelARGB32(pos_x, pos_y) == YELLOW)
        return 's';
    else
        return 'n';
}

Position set_random_position()
{
    Position temp_pos;
    do
    {
        temp_pos.x = rand(1, 10) * PLAYER_STEP / 2;
        temp_pos.y = rand(1, 10) * PLAYER_STEP / 2;
    } while (getPixelARGB32(temp_pos.x, temp_pos.y) != BLACK);

    return temp_pos;
}
