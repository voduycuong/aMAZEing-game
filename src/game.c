#include "game.h"

#define PLAYER_STEP MAZE_HEIGHT / 19 // Character step
#define ENTITY_RADIUS 20             // Hitbox dimension

// Animation duration
#define FRAME_CHANGE_INTERVAL 12
#define DELAY_TIME 20000

#define BLACK 0x00000000
int path_color = 0x00FFFFFF; // Default path color

// Default items' flag
int star_flag = 1;
int bomb_flag = 2;
int key_flag = 3;
int trap_flag = 4;
int detrap_flag = 5;

// Initialize entities
Character guts;
Character griffith;
Item star;
Item bomb;
Item key;
Item trap;
Item detrap;

// Initialize animation
AnimationState guts_animation_state = GUTS_FRONT_IDLE;
AnimationState griffith_animation_state = GRIFFITH_FRONT_IDLE;
AnimationState currentFrame;

void game(int *level)
{
    // Default position of main character and exit gate
    Position start_pos1 = {PLAYER_STEP / 2, PLAYER_STEP * 10 - PLAYER_STEP / 2};                 // No modify
    Position end_pos = {PLAYER_STEP * 19 - PLAYER_STEP / 2, PLAYER_STEP * 10 - PLAYER_STEP / 2}; // No modify

    // Position of other entities based on maze level
    Position start_pos2, star_pos, bomb_pos, key_pos, trap_pos, detrap_pos;
    int charracter_fov;
    set_maze_entity_position(*level, &path_color, &start_pos2, &star_pos, &bomb_pos, &key_pos, &trap_pos, &detrap_pos, &charracter_fov);

    // Initialize hitboxes
    Box guts_box = {start_pos1, ENTITY_RADIUS, ENTITY_RADIUS};
    Box griffith_box = {start_pos2, ENTITY_RADIUS, ENTITY_RADIUS};
    Box star_box = {star_pos, ENTITY_RADIUS, ENTITY_RADIUS};
    Box bomb_box = {bomb_pos, ENTITY_RADIUS, ENTITY_RADIUS};
    Box key_box = {key_pos, ENTITY_RADIUS, ENTITY_RADIUS};
    Box trap_box = {trap_pos, ENTITY_RADIUS, ENTITY_RADIUS};
    Box detrap_box = {detrap_pos, ENTITY_RADIUS, ENTITY_RADIUS};

    // Initialize characters
    guts.box = guts_box;
    guts.FOV_radius = charracter_fov;
    guts.currentFrame = GUTS_FRONT_IDLE;
    griffith.box = griffith_box;
    griffith.FOV_radius = charracter_fov;
    griffith.currentFrame = GRIFFITH_FRONT_IDLE;

    // Initialize items
    star.box = star_box;
    star.icon_frame = STAR_FRAME;
    bomb.box = bomb_box;
    bomb.icon_frame = BOMB_FRAME;
    key.box = key_box;
    key.icon_frame = KEY_FRAME;
    trap.box = trap_box;
    trap.icon_frame = TRAP_FRAME;
    detrap.box = detrap_box;
    detrap.icon_frame = LEVER_FRAME;

    // Clear maze
    clear_maze();
    show_level_title(*level);
    wait_msec(1000000);
    clear_maze();

    // Game begins
    while (1)
    {
        // Check for win
        if (reach_exit_gate(guts.box.pos, end_pos)) // Guts reach exit gate and key is retrieved
        {
            clear_maze();
            drawStringARGB32(200, 400, "You escaped the maze!", 0x00ffffff, 3);
            wait_msec(1000000);
            if (*level < 5) // If not reach last level, increase level
            {
                // Next level
                int temp_level = *level;
                temp_level++;
                *level = temp_level;
                reset_flag(); // Set default value for flags
                break;
            }
        }

        else
        {
            // Show FOV for 2 characters
            make_fov(guts.box.pos, guts.FOV_radius, *level);
            make_fov(griffith.box.pos, griffith.FOV_radius, *level);

            // Draw 2 characters
            draw_character_frame(guts.box.pos, guts.currentFrame);
            draw_character_frame(griffith.box.pos, griffith.currentFrame);

            // Draw exit gate
            if (key_flag && (guts.FOV_radius > distance(guts.box.pos.x, end_pos.x, guts.box.pos.y, end_pos.y) ||
                             griffith.FOV_radius > distance(griffith.box.pos.x, end_pos.x, griffith.box.pos.y, end_pos.y)))
                drawRectARGB32(MAZE_WIDTH - 35, MAZE_HEIGHT / 2 - 31, MAZE_WIDTH - 10, MAZE_HEIGHT / 2 + 24, 0x00FF0000, 1);

            if (detrap_flag == 0) // Check for detrap, if detrapped, deactivate trap
            {
                trap.box.pos.x = -1;
                trap.box.pos.y = -1;
            }

            if (trap_flag == 0) // Check for trap, if trapped, reset level
            {
                clear_maze();
                drawStringARGB32(200, 400, "You wake up!", 0x00FFB84C, 3);
                wait_msec(2000000);
                reset_flag();
                break;
            }

            // If items are inside characters' FOV, they're shown
            if (star_flag == 1 && (in_FOV(guts, star) || in_FOV(griffith, star)))
                draw_icon_frame(star.box.pos, star.icon_frame);

            if (bomb_flag == 2 && (in_FOV(guts, bomb) || in_FOV(griffith, bomb)))
                draw_icon_frame(bomb.box.pos, bomb.icon_frame);

            if (key_flag == 3 && (in_FOV(guts, key) || in_FOV(griffith, key)))
                draw_icon_frame(key.box.pos, key.icon_frame);

            if ((trap_flag == 4 && detrap_flag == 5) && (in_FOV(guts, trap) || in_FOV(griffith, trap)))
                draw_icon_frame(trap.box.pos, trap.icon_frame);

            if (detrap_flag == 5 && (in_FOV(guts, detrap) || in_FOV(griffith, detrap)))
                draw_icon_frame(detrap.box.pos, detrap.icon_frame);

            // Get direction from player
            char input = uart_getc();

            // Handle input for both characters
            handle_input(&guts, input, *level);
            handle_input(&griffith, input, *level);
        }
    }
}

