#include "kernel.h"

char *commands[] = {"start", "level", "tutorial", "about", "exit"};

void main()
{
	// Initialization
	uart_init();	// Set up serial console
	framebf_init(); // Initialize frame buffer

	// Set up
	clear_screen(); // Clear screen
	// show_welcome_screen(); // Show welcome screen
	// main_menu();

	while (1)
	{
		cli();
		// game();
	}
}

void cli()
{
	drawStringARGB32(0, 0, "abcdefghijklmnopqrstuvwxyz", 0x00FFFFFF, 3);
	drawStringARGB32(0, 50, "ABCDEFGHIJKLMNOPQRSTUVWXYZ", 0x00FFFFFF, 3);
	drawStringARGB32(0, 100, "abcdefghijklmnopqrstuvwxyz", 0x00AA0000, 3);
	drawStringARGB32(0, 150, "ABCDEFGHIJKLMNOPQRSTUVWXYZ", 0x00FFFF00, 3);

	// drawCharARGB32('a', 100, 100, 0x00ffffff);

	static int cmd_index = 0; // Indexing commands

	char input = uart_getc();

	// 's' key is pressed
	if (input == 's')
	{
		cmd_index++;
		if (cmd_index > 4)
			cmd_index = 0;
	}

	// 'w' key is pressed
	if (input == 'w')
	{
		cmd_index--;
		// Reset command index if exceeded
		if (cmd_index < 0)
			cmd_index = 4;
	}

	// Return key is pressed
	else if (input == '\n')
	{
		// Check buffer with available commands
		if (cmd_index == 0) // start command
			game();

		else if (cmd_index == 1) // choose level command
			choose_level();

		else if (cmd_index == 2) // help command
			show_tutorial();

		else if (cmd_index == 3) // about command
			show_about();

		else if (cmd_index == 4) // exit command
			exit();

		cmd_index = 0;
	}
}

// Show welcome screen when OS boot up
void show_welcome_screen()
{
	show_about(); // Welcome screen
}

void clear_screen()
{
	uart_puts("\033[2J\033[f"); // Clear entire screen + Move cursor to upper left corner
}