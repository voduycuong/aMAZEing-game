#include "level.h"

int level_box_pos = 150;

void choose_level()
{
    while (1)
    {
        drawStringARGB32(MAZE_WIDTH / 2 - 150, 20, "CHOOSE LEVEL", 0x00DC143C, 3); // Show title
        drawStringARGB32(300, 600, "back", 0x00DC143C, 2);                         // back button

        static int level_index = 0; // Indexing commands

        show_box(level_index);

        char input = uart_getc();

        // 'a' key is pressed
        if (input == 'd')
        {
            level_index++;
            if (level_index > 5)
                level_index = 0;
            show_box(level_index);
        }

        // 'd' key is pressed
        if (input == 'a')
        {
            level_index--;
            if (level_index == 0)
                level_index = 5;
            show_box(level_index);
        }

        // Return key is pressed
        if (input == '\n')
        {
            if (level_index >= 0 && level_index < 5)
                game(&level_index);

            level_index = 0;
            break;
        }
    }
}

void show_box(int level)
{
    drawRectARGB32(0, 70, MAZE_WIDTH, MAZE_HEIGHT - 500, 0x00000000, 1); // Clear screen
    drawCharARGB32('>', 270, 610, 0x00000000, 2);

    int square_size = 100;

    int x1 = 148; // Left padding
    int y1 = 100; // Top padding
    int x2 = x1 + square_size;
    int y2 = y1 + square_size;

    switch (level)
    {
    // First line (first 3 levels)
    case 0:
        drawRectARGB32(x1, y1, x2, y2, 0x00ABCDAC, 0);
        break;
    case 1:
        drawRectARGB32(x1 + 150, y1, x2 + 150, y2, 0x00ABCDAC, 0);
        break;
    case 2:
        drawRectARGB32(x1 + 150 * 2, y1, x2 + 150 * 2, y2, 0x00ABCDAC, 0);
        break;

    // Second line (last 2 levels)
    case 3:
        drawRectARGB32(223, y2 + 50, 223 + 100, y2 + 150, 0x00ABCDAC, 0);
        break;
    case 4:
        drawRectARGB32(223 + 100 + 50, y2 + 50, 223 + 100 + 50 + 100, y2 + 150, 0x00ABCDAC, 0);
        break;

    // Back
    case 5:
        drawCharARGB32('>', 270, 410, 0x00ffffff, 2);
        break;

    default:
        break;
    }
}
