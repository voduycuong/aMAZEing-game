#include "kernel.h"

char *commands[] = {"start", "level", "tutorial", "about", "exit"};
int index_pos = 350;
int level = 0;

void main()
{
	// Initialization
	uart_init();	// Set up serial console
	framebf_init(); // Initialize frame buffer

	// Set up
	clear_screen(); // Clear screen
	show_main_menu();

	while (1)
	{
		cli();
		// level = 4;
		// game(&level);
	}
}

void cli()
{
	static int cmd_index = 0; // Indexing commands

	if (cmd_index == 0)
		drawCharARGB32('>', 300, index_pos, 0x00ffffff, 2);

	char input = uart_getc();

	// 's' key is pressed
	if (input == 's')
	{
		cmd_index++;
		// Reset command index if exceeded
		if (cmd_index > 4)
			cmd_index = 0;

		drawCharARGB32('>', 300, index_pos, 0x00000000, 3);
		index_pos += 50;
		if (index_pos > 550)
			index_pos = 350;
		drawCharARGB32('>', 300, index_pos, 0x00ffffff, 2);
	}

	// 'w' key is pressed
	if (input == 'w')
	{
		cmd_index--;
		// Reset command index if exceeded
		if (cmd_index < 0)
			cmd_index = 4;

		drawCharARGB32('>', 300, index_pos, 0x00000000, 3);
		index_pos -= 50;
		if (index_pos < 350)
			index_pos = 550;
		drawCharARGB32('>', 300, index_pos, 0x00ffffff, 2);
	}

	// Return key is pressed
	if (input == '\n')
	{
		// Check buffer with available commands
		if (cmd_index == 0) // start command
		{
			while (level < 5)
				game(&level); // start with level 1

			clear_maze();
			drawStringARGB32(250, 400, "the end", 0x00ffffff, 3);
			wait_msec(2000000);
			show_about();
			wait_msec(2000000);
			clear_maze();
			show_main_menu();
		}

		else if (cmd_index == 1) // choose level command
		{
			choose_level();
			show_main_menu();
		}

		else if (cmd_index == 2) // help command
		{
			show_tutorial();
			show_main_menu();
		}

		else if (cmd_index == 3) // about command
		{
			show_about();
			show_main_menu();
		}
		else if (cmd_index == 4) // exit command
		{
			exit();
			show_main_menu();
		}
		cmd_index = 0;
		index_pos = 350;
	}
}

void clear_screen()
{
	uart_puts("\033[2J\033[f"); // Clear entire screen + Move cursor to upper left corner
}

void show_main_menu()
{
	int start_pos = 340;
	drawRectARGB32(0, 0, MAZE_WIDTH, MAZE_HEIGHT, 0x00000000, 1); // Clear screen

	drawStringARGB32(350, start_pos, "start", 0x00FFFFFF, 2);
	start_pos += 50;
	drawStringARGB32(350, start_pos, "choose level", 0x00FFB84C, 2);
	start_pos += 50;
	drawStringARGB32(350, start_pos, "how to play", 0x00F266AB, 2);
	start_pos += 50;
	drawStringARGB32(350, start_pos, "about", 0x00A459D1, 2);
	start_pos += 50;
	drawStringARGB32(350, start_pos, "exit", 0x002CD3E1, 2);
}