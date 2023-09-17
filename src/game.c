#include "game.h"

#define PLAYER_STEP MAZE_HEIGHT / 19
#define ENTITY_RADIUS 10

// Colors
#define GUTS 0x0068ADFF     // 1st character
#define GRIFFITH 0x007030A0 // 2nd character
#define WALL 0x00000000
#define PATH 0x00FFFFFF
#define BOMB 0x00FF0101   // Red
#define STAR 0x00FFC000   // Yellow == Battery
#define KEY 0x0070AD47    // Green
#define TRAP 0x00FF28F0   // Pink
#define DETRAP 0x0027FFDC // Cyan
#define WHITE 0x00ffffff
#define BLACK 0x00000000
#define FRAME_CHANGE_INTERVAL 12

#define DELAY_TIME 15000

IconType maze[MAZE_WIDTH][MAZE_HEIGHT];

int default_fov = 66;
int star_flag = 1;
int bomb_flag = 2;
int key_flag = 3;
int trap_flag = 4;
int detrap_flag = 5;

Entity guts;
Entity griffith;
Entity star;
Entity bomb;
Entity key;
Entity trap;
Entity detrap;

AnimationState guts_animation_state = GUTS_FRONT_IDLE;
AnimationState griffith_animation_state = GRIFFITH_FRONT_IDLE;

