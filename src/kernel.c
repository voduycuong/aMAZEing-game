#include "kernel.h"

volatile int max_x = 700, start = 0, pic_num = 0, current_image = 0, x, y;

void main()
{
	uart_init();	// Set up serial console
	framebf_init(); // Initialize frame buffer

	// main_menu();
	// drawRectARGB32(100, 100, 400, 400, 0x00F4A896, 1);

	while (1)
	{
		for (int y = 0; y < 400; y++)
			for (int x = start; x < max_x; x++)
				drawPixelARGB32(x, y, epd_bitmap_allArray[pic_num][y * 700 + x - start]);

		char c = uart_getc();
		if (c == 'd')
			pic_num++;
	}
}
