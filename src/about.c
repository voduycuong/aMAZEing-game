#include "about.h"

void show_about()
{
    int top_padding = 200;
    int left_padding = 240;
    int size = 2;

    drawRectARGB32(0, 0, MAZE_WIDTH, MAZE_HEIGHT, 0x00000000, 1); // Clear screen

    drawStringARGB32(70, 100, "- A PROJECT BY GROUP 22 - ", 0x00ffffff, size + 2);

    drawStringARGB32(left_padding, top_padding, "Tran Minh Anh - S3931980", 0x00ffffff, size);
    top_padding += 50;
    drawStringARGB32(left_padding, top_padding, "Vo Duy Cuong - S3941544", 0x00FFB84C, size);
    top_padding += 50;
    drawStringARGB32(left_padding, top_padding, "Nguyen Tuan Dung - S3927511", 0x00F266AB, size);
    top_padding += 50;
    drawStringARGB32(left_padding, top_padding, "Dam Ta Minh Nhat - S3926946", 0x00A459D1, size);
    top_padding += 50;
    drawStringARGB32(left_padding, top_padding, "Le Tran Trong Phuoc - S3927230", 0x002CD3E1, size);

    uart_getc();
}