// Function for input directions
void handle_input(Character *character, int input, int level)
{
    // Create a box for next step
    Box next_step;
    next_step.width = ENTITY_RADIUS;
    next_step.height = ENTITY_RADIUS;

    switch (input)
    {
    case 'w': // Up

        // Calculating next step
        next_step.pos.x = character->box.pos.x;
        next_step.pos.y = character->box.pos.y - PLAYER_STEP;

        if (walkable(next_step))                        // Check if next step is not a wall (either path or items)
            if (character->box.pos.y - PLAYER_STEP > 0) // Character is not outside the maze
                // Draw animation of character
                handle_character_movement(character, input, level);

        break;

    case 's': // Down
        next_step.pos.x = character->box.pos.x;
        next_step.pos.y = character->box.pos.y + PLAYER_STEP;
        if (walkable(next_step))
            if (character->box.pos.y + PLAYER_STEP < MAZE_WIDTH)
                handle_character_movement(character, input, level);
        break;

    case 'a': // Left
        next_step.pos.x = character->box.pos.x - PLAYER_STEP;
        next_step.pos.y = character->box.pos.y;
        if (walkable(next_step))
            if (character->box.pos.x - PLAYER_STEP > 0)
                handle_character_movement(character, input, level);
        break;

    case 'd': // Right
        next_step.pos.x = character->box.pos.x + PLAYER_STEP;
        next_step.pos.y = character->box.pos.y;
        if (walkable(next_step))
            if (character->box.pos.x + PLAYER_STEP < MAZE_HEIGHT)
                handle_character_movement(character, input, level);
        break;

        // Cheat
    case 'o':
        increase_fov(character->box.pos, &character->FOV_radius);
        break;
    case 'p':
        decrease_fov(character->box.pos, &character->FOV_radius);
        break;

    default:
        break;
    }
    check_entity(character, &star, &star_flag);
    check_entity(character, &bomb, &bomb_flag);
    check_entity(character, &key, &key_flag);
    check_entity(character, &trap, &trap_flag);
    check_entity(character, &detrap, &detrap_flag);
}

/*
 * Draw field of view
 */