void game(int *level)
{
    // Default position of main character and exit gate
    Position start_pos1 = {PLAYER_STEP / 2, PLAYER_STEP * 10 - PLAYER_STEP / 2};                 // No modify
    Position end_pos = {PLAYER_STEP * 19 - PLAYER_STEP / 2, PLAYER_STEP * 10 - PLAYER_STEP / 2}; // No modify

    // Position of other entities based on maze level
    Position start_pos2;
    Position star_pos;
    Position bomb_pos;
    Position key_pos;
    Position trap_pos;
    Position detrap_pos;

    set_maze_entity_position(*level, &start_pos2, &star_pos, &bomb_pos, &key_pos, &trap_pos, &detrap_pos, &default_fov);

    // Initialize hitbox
    Box guts_box = {start_pos1, ENTITY_RADIUS, ENTITY_RADIUS};
    Box griffith_box = {start_pos2, ENTITY_RADIUS, ENTITY_RADIUS};
    Box star_box = {star_pos, ENTITY_RADIUS, ENTITY_RADIUS};
    Box bomb_box = {bomb_pos, ENTITY_RADIUS, ENTITY_RADIUS};
    Box key_box = {key_pos, ENTITY_RADIUS, ENTITY_RADIUS};
    Box trap_box = {trap_pos, ENTITY_RADIUS, ENTITY_RADIUS};
    Box detrap_box = {detrap_pos, ENTITY_RADIUS, ENTITY_RADIUS};

    // Initialize entity
    guts.box = guts_box;
    guts.FOV_radius = default_fov;
    griffith.box = griffith_box;
    griffith.FOV_radius = default_fov;
    star.box = star_box;
    bomb.box = bomb_box;
    key.box = key_box;
    trap.box = trap_box;
    detrap.box = detrap_box;

    clear_maze();

    guts.currentFrame = GUTS_FRONT_IDLE;
    griffith.currentFrame = GRIFFITH_FRONT_IDLE;
    star.iconFrame = STAR_FRAME;
    bomb.iconFrame = BOMB_FRAME;
    key.iconFrame = KEY_FRAME;
    trap.iconFrame = TRAP_FRAME;
    detrap.iconFrame = LEVER_FRAME;

    switch (*level) // Show level title
    {
    case 0:
        drawStringARGB32(200, 400, "Level 1", 0x00ffffff, 4);
        break;
    case 1:
        drawStringARGB32(200, 400, "Level 2", 0x00ffffff, 4);
        break;
    case 2:
        drawStringARGB32(200, 400, "Level 3", 0x00ffffff, 4);
        break;
    case 3:
        drawStringARGB32(200, 400, "Level 4", 0x00ffffff, 4);
        break;
    case 4:
        drawStringARGB32(200, 400, "Level 5", 0x00ffffff, 4);
        break;
    default:
        break;
    }
    wait_msec(1000000);
    clear_maze();

    // Begin game
    while (1)
    {
        // Check for win
        if (win(guts.box.pos, end_pos, key_flag)) // Guts reach exit gate and key is retrieved
        {
            clear_maze();
            drawStringARGB32(200, 400, "ok you win!", 0x00ffffff, 3);
            wait_msec(1000000);
            if (*level < 5) // If not reach last level, increase level
            {
                // Next level
                int temp_level = *level;
                temp_level++;
                *level = temp_level;
                set_level(); // Set default value for entities
                break;
            }
        }

        else
        {
            // Show FOV for 2 characters
            make_fov(guts.box.pos, guts.FOV_radius, *level);
            make_fov(griffith.box.pos, griffith.FOV_radius, *level);

            // Draw 2 characters
            drawCharacterFrame(guts.box.pos, guts.currentFrame);
            drawCharacterFrame(griffith.box.pos, griffith.currentFrame);

            if (detrap_flag == 0) // Check for detrap, if detrapped, deactivate trap
            {
                trap.box.pos.x = -1;
                trap.box.pos.y = -1;
            }

            if (trap_flag == 0) // Check for trap, if trapped, reset level
            {
                clear_maze();
                drawStringARGB32(200, 400, "you died", 0x00ffffff, 3);
                wait_msec(1000000);
                set_level();
                break;
            }

            // // Draw star & bomb & key hidden
            // if (star_flag == 1)
            //     drawIconFrame(star.box.pos, star.iconFrame);
            // if (bomb_flag == 1)
            //     drawIconFrame(bomb.box.pos, bomb.iconFrame);
            // if (key_flag == 1)
            //     drawIconFrame(key.box.pos, key.iconFrame);

            // If entities are inside characters' FOV, they're shown
            if (star_flag == 1 && (guts.FOV_radius > distance(guts.box.pos.x, star.box.pos.x, guts.box.pos.y, star.box.pos.y) ||
                                   griffith.FOV_radius > distance(griffith.box.pos.x, star.box.pos.x, griffith.box.pos.y, star.box.pos.y)))
                drawIconFrame(star.box.pos, star.iconFrame);

            if (bomb_flag == 2 && (guts.FOV_radius > distance(guts.box.pos.x, bomb.box.pos.x, guts.box.pos.y, bomb.box.pos.y) ||
                                   griffith.FOV_radius > distance(griffith.box.pos.x, bomb.box.pos.x, griffith.box.pos.y, bomb.box.pos.y)))
                drawIconFrame(bomb.box.pos, bomb.iconFrame);

            if (key_flag == 3 && (guts.FOV_radius > distance(guts.box.pos.x, key.box.pos.x, guts.box.pos.y, key.box.pos.y) ||
                                  griffith.FOV_radius > distance(griffith.box.pos.x, key.box.pos.x, griffith.box.pos.y, key.box.pos.y)))
                drawIconFrame(key.box.pos, key.iconFrame);

            if (trap_flag == 4 && detrap_flag == 5 &&
                (guts.FOV_radius > distance(guts.box.pos.x, trap.box.pos.x, guts.box.pos.y, trap.box.pos.y) ||
                 griffith.FOV_radius > distance(griffith.box.pos.x, trap.box.pos.x, griffith.box.pos.y, trap.box.pos.y)))
                drawIconFrame(trap.box.pos, trap.iconFrame);

            if (detrap_flag == 5 && (guts.FOV_radius > distance(guts.box.pos.x, detrap.box.pos.x, guts.box.pos.y, detrap.box.pos.y) ||
                                     griffith.FOV_radius > distance(griffith.box.pos.x, detrap.box.pos.x, griffith.box.pos.y, detrap.box.pos.y)))
                drawIconFrame(detrap.box.pos, detrap.iconFrame);

            // Get direction from player
            char input = uart_getc();
            handle_input(&guts, input, *level);
            handle_input(&griffith, input, *level);
        }
    }
}

