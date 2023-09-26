#include "./include/game.h"

typedef struct {
    int x;
    int y;
    int radius;
} FOV;

FOV character1_fov;
FOV character2_fov;


void game_loop() {
    // Store old FOV positions
    int old_fov1_x = character1_fov.x;
    int old_fov1_y = character1_fov.y;
    int old_fov2_x = character2_fov.x;
    int old_fov2_y = character2_fov.y;

    // Update game state and FOVs
    update_game_state();

    // Update pixels for each character's FOV
    update_pixels_for_fov(&character1_fov, old_fov1_x, old_fov1_y);
    update_pixels_for_fov(&character2_fov, old_fov2_x, old_fov2_y);
}

void update_pixels_for_fov(FOV* fov, int old_x, int old_y) {
    int radius_squared = fov->radius * fov->radius;
    for (int y = -fov->radius; y <= fov->radius; y++) {
        for (int x = -fov->radius; x <= fov->radius; x++) {
            int dist_squared = x*x + y*y;
            if (dist_squared <= radius_squared) {
                // This pixel is inside the FOV
                int world_x = fov->x + x;
                int world_y = fov->y + y;
                // Check if this pixel was inside the old FOV
                int dx = world_x - old_x;
                int dy = world_y - old_y;
                if (dx*dx + dy*dy > radius_squared) {
                    // This pixel was outside the old FOV, so redraw it
                    draw_pixel(world_x, world_y, color);
                }
            } else {
                // This pixel is outside the FOV, so make it black
                draw_pixel(fov->x + x, fov->y + y, BLACK);
            }
        }
    }
}