void make_fov(Position pos, int radius, int level)
{
    for (int y = 0; y < MAZE_HEIGHT; y++)
        for (int x = 0; x < MAZE_WIDTH; x++)
            if (x * x + y * y < radius * radius)
            {
                // Draw 4 quarters
                if (pos.x + x < MAZE_WIDTH && pos.y + y < MAZE_HEIGHT)
                    drawPixelARGB32(pos.x + x, pos.y + y, epd_bitmap_allArray[level][(pos.y + y) * MAZE_WIDTH + (pos.x + x)]);
                if (pos.x - x > 0 && pos.y - y > 0)
                    drawPixelARGB32(pos.x - x, pos.y - y, epd_bitmap_allArray[level][(pos.y - y) * MAZE_WIDTH + (pos.x - x)]);
                if (pos.x + x < MAZE_HEIGHT && pos.y - y > 0)
                    drawPixelARGB32(pos.x + x, pos.y - y, epd_bitmap_allArray[level][(pos.y - y) * MAZE_WIDTH + (pos.x + x)]);
                if (pos.x - x > 0 && pos.y + y < MAZE_HEIGHT)
                    drawPixelARGB32(pos.x - x, pos.y + y, epd_bitmap_allArray[level][(pos.y + y) * MAZE_WIDTH + (pos.x - x)]);
            }
}

/*
 * Clear field of view
 */
void clear_fov(Position pos, int radius)
{
    for (int y = 0; y < MAZE_HEIGHT; y++)
        for (int x = 0; x < MAZE_WIDTH; x++)
            if (x * x + y * y <= radius * radius *2)
            {
                drawPixelARGB32(pos.x + x, pos.y + y, BLACK);
                drawPixelARGB32(pos.x - x, pos.y - y, BLACK);
                drawPixelARGB32(pos.x + x, pos.y - y, BLACK);
                drawPixelARGB32(pos.x - x, pos.y + y, BLACK);
            }
}

/*
 * Increase field of view
 */
void increase_fov(Position pos, int *radius)
{
    drawCircleARGB32(pos.x, pos.y, *radius, BLACK); // Clear previous FOV
    if (*radius < 200)
        *radius += 40;
}

/*
 * Decrease field of view
 */
void decrease_fov(Position pos, int *radius)
{
    drawCircleARGB32(pos.x, pos.y, *radius, BLACK); // Clear previous FOV
    if (*radius > PLAYER_STEP * 2)
        *radius -= 30;
}

/*
 * Check if player has escaped or not
 */
int reach_exit_gate(Position pos, Position win)
{
    if (pos.x == win.x && pos.y == win.y)
        return 1;
    else
        return 0;
}

/*
 * Check for wall
 */
int walkable(Box character)
{
    if (getPixelARGB32(character.pos.x, character.pos.y) == path_color ||
        detect_collision(character, star.box) ||
        detect_collision(character, bomb.box) ||
        detect_collision(character, key.box) ||
        detect_collision(character, trap.box) ||
        detect_collision(character, detrap.box))
        return 1;

    return 0;
}

/*
 * Detect collision of 2 hitboxes
 */
int detect_collision(Box a, Box b)
{
    if (a.pos.x < b.pos.x + b.width && a.pos.x + a.width > b.pos.x &&
        a.pos.y < b.pos.y + b.height && a.pos.y + a.height > b.pos.y)

        return 1; // Collision detected
    return 0;     // No collision
}

/*
 * Clear screen
 */
void clear_maze()
{
    for (int y = 0; y <= MAZE_HEIGHT; y++)
        for (int x = 0; x <= MAZE_WIDTH; x++)
            drawPixelARGB32(x, y, BLACK);
}

/*
 * Check for interaction of characters and items
 * If there is collision, do action and set flag off
 */
void check_entity(Character *character, Item *item, int *flag)
{
    int temp = *flag;

    if (detect_collision(character->box, item->box))
    {
        if (temp == 1) // Star
        {
            increase_fov(character->box.pos, &character->FOV_radius);
            temp = 0;
        }

        else if (temp == 2) // Bomb
        {
            decrease_fov(character->box.pos, &character->FOV_radius);
            temp = 0;
        }

        else if (temp == 3 || temp == 4 || temp == 5) // Key or Trap or Detrap
            temp = 0;
    }

    *flag = temp;
}

/*
 * Set default value for characters and flags
 */
void reset_flag()
{
    // Reset flags
    star_flag = 1;
    bomb_flag = 2;
    key_flag = 3;
    trap_flag = 4;
    detrap_flag = 5;
}

/*
 * Set value for entities based on level
 */
