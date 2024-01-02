#include "functions/game.h"
#include "functions/graphiks.h"
#include "functions/pacman.h"
#include "functions/init.h"

struct Ghost
{
    double x;
    double y;
    int x_block_cordinates;
    int y_block_cordinates;

    double x_speed;
    double y_speed;
    double moovement_speed;

    double animation_frame;
    SDL_Texture *tiles[4];

    int x_target;
    int y_target;

    char character;
} Ghost;

void initRedGhost(struct Ghost *ghost, struct GameMap *game_map)
{
    ghost->x_block_cordinates = 18;
    ghost->y_block_cordinates = 17;
    ghost->x = ghost->x_block_cordinates * BLOCK_SIZE;
    ghost->y = ghost->y_block_cordinates * BLOCK_SIZE;

    ghost->moovement_speed = MOVEMENT_SPEED * 0.75;
    ghost->x_speed = ghost->moovement_speed;
    ghost->y_speed = 0;

    ghost->animation_frame = 0;
    ghost->tiles[0] = NULL;
    ghost->tiles[1] = NULL;
    ghost->tiles[2] = NULL;
    ghost->tiles[3] = NULL;

    ghost->character = 'r';
}
void initPinkGhost(struct Ghost *ghost, struct GameMap *game_map)
{
    ghost->x_block_cordinates = 18;
    ghost->y_block_cordinates = 17;
    ghost->x = ghost->x_block_cordinates * BLOCK_SIZE;
    ghost->y = ghost->y_block_cordinates * BLOCK_SIZE;

    ghost->moovement_speed = MOVEMENT_SPEED * 0.75;
    ghost->x_speed = ghost->moovement_speed;
    ghost->y_speed = 0;

    ghost->animation_frame = 0;
    ghost->tiles[0] = NULL;
    ghost->tiles[1] = NULL;
    ghost->tiles[2] = NULL;
    ghost->tiles[3] = NULL;

    ghost->character = 'p';
}
void drawGhost(struct SDL_Renderer *renderer, struct Ghost *ghost, struct GameMap *game_map)
{

    if (ghost->character == 'r')
    {
        // draw ghost image hitbox
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_Rect ghost_box = {.x = ghost->x, .y = ghost->y, .w = BLOCK_SIZE, .h = BLOCK_SIZE};
        SDL_RenderFillRect(renderer, &ghost_box);
    }
    else if (ghost->character == 'p')
    {
        SDL_SetRenderDrawColor(renderer, 255, 192, 203, 255);
        SDL_Rect ghost_box = {.x = ghost->x, .y = ghost->y, .w = BLOCK_SIZE, .h = BLOCK_SIZE};
        SDL_RenderFillRect(renderer, &ghost_box);
    }
    // draw ghost position map(greeen)
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_Rect ghost_box1 = {.x = ghost->x_block_cordinates * BLOCK_SIZE, .y = ghost->y_block_cordinates * BLOCK_SIZE + game_map->top_margin, .w = BLOCK_SIZE, .h = BLOCK_SIZE};
    SDL_RenderFillRect(renderer, &ghost_box1);
}

