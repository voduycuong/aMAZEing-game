#include "./include/game.h"

typedef struct {
    int x;
    int y;
    int radius;
} FOV;

typedef struct {
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
} Color;

FOV character1_fov;
FOV character2_fov;

Color get_pixel_color(int x, int y) {
    // Check if the pixel is within the game world
    if (x >= 0 && x < MAZE_WIDTH && y >= 0 && y < MAZE_HEIGHT) {
        // Get the game object at this pixel
        GameObject* obj = get_game_object_at(x, y);

        // Determine the color based on the game object
        if (obj == NULL) {
            // No game object at this pixel, so it's probably empty space
            return (Color){0, 0, 0, 255}; // Black
        } else if (obj->type == GAME_OBJECT_TYPE_PLAYER) {
            // This is a player character
            return (Color){255, 255, 255, 255}; // White
        } else if (obj->type == GAME_OBJECT_TYPE_ENEMY) {
            // This is an enemy character
            return (Color){255, 0, 0, 255}; // Red
        } else {
            // Some other type of game object
            return obj->color;
        }
    } else {
        // The pixel is outside the game world
        return (Color){0, 0, 0, 255}; // Black
    }
}

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
                    Color color = get_pixel_color(world_x, world_y);
                    draw_pixel(world_x, world_y, color);
                }
            } else {
                // This pixel is outside the FOV, so make it black
                draw_pixel(fov->x + x, fov->y + y, BLACK);
            }
        }
    }
}