// Function for input directions
void handle_input(Entity *entity, int input, int level)
{
    switch (input)
    {
    case 'w':                                                             // Up
        if (walkable(entity->box.pos.x, entity->box.pos.y - PLAYER_STEP)) // Check for wall for next step
        {
            // clear_fov(entity->box.pos, entity->FOV_radius); // Clear previous FOV before moving
            if (entity->box.pos.y - PLAYER_STEP > 0)
            {
                handleAndAnimateCharacterMovement(entity, input, level);
                check_entity(entity, &star, &star_flag);
                check_entity(entity, &bomb, &bomb_flag);
                check_entity(entity, &key, &key_flag);
                check_entity(entity, &trap, &trap_flag);
                check_entity(entity, &detrap, &detrap_flag);
            }
        }
        break;

    case 's': // Down
        if (walkable(entity->box.pos.x, entity->box.pos.y + PLAYER_STEP))
        {
            // clear_fov(entity->box.pos, entity->FOV_radius);
            if (entity->box.pos.y + PLAYER_STEP < MAZE_WIDTH)
            {
                handleAndAnimateCharacterMovement(entity, input, level);
                check_entity(entity, &star, &star_flag);
                check_entity(entity, &bomb, &bomb_flag);
                check_entity(entity, &key, &key_flag);
                check_entity(entity, &trap, &trap_flag);
                check_entity(entity, &detrap, &detrap_flag);
            }
        }
        break;

    case 'a': // Left
        if (walkable(entity->box.pos.x - PLAYER_STEP, entity->box.pos.y))
        {
            // clear_fov(entity->box.pos, entity->FOV_radius);
            if (entity->box.pos.x - PLAYER_STEP > 0)
            {
                handleAndAnimateCharacterMovement(entity, input, level);
                check_entity(entity, &star, &star_flag);
                check_entity(entity, &bomb, &bomb_flag);
                check_entity(entity, &key, &key_flag);
                check_entity(entity, &trap, &trap_flag);
                check_entity(entity, &detrap, &detrap_flag);
            }
        }
        break;

    case 'd': // Right
        if (walkable(entity->box.pos.x + PLAYER_STEP, entity->box.pos.y))
        {
            // clear_fov(entity->box.pos, entity->FOV_radius);
            if (entity->box.pos.x + PLAYER_STEP < MAZE_HEIGHT)
            {
                handleAndAnimateCharacterMovement(entity, input, level);
                check_entity(entity, &star, &star_flag);
                check_entity(entity, &bomb, &bomb_flag);
                check_entity(entity, &key, &key_flag);
                check_entity(entity, &trap, &trap_flag);
                check_entity(entity, &detrap, &detrap_flag);
            }
        }
        break;

        // Cheat
    case 'o':
        increase_fov(entity->box.pos, &entity->FOV_radius);
        break;
    case 'p':
        decrease_fov(entity->box.pos, &entity->FOV_radius);
        break;

    default:
        break;
    }
}