void ghostMove(struct Ghost *ghost, struct GameMap *game_map, double delta_time, struct Wall *walls, int to_x, int to_y)
{
    double top_distance = sqrt(pow(ghost->x_block_cordinates - to_x, 2) + pow(ghost->y_block_cordinates - 1 - to_y, 2));
    double bottom_distance = sqrt(pow(ghost->x_block_cordinates - to_x, 2) + pow(ghost->y_block_cordinates + 1 - to_y, 2));
    double right_distance = sqrt(pow(ghost->x_block_cordinates + 1 - to_x, 2) + pow(ghost->y_block_cordinates - to_y, 2));
    double left_distance = sqrt(pow(ghost->x_block_cordinates - 1 - to_x, 2) + pow(ghost->y_block_cordinates - to_y, 2));
    bool able_top = isAbleToGo(walls, game_map, ghost->x_block_cordinates, ghost->y_block_cordinates - 1) && !(ghost->y_speed > 0);
    bool able_bottom = isAbleToGo(walls, game_map, ghost->x_block_cordinates, ghost->y_block_cordinates + 1) && !(ghost->y_speed < 0);
    bool able_right = isAbleToGo(walls, game_map, ghost->x_block_cordinates + 1, ghost->y_block_cordinates) && !(ghost->x_speed < 0);
    bool able_left = isAbleToGo(walls, game_map, ghost->x_block_cordinates - 1, ghost->y_block_cordinates) && !(ghost->x_speed > 0);

    // if (((top_distance <= bottom_distance) * able_bottom) && ((top_distance <= right_distance) * able_right) && ((top_distance <= left_distance) * able_left) && able_top)

    if (able_top && ((top_distance <= bottom_distance) || !able_bottom) && ((top_distance <= left_distance) || !able_left) && ((top_distance <= right_distance) || !able_right))
    {
        ghost->x_speed = 0;
        ghost->y_speed = -ghost->moovement_speed;
    }
    else if (able_bottom && ((bottom_distance <= top_distance) || !able_top) && ((bottom_distance <= left_distance) || !able_left) && ((bottom_distance <= right_distance) || !able_right))
    {
        ghost->x_speed = 0;
        ghost->y_speed = ghost->moovement_speed;
    }
    else if (able_right && ((right_distance <= bottom_distance) || !able_bottom) && ((right_distance <= left_distance) || !able_left) && ((right_distance <= top_distance) || !able_top))
    {
        ghost->x_speed = ghost->moovement_speed;
        ghost->y_speed = 0;
    }
    else if (able_left && ((left_distance <= bottom_distance) || !able_bottom) && ((left_distance <= right_distance) || !able_right) && ((left_distance <= top_distance) || !able_top))
    {
        ghost->x_speed = -ghost->moovement_speed;
        ghost->y_speed = 0;
    }

    ghost->x = round(ghost->x + ghost->x_speed * delta_time);
    ghost->y = round(ghost->y + ghost->y_speed * delta_time);

    if (ghost->x_block_cordinates < -1)
        ghost->x = game_map->pixel_width;
    else if (ghost->x_block_cordinates > game_map->block_width)
        ghost->x = -1 * BLOCK_SIZE;
    else if (ghost->x_block_cordinates >= 0 && ghost->x_block_cordinates < game_map->block_width && ghost->y_block_cordinates >= 0 && ghost->y_block_cordinates < game_map->block_height)
    {
        if (ghost->x_speed > 0) // checks if ghost is in a wall // add general if x y is not out of border
        {
            if ((walls[ghost->y_block_cordinates * game_map->block_width + ghost->x_block_cordinates + 1].hitBox.x <= ghost->x + BLOCK_SIZE) && walls[ghost->y_block_cordinates * game_map->block_width + ghost->x_block_cordinates + 1].type >= 'a' && walls[ghost->y_block_cordinates * game_map->block_width + ghost->x_block_cordinates + 1].type <= 'y')
            {
                ghost->x = walls[ghost->y_block_cordinates * game_map->block_width + ghost->x_block_cordinates + 1].hitBox.x - BLOCK_SIZE;
            }
        }
        else if (ghost->x_speed < 0)
        {
            if ((walls[ghost->y_block_cordinates * game_map->block_width + ghost->x_block_cordinates - 1].hitBox.x >= ghost->x - BLOCK_SIZE) && walls[ghost->y_block_cordinates * game_map->block_width + ghost->x_block_cordinates - 1].type >= 'a' && walls[ghost->y_block_cordinates * game_map->block_width + ghost->x_block_cordinates - 1].type <= 'y')
            {
                ghost->x = walls[ghost->y_block_cordinates * game_map->block_width + ghost->x_block_cordinates - 1].hitBox.x + BLOCK_SIZE;
            }
        }
        else if (ghost->y_speed > 0)
        {
            if ((walls[(ghost->y_block_cordinates + 1) * game_map->block_width + ghost->x_block_cordinates].hitBox.y <= ghost->y + BLOCK_SIZE) && walls[(ghost->y_block_cordinates + 1) * game_map->block_width + ghost->x_block_cordinates].type >= 'a' && walls[(ghost->y_block_cordinates + 1) * game_map->block_width + ghost->x_block_cordinates].type <= 'y')
            {
                ghost->y = walls[(ghost->y_block_cordinates + 1) * game_map->block_width + ghost->x_block_cordinates].hitBox.y - BLOCK_SIZE;
            }
        }
        else if (ghost->y_speed < 0)
        {
            if ((walls[(ghost->y_block_cordinates - 1) * game_map->block_width + ghost->x_block_cordinates].hitBox.y >= ghost->y - BLOCK_SIZE) && walls[(ghost->y_block_cordinates - 1) * game_map->block_width + ghost->x_block_cordinates].type >= 'a' && walls[(ghost->y_block_cordinates - 1) * game_map->block_width + ghost->x_block_cordinates].type <= 'y')
            {
                ghost->y = walls[(ghost->y_block_cordinates - 1) * game_map->block_width + ghost->x_block_cordinates].hitBox.y + BLOCK_SIZE;
            }
        }
    }

    ghost->x_block_cordinates = round((ghost->x / BLOCK_SIZE));
    ghost->y_block_cordinates = round((ghost->y - game_map->top_margin) / BLOCK_SIZE);
}

void mooveRedGhost(struct Ghost *ghost, struct GameMap *game_map, double delta_time, struct Wall *walls, struct Pacman *pacman)
{
    ghostMove(ghost, game_map, delta_time, walls, pacman->x_block_cordinates, pacman->y_block_cordinates);
}
void moovePinkGhost(struct Ghost *ghost, struct GameMap *game_map, double delta_time, struct Wall *walls, struct Pacman *pacman)
{
    if (pacman->y_speed < 0)
    {
        ghostMove(ghost, game_map, delta_time, walls, pacman->x_block_cordinates - 4, pacman->y_block_cordinates - 4);
    }
    else if (pacman->y_speed > 0)
    {
        ghostMove(ghost, game_map, delta_time, walls, pacman->x_block_cordinates, pacman->y_block_cordinates + 4);
    }
    else if (pacman->x_speed < 0)
    {
        ghostMove(ghost, game_map, delta_time, walls, pacman->x_block_cordinates - 4, pacman->y_block_cordinates);
    }
    else if (pacman->x_speed > 0)
    {
        ghostMove(ghost, game_map, delta_time, walls, pacman->x_block_cordinates + 4, pacman->y_block_cordinates);
    }
}
int main()
{
    struct Ghost redGhost;
    struct Ghost pinkGhost;
    struct GameMap game_map;
    struct Pacman pacman = {.lives = 4};
    struct Wall *map = NULL;

    initRedGhost(&redGhost, &game_map);
    initPinkGhost(&pinkGhost, &game_map);

    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    WindowGameInit(&game_map, &pacman, &window, &renderer);

    ImageInit(&pacman, &map, renderer, &game_map);

    bool running = true;
    SDL_Event event = {};

    Uint64 delta_now = SDL_GetPerformanceCounter();
    Uint64 delta_last = 0;
    double delta_time = 0;

    // keys
    bool key_up_pressed = false;
    bool key_down_pressed = false;
    bool key_left_pressed = false;
    bool key_right_pressed = false;

    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
            }
            else if (event.type == SDL_KEYDOWN)
            {
                switch (event.key.keysym.sym)
                {
                case SDLK_UP:
                    key_up_pressed = true;
                    key_down_pressed = false;
                    key_left_pressed = false;
                    key_right_pressed = false;
                    break;
                case SDLK_DOWN:
                    key_down_pressed = true;
                    key_up_pressed = false;
                    key_left_pressed = false;
                    key_right_pressed = false;
                    break;
                case SDLK_LEFT:
                    key_left_pressed = true;
                    key_up_pressed = false;
                    key_down_pressed = false;
                    key_right_pressed = false;
                    break;
                case SDLK_RIGHT:
                    key_right_pressed = true;
                    key_up_pressed = false;
                    key_down_pressed = false;
                    key_left_pressed = false;
                    break;
                }
                break;
            }
            else if (event.type == SDL_KEYUP)
            {
                switch (event.key.keysym.sym)
                {
                case SDLK_UP:
                    key_up_pressed = false;
                    break;
                case SDLK_DOWN:
                    key_down_pressed = false;
                    break;
                case SDLK_LEFT:
                    key_left_pressed = false;
                    break;
                case SDLK_RIGHT:
                    key_right_pressed = false;
                    break;
                }
                break;
            }
        }

        if (key_up_pressed && isAbleToGo(map, &game_map, pacman.x_block_cordinates, pacman.y_block_cordinates - 1))
        {
            pacman.y_speed = -1 * pacman.moovement_speed;
            pacman.x_speed = 0;
        }
        else if (key_down_pressed && isAbleToGo(map, &game_map, pacman.x_block_cordinates, pacman.y_block_cordinates + 1))
        {
            pacman.y_speed = pacman.moovement_speed;
            pacman.x_speed = 0;
        }
        else if (key_left_pressed && isAbleToGo(map, &game_map, pacman.x_block_cordinates - 1, pacman.y_block_cordinates))
        {
            pacman.x_speed = -1 * pacman.moovement_speed;
            pacman.y_speed = 0;
        }
        else if (key_right_pressed && isAbleToGo(map, &game_map, pacman.x_block_cordinates + 1, pacman.y_block_cordinates))
        {
            pacman.x_speed = pacman.moovement_speed;
            pacman.y_speed = 0;
        }

        // Delta time
        delta_last = delta_now;
        delta_now = SDL_GetPerformanceCounter();
        delta_time = (double)((delta_now - delta_last) * 1000 / (double)SDL_GetPerformanceFrequency());

        // draw wals, pacman, score, bg
        mapUxDraw(renderer, map, &pacman, &game_map);

        // checking points
        if (game_map.collected_point_amount < game_map.point_amount)
        {
            // moove pacman hitbox
            pacmanMoove(&pacman, delta_time, &game_map, map);
            // pacman animate
            pacman_animate(&pacman, delta_time);
            //  check and collect intersected points
            point_collector(&pacman, &game_map, map);

            // red
            drawGhost(renderer, &redGhost, &game_map);
            mooveRedGhost(&redGhost, &game_map, delta_time, map, &pacman);

            // pink
            drawGhost(renderer, &pinkGhost, &game_map);
            moovePinkGhost(&pinkGhost, &game_map, delta_time, map, &pacman);
        }
        else
        {
            printf("You won\n");
        }

        // update renderer
        SDL_RenderPresent(renderer);
    }

    // Uvolnění prostředků
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    free(map);

    return 0;
}