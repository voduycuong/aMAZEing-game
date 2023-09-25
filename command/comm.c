#include "comm.h"
#include "../graphsrc/image.h"

void clear ()
{
    uart_puts("\033[H");    // clear terminal
    uart_puts("\033[J");    // return cursor to home position
}

char* autocomp(char* input, int time) 
{
    char * str = input;
    static int index_found = 0;
    static char * tmp;
    if (time == 1)          // condition to keep origin input
    {
        strcpy(tmp, str);
        index_found = 0;
    }
    else
    {
        strcpy(str,tmp);
    }
    char* commands[5] = {"display large image", "clear", "display image slideshow", "display video"};  // declare command list
    int i, j, k, len = strlen(str);
    for (i = index_found; i < 5; i++)   // loop for matching check
    {
        int match = 1;
        for (j = 0; j < len; j++)
        {
           if (str[j] != commands[i][j]) 
            {
                match = 0;
            }
        }
        
        if (match) // check for match
        {
            index_found = i;                                        // upadte matching index
            for (k = len; commands[index_found][k] != '\0'; k++)    // write the matched command to the buffer
            {
                input[k] = commands[index_found][k];
            }
            input[k] = '\0';
            
            index_found++;                                          // increase matched index for next run to read
            
            if (index_found == 5)                                   // condition for matched index exceed the list to reset to the start
            {
                index_found = 0;
            }
            break;
        }    
        else if (i == 4)                                            // condition for reset matched index when reach last word of command list
        {
            index_found = 0;
            break;
        }
    }
    return input;
}

void display_slideshow ()
{
    // declare variables and resolution of the framebuffer
    int max_x = 545, max_y = 345, start = 0, pic_num = 0, current_image = 0, x, y;
    // Initialize frame buffer (physical: 545x345, virtual:545*3x345*3)
    frame_buffer_setup(545,345,545*7,345*7);
    // Display instruction
    uart_puts("Displaying 3 images slideshow\n");
    uart_puts("Press <a> or <d> to see next/previous picture \n");
    uart_puts("Press <e> to exit and enter new command \n");
    // Print out 3 pictures
    for (y = 0; y < max_y; y++) 
    {
        for (x = start; x < max_x; x++) {
            drawPixelARGB32(x, y, epd_bitmap_allArray[pic_num][y * 545 + x - start]);
        }
            if ((y == max_y-1) && (max_x < 545*3)) {
                max_x += 545;
                start += 545;
                pic_num++;
                y = 0;
        }
    }
    // Loop for pictures slideshow
    while (1)
    {
        // Get key pressed from user
        char c = uart_getc();
        // check if <d> key
        if (c == 'd') 
        {
            // return to first image when at the last image
            if (current_image == 2)
            {
                current_image = 0;
            }
            // go to next image
            else
            {
                current_image ++;
            }
            // Display next image by sending offset message to the mailbox
            display_image(current_image);
        }
        // check if <a> key
        if (c == 'a') 
        {
            // return to last image when at the first image
            if (current_image == 0)
            {
                current_image = 2;
            }
            // go to previous picture
            else
            {
                current_image --;
            }
            // Display next image by sending offset message to the mailbox
            display_image(current_image);
        }
        // Exit the command when press <e>
        if (c =='e')
        {
            break;
        }
    }
    
}

void display_image_section(unsigned int y_offset) {
    for (unsigned int y = 0; y < height; ++y) {  // height and width are your framebuffer dimensions
        for (unsigned int x = 0; x < width; ++x) {
            if ((y + y_offset) < 2048 && x < 1024) {
                unsigned int pixel = epd_bitmap_HAALAND[(y + y_offset) * 1024 + x];
                drawPixelARGB32(x, y, pixel);
            }
        }
    }
}

void display_largeimg ()
{
    // Initialize frame buffer
    frame_buffer_setup(1024,768,1024,768);
    // Initialize scroll position
    unsigned int y_offset = 0;
    uart_puts("Displaying large images \n");
    uart_puts("Press <w> or <s> to see lower/higher section of the picture \n");
    uart_puts("Press <e> to exit and enter new command \n");
    // Loop to handle scrolling
    while (1) 
    {
        // Read keyboard input
        char key = uart_getc();
        

        // Update scroll position based on key press
        if (key == 'w') 
        {
            if (y_offset > 0) 
            {
                y_offset--;
            } 
        }
        else if (key == 's') 
        {
            if (y_offset < (2048 - height)) 
            {
                y_offset++;
            }
        }
        // exit
        else if (key == 'e') 
        {
            break;
        }

        // Display the current section of the image
        display_image_section(y_offset);
    }
}

void display_video(){
    frame_buffer_setup(800,600,800,600);
    play_video();
}