void set_maze_entity_position(int level, int *path,
                              Position *start2, Position *star, Position *bomb, Position *key, Position *trap, Position *detrap,
                              int *fov)
{
    switch (level)
    {
    case 0:
        // level 1
        *path = 0x00d2fee1;

        start2->x = PLAYER_STEP * 3 - PLAYER_STEP / 2;
        start2->y = PLAYER_STEP * 18 - PLAYER_STEP / 2;

        star->x = PLAYER_STEP * 4 - PLAYER_STEP / 2,
        star->y = PLAYER_STEP * 8 - PLAYER_STEP / 2;

        bomb->x = PLAYER_STEP * 7 - PLAYER_STEP / 2;
        bomb->y = PLAYER_STEP * 10 - PLAYER_STEP / 2;

        key->x = PLAYER_STEP * 16 - PLAYER_STEP / 2;
        key->y = PLAYER_STEP * 12 - PLAYER_STEP / 2;

        trap->x = PLAYER_STEP * 3 - PLAYER_STEP / 2;
        trap->y = PLAYER_STEP * 6 - PLAYER_STEP / 2;

        detrap->x = PLAYER_STEP * 18 - PLAYER_STEP / 2;
        detrap->y = PLAYER_STEP * 18 - PLAYER_STEP / 2;

        *fov = PLAYER_STEP * 3;

        break;

    case 1:
        // Level 2
        *path = 0x00fef2dc;

        start2->x = PLAYER_STEP * 2 - PLAYER_STEP / 2;
        start2->y = PLAYER_STEP * 6 - PLAYER_STEP / 2;

        star->x = PLAYER_STEP * 8 - PLAYER_STEP / 2,
        star->y = PLAYER_STEP * 17 - PLAYER_STEP / 2;

        bomb->x = PLAYER_STEP * 2 - PLAYER_STEP / 2;
        bomb->y = PLAYER_STEP * 15 - PLAYER_STEP / 2;

        key->x = PLAYER_STEP * 18 - PLAYER_STEP / 2;
        key->y = PLAYER_STEP * 2 - PLAYER_STEP / 2;

        trap->x = PLAYER_STEP * 18 - PLAYER_STEP / 2;
        trap->y = PLAYER_STEP * 5 - PLAYER_STEP / 2;

        detrap->x = PLAYER_STEP * 5 - PLAYER_STEP / 2;
        detrap->y = PLAYER_STEP * 18 - PLAYER_STEP / 2;

        *fov = PLAYER_STEP * 3;

        break;

    case 2:
        // Level 3
        *path = 0x00ffe4f9;

        start2->x = PLAYER_STEP * 14 - PLAYER_STEP / 2;
        start2->y = PLAYER_STEP * 8 - PLAYER_STEP / 2;

        star->x = PLAYER_STEP * 4 - PLAYER_STEP / 2,
        star->y = PLAYER_STEP * 12 - PLAYER_STEP / 2;

        bomb->x = PLAYER_STEP * 14 - PLAYER_STEP / 2;
        bomb->y = PLAYER_STEP * 15 - PLAYER_STEP / 2;

        key->x = PLAYER_STEP * 14 - PLAYER_STEP / 2;
        key->y = PLAYER_STEP * 18 - PLAYER_STEP / 2;

        trap->x = PLAYER_STEP * 18 - PLAYER_STEP / 2;
        trap->y = PLAYER_STEP * 10 - PLAYER_STEP / 2;

        detrap->x = PLAYER_STEP * 2 - PLAYER_STEP / 2;
        detrap->y = PLAYER_STEP * 6 - PLAYER_STEP / 2;

        *fov = PLAYER_STEP * 2.5;

        break;

    case 3:
        // Level 4
        *path = 0x00f1d7ff;

        start2->x = PLAYER_STEP * 8 - PLAYER_STEP / 2;
        start2->y = PLAYER_STEP * 6 - PLAYER_STEP / 2;

        star->x = PLAYER_STEP * 10 - PLAYER_STEP / 2,
        star->y = PLAYER_STEP * 6 - PLAYER_STEP / 2;

        bomb->x = PLAYER_STEP * 18 - PLAYER_STEP / 2;
        bomb->y = PLAYER_STEP * 15 - PLAYER_STEP / 2;

        key->x = PLAYER_STEP * 2 - PLAYER_STEP / 2;
        key->y = PLAYER_STEP * 15 - PLAYER_STEP / 2;

        trap->x = PLAYER_STEP * 6 - PLAYER_STEP / 2;
        trap->y = PLAYER_STEP * 14 - PLAYER_STEP / 2;

        detrap->x = PLAYER_STEP * 16 - PLAYER_STEP / 2;
        detrap->y = PLAYER_STEP * 6 - PLAYER_STEP / 2;

        *fov = PLAYER_STEP * 2;

        break;

    case 4:
        // Level 5
        *path = 0x00d4faff;

        start2->x = PLAYER_STEP * 3 - PLAYER_STEP / 2;
        start2->y = PLAYER_STEP * 2 - PLAYER_STEP / 2;

        star->x = PLAYER_STEP * 16 - PLAYER_STEP / 2,
        star->y = PLAYER_STEP * 8 - PLAYER_STEP / 2;

        bomb->x = PLAYER_STEP * 13 - PLAYER_STEP / 2;
        bomb->y = PLAYER_STEP * 10 - PLAYER_STEP / 2;

        key->x = PLAYER_STEP * 18 - PLAYER_STEP / 2;
        key->y = PLAYER_STEP * 8 - PLAYER_STEP / 2;

        trap->x = PLAYER_STEP * 4 - PLAYER_STEP / 2;
        trap->y = PLAYER_STEP * 8 - PLAYER_STEP / 2;

        detrap->x = PLAYER_STEP * 8 - PLAYER_STEP / 2;
        detrap->y = PLAYER_STEP * 17 - PLAYER_STEP / 2;

        *fov = PLAYER_STEP + 1;

        break;

    default:
        break;
    }
}

/*
 * Draw animation for characters when moving
 */
void draw_character_frame(Position pos, AnimationState state)
{
    uint32_t *currentFrame;
    switch (state)
    {
    case GUTS_FRONT_IDLE:
        currentFrame = guts_front_idle;
        break;
    case GUTS_FRONT_WALK1:
        currentFrame = guts_front_walk1;
        break;
    case GUTS_FRONT_WALK2:
        currentFrame = guts_front_walk2;
        break;
    case GUTS_BACK_IDLE:
        currentFrame = guts_back_idle;
        break;
    case GUTS_BACK_WALK1:
        currentFrame = guts_back_walk1;
        break;
    case GUTS_BACK_WALK2:
        currentFrame = guts_back_walk2;
        break;
    case GUTS_RIGHT_IDLE:
        currentFrame = guts_right_idle;
        break;
    case GUTS_RIGHT_WALK1:
        currentFrame = guts_right_walk1;
        break;
    case GUTS_RIGHT_WALK2:
        currentFrame = guts_right_walk2;
        break;
    case GUTS_LEFT_IDLE:
        currentFrame = guts_left_idle;
        break;
    case GUTS_LEFT_WALK1:
        currentFrame = guts_left_walk1;
        break;
    case GUTS_LEFT_WALK2:
        currentFrame = guts_left_walk2;
        break;
    case GRIFFITH_FRONT_IDLE:
        currentFrame = griffith_front_idle;
        break;
    case GRIFFITH_FRONT_WALK1:
        currentFrame = griffith_front_walk1;
        break;
    case GRIFFITH_FRONT_WALK2:
        currentFrame = griffith_front_walk2;
        break;
    case GRIFFITH_BACK_IDLE:
        currentFrame = griffith_back_idle;
        break;
    case GRIFFITH_BACK_WALK1:
        currentFrame = griffith_back_walk1;
        break;
    case GRIFFITH_BACK_WALK2:
        currentFrame = griffith_back_walk2;
        break;
    case GRIFFITH_RIGHT_IDLE:
        currentFrame = griffith_right_idle;
        break;
    case GRIFFITH_RIGHT_WALK1:
        currentFrame = griffith_right_walk1;
        break;
    case GRIFFITH_RIGHT_WALK2:
        currentFrame = griffith_right_walk2;
        break;
    case GRIFFITH_LEFT_IDLE:
        currentFrame = griffith_left_idle;
        break;
    case GRIFFITH_LEFT_WALK1:
        currentFrame = griffith_left_walk1;
        break;
    case GRIFFITH_LEFT_WALK2:
        currentFrame = griffith_left_walk2;
        break;
    default:
        currentFrame = guts_front_idle;
    }

    int frameWidth = 20;
    uint32_t mask = 0x0;

    int startX = pos.x - frameWidth / 2;
    int startY = pos.y - frameWidth / 2;

    for (int y = 0; y < frameWidth; y++)
        for (int x = 0; x < frameWidth; x++)
        {
            uint32_t pixel = currentFrame[y * frameWidth + x];
            if (pixel != mask)
                drawPixelARGB32(startX + x, startY + y, pixel);
        }
}

