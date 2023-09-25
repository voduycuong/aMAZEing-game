#include "../header/string.h"
#include "../uart/uart0.h"
#include "../kernel/mbox.h"
#include "../kernel/framebf.h"
#include "play_vid.h"
#include "../graphsrc/pic.h"
void clear ();
char* autocomp(char* input, int time);
void display_slideshow ();
void display_largeimg ();
void display_image_section(unsigned int y_offset);
void display_video();