// Draw field of view
void make_fov(Position pos, int radius, int level)
{
    for (int y = 0; y < MAZE_HEIGHT; y++)
        for (int x = 0; x < MAZE_WIDTH; x++)
            if (x * x + y * y < radius * radius)
            {
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

// Clear field of view
void clear_fov(Position pos, int radius)
{
    for (int y = 0; y < MAZE_HEIGHT; y++)
        for (int x = 0; x < MAZE_WIDTH; x++)
            if (x * x + y * y <= radius * radius)
            {
                drawPixelARGB32(pos.x + x, pos.y + y, WALL);
                drawPixelARGB32(pos.x - x, pos.y - y, WALL);
                drawPixelARGB32(pos.x + x, pos.y - y, WALL);
                drawPixelARGB32(pos.x - x, pos.y + y, WALL);
            }
}

// Increase field of view
void increase_fov(Position pos, int *radius)
{

    drawCircleARGB32(pos.x, pos.y, *radius, WALL);
    if (*radius < 200)
        *radius += 40;
}

// Decrease field of view
void decrease_fov(Position pos, int *radius)
{
    drawCircleARGB32(pos.x, pos.y, *radius, WALL);
    if (*radius > PLAYER_STEP * 2)
        *radius -= 30;
}

// Check if player has escaped or not
int win(Position pos, Position win, int flag)
{
    if (pos.x == win.x && pos.y == win.y && flag == 0)
        return 1;
    else
        return 0;
}

// Check for wall
int walkable(int pos_x, int pos_y)
{
    IconType icon = maze[pos_x][pos_y];

    if (getPixelARGB32(pos_x, pos_y) == PATH)
        return 1;

    if (icon == STAR || icon == BOMB || icon == KEY || icon == LEVER || icon == TRAP)
        return 1;

    return 0;
}

// Detect collision of 2 hitboxes
int detect_collision(Box a, Box b)
{
    if (a.pos.x < b.pos.x + b.width &&
        a.pos.x + a.width > b.pos.x &&
        a.pos.y < b.pos.y + b.height &&
        a.pos.y + a.height > b.pos.y)
    {
        return 1; // Collision detected
    }
    return 0; // No collision
}

// Clear screen
void clear_maze()
{
    for (int y = 0; y <= MAZE_HEIGHT; y++)
        for (int x = 0; x <= MAZE_WIDTH; x++)
            drawPixelARGB32(x, y, WALL);
}

/*
 * Check for interaction of characters and entities
 * If there is collision, do action and set flag off
 */
void check_entity(Entity *entity1, Entity *entity2, int *flag)
{
    int temp = *flag;

    if (detect_collision(entity1->box, entity2->box))
    {
        if (temp == 1) // Star
        {
            increase_fov(entity1->box.pos, &entity1->FOV_radius);
            drawCircleARGB32(entity2->box.pos.x, entity2->box.pos.y, ENTITY_RADIUS, PATH);
            temp = 0;
        }

        else if (temp == 2) // Bomb
        {
            decrease_fov(entity1->box.pos, &entity1->FOV_radius);
            drawCircleARGB32(entity2->box.pos.x, entity2->box.pos.y, ENTITY_RADIUS, PATH);
            temp = 0;
        }

        else if (temp == 3) // Key
        {
            drawCircleARGB32(entity2->box.pos.x, entity2->box.pos.y, ENTITY_RADIUS, PATH);
            temp = 0;
        }

        else if (temp == 4) // Trap
        {
            drawCircleARGB32(entity2->box.pos.x, entity2->box.pos.y, ENTITY_RADIUS, PATH);
            temp = 0;
        }

        else if (temp == 5) // Trapless
        {
            drawCircleARGB32(entity2->box.pos.x, entity2->box.pos.y, ENTITY_RADIUS, PATH);
            temp = 0;
        }
    }

    *flag = temp;
}

// Set default value for characters and flags
void set_level()
{
    // Set FOV
    guts.FOV_radius = default_fov;
    griffith.FOV_radius = default_fov;

    // Reset flags
    star_flag = 1;
    bomb_flag = 2;
    key_flag = 3;
    trap_flag = 4;
    detrap_flag = 5;
}

// Set value for entities based on level
void set_maze_entity_position(int level, Position *start2, Position *star, Position *bomb, Position *key, Position *trap, Position *detrap, int *fov)
{
    switch (level)
    {
    case 0:

        // level 1
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

        *fov = 88;

        break;

    case 1:
        // Level 2

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

        *fov = 83;
        break;

    case 2:
        // Level 3
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

        *fov = 78;
        break;

    case 3:
        // Level 4
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

        *fov = 68;
        break;

    case 4:
        // Level 5
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

        *fov = 58;
        break;

    default:
        break;
    }
}

void drawCharacterFrame(Position pos, AnimationState state)
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
    uint32_t black = 0x00000000;

    int startX = pos.x - frameWidth / 2;
    int startY = pos.y - frameWidth / 2;

    for (int y = 0; y < frameWidth; y++)
    {
        for (int x = 0; x < frameWidth; x++)
        {
            uint32_t pixel = currentFrame[y * frameWidth + x];

            if (pixel != black)
            {
                drawPixelARGB32(startX + x, startY + y, pixel);
            }
        }
    }
}

void clearCharacterFrame(Position pos)
{
    int frameWidth = 20;

    int startX = pos.x - frameWidth / 2;
    int startY = pos.y - frameWidth / 2;

    for (int y = 0; y < frameWidth; y++)
    {
        for (int x = 0; x < frameWidth; x++)
        {
            drawPixelARGB32(startX + x, startY + y, PATH);
        }
    }
}

void handleAndAnimateCharacterMovement(Entity *entity, int input, int level)
{
    Position temp_pos = entity->box.pos;
    AnimationState animations[4];

    switch (input)
    {
    case 'w':
        if (entity == &guts)
        {
            animations[0] = GUTS_BACK_IDLE;
            animations[1] = GUTS_BACK_WALK1;
            animations[2] = GUTS_BACK_WALK2;
            animations[3] = GUTS_BACK_IDLE;
        }
        else if (entity == &griffith)
        {
            animations[0] = GRIFFITH_BACK_IDLE;
            animations[1] = GRIFFITH_BACK_WALK1;
            animations[2] = GRIFFITH_BACK_WALK2;
            animations[3] = GRIFFITH_BACK_IDLE;
        }
        break;
    case 's':
        if (entity == &guts)
        {
            animations[0] = GUTS_FRONT_IDLE;
            animations[1] = GUTS_FRONT_WALK1;
            animations[2] = GUTS_FRONT_WALK2;
            animations[3] = GUTS_FRONT_IDLE;
        }
        else if (entity == &griffith)
        {
            animations[0] = GRIFFITH_FRONT_IDLE;
            animations[1] = GRIFFITH_FRONT_WALK1;
            animations[2] = GRIFFITH_FRONT_WALK2;
            animations[3] = GRIFFITH_FRONT_IDLE;
        }
        break;
    case 'a':
        if (entity == &guts)
        {
            animations[0] = GUTS_LEFT_IDLE;
            animations[1] = GUTS_LEFT_WALK1;
            animations[2] = GUTS_LEFT_WALK2;
            animations[3] = GUTS_LEFT_IDLE;
        }
        else if (entity == &griffith)
        {
            animations[0] = GRIFFITH_LEFT_IDLE;
            animations[1] = GRIFFITH_LEFT_WALK1;
            animations[2] = GRIFFITH_LEFT_WALK2;
            animations[3] = GRIFFITH_LEFT_IDLE;
        }
        break;
    case 'd':
        if (entity == &guts)
        {
            animations[0] = GUTS_RIGHT_IDLE;
            animations[1] = GUTS_RIGHT_WALK1;
            animations[2] = GUTS_RIGHT_WALK2;
            animations[3] = GUTS_RIGHT_IDLE;
        }
        else if (entity == &griffith)
        {
            animations[0] = GRIFFITH_RIGHT_IDLE;
            animations[1] = GRIFFITH_RIGHT_WALK1;
            animations[2] = GRIFFITH_RIGHT_WALK2;
            animations[3] = GRIFFITH_RIGHT_IDLE;
        }
        break;
    default:
        return;
    }

    for (int i = 0; i < 4; i++)
    {
        clearCharacterFrame(temp_pos);
        clear_fov(entity->box.pos, entity->FOV_radius); // Clear previous FOV before moving

        switch (input)
        {
        case 'w':
            temp_pos.y -= FRAME_CHANGE_INTERVAL;
            break;
        case 's':
            temp_pos.y += FRAME_CHANGE_INTERVAL;
            break;
        case 'a':
            temp_pos.x -= FRAME_CHANGE_INTERVAL;
            break;
        case 'd':
            temp_pos.x += FRAME_CHANGE_INTERVAL;
            break;
        }

        entity->box.pos = temp_pos;
        entity->currentFrame = animations[i];

        make_fov(guts.box.pos, guts.FOV_radius, level);
        drawCharacterFrame(entity->box.pos, entity->currentFrame);
        wait_msec(DELAY_TIME);
    }
}

void drawIconFrame(Position pos, IconFrame frame)
{
    uint32_t *iconFrame;
    IconType IconType;

    switch (frame)
    {
    case STAR_FRAME:
        iconFrame = star_frame;
        IconType = STAR;
        break;
    case BOMB_FRAME:
        iconFrame = bomb_frame;
        IconType = BOMB;
        break;
    case KEY_FRAME:
        iconFrame = key_frame;
        IconType = KEY;
        break;
    case LEVER_FRAME:
        iconFrame = lever_frame;
        IconType = LEVER;
        break;
    case TRAP_FRAME:
        iconFrame = trap_frame;
        IconType = TRAP;
        break;
    default:
        iconFrame = NULL;
        IconType = EMPTY;
    }

    int frameWidth = 20;
    uint32_t black = 0x00000000;

    int startX = pos.x - frameWidth / 2;
    int startY = pos.y - frameWidth / 2;

    for (int y = 0; y < frameWidth; y++)
    {
        for (int x = 0; x < frameWidth; x++)
        {
            uint32_t pixel = iconFrame[y * frameWidth + x];

            if (pixel != black)
            {
                drawPixelARGB32(startX + x, startY + y, pixel);
                maze[startX + x][startY + y] = IconType; // Update maze
            }
        }
    }
}