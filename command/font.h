// ----------------------------------- font.h -------------------------------------
#include "../kernel/framebf.h"
void drawCharARGB32(unsigned char ch, int x, int y, unsigned int attr, int zoom);
void drawStringARGB32(int x, int y, char *s, unsigned int attr, int zoom);