/*
 * Clear animation of previous move
 */
void clear_character_frame(Position pos)
{
    int frameWidth = 20;

    int startX = pos.x - frameWidth / 2;
    int startY = pos.y - frameWidth / 2;

    for (int y = 0; y < frameWidth; y++)
        for (int x = 0; x < frameWidth; x++)
            drawPixelARGB32(startX + x, startY + y, path_color);
}

/*
 * Show animation for characters when moving
 */
void handle_character_movement(Character *character, int input, int level)
{
    Position temp_pos = character->box.pos;

    switch (input)
    {
    case 'w':
        if (character == &guts)
        {
            currentFrame = GUTS_BACK_IDLE;
        }
        else if (character == &griffith)
        {
            currentFrame = GRIFFITH_BACK_IDLE;
        }
        break;
    case 's':
        if (character == &guts)
        {
            currentFrame = GUTS_FRONT_IDLE;
        }
        else if (character == &griffith)
        {
            currentFrame = GRIFFITH_FRONT_IDLE;
        }
        break;
    case 'a':
        if (character == &guts)
        {
            currentFrame = GUTS_LEFT_IDLE;
        }
        else if (character == &griffith)
        {
            currentFrame = GRIFFITH_LEFT_IDLE;
        }
        break;
    case 'd':
        if (character == &guts)
        {
            currentFrame = GUTS_RIGHT_IDLE;
        }
        else if (character == &griffith)
        {
            currentFrame = GRIFFITH_RIGHT_IDLE;
        }
        break;
    default:
        return;
    }

    clear_fov(character->box.pos, character->FOV_radius); // Clear previous FOV before moving

    switch (input)
    {
    case 'w':
        temp_pos.y -= PLAYER_STEP;
        break;
    case 's':
        temp_pos.y += PLAYER_STEP;
        break;
    case 'a':
        temp_pos.x -= PLAYER_STEP;
        break;
    case 'd':
        temp_pos.x += PLAYER_STEP;
        break;
    }

    // Update entities' position
    character->box.pos = temp_pos;
    character->currentFrame = currentFrame;

    make_fov(character->box.pos, character->FOV_radius, level);        // Draw updated FOV
    draw_character_frame(character->box.pos, character->currentFrame); // Draw updated character frame
}

/*
 * Show icon for items
 */
void draw_icon_frame(Position pos, IconFrame frame)
{
    uint32_t *icon_frame;

    switch (frame)
    {
    case STAR_FRAME:
        icon_frame = star_frame;
        break;
    case BOMB_FRAME:
        icon_frame = bomb_frame;
        break;
    case KEY_FRAME:
        icon_frame = key_frame;
        break;
    case LEVER_FRAME:
        icon_frame = lever_frame;
        break;
    case TRAP_FRAME:
        icon_frame = trap_frame;
        break;
    default:
        icon_frame = 0;
    }

    int frameWidth = 20;
    uint32_t mask = 0x0;

    int startX = pos.x - frameWidth / 2;
    int startY = pos.y - frameWidth / 2;

    for (int y = 0; y < frameWidth; y++)
        for (int x = 0; x < frameWidth; x++)
        {
            uint32_t pixel = icon_frame[y * frameWidth + x];
            if (pixel != mask)
                drawPixelARGB32(startX + x, startY + y, pixel);
        }
}

/*
 * Check if items are in characters' FOV
 */
int in_FOV(Character character, Item item)
{
    if (character.FOV_radius > distance(character.box.pos.x, item.box.pos.x, character.box.pos.y, item.box.pos.y))
        return 1;
    else
        return 0;
}

/*
 * Show level title of each maze
 */
void show_level_title(int level)
{
    switch (level)
    {
    case 0:
        drawStringARGB32(200, 400, "Level 1", 0x00ffffff, 4);
        break;
    case 1:
        drawStringARGB32(200, 400, "Level 2", 0x00FFB84C, 4);
        break;
    case 2:
        drawStringARGB32(200, 400, "Level 3", 0x00F266AB, 4);
        break;
    case 3:
        drawStringARGB32(200, 400, "Level 4", 0x00A459D1, 4);
        break;
    case 4:
        drawStringARGB32(200, 400, "Level 5", 0x002CD3E1, 4);
        break;
    default:
        break;
    }
}