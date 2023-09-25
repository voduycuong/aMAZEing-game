// ----------------------------------- framebf.h -------------------------------------
#include "mbox.h"
extern unsigned int width, height, pitch;
void drawPixelARGB32(int x, int y, unsigned int attr);
void drawRectARGB32(int x1, int y1, int x2, int y2, unsigned int attr, int fill);
void drawALineARGB32 (int x1, int y, int x2, unsigned int attr);
void drawACircleARGB32 (int center_x, int center_y, int radius, unsigned int attr);
void display_image(int image_num);
void frame_buffer_setup(int phy_w, int phy_h,int vir_w, int vir_h);