#ifndef Pacman_h
#define Pacman_h

#include "init.h"
#include "graphiks.h"
void initializePacman(struct Pacman *pacman)
{
    pacman->x_block_cordinates = 14;
    pacman->y_block_cordinates = 17;
    pacman->x = pacman->x_block_cordinates * BLOCK_SIZE - BLOCK_SIZE / 2;
    pacman->y = (pacman->y_block_cordinates + 3) * BLOCK_SIZE;
    pacman->x_speed = 0;
    pacman->y_speed = 0;
    pacman->moovement_speed = MOVEMENT_SPEED;
    pacman->animation_frame = 3;
    pacman->isKilling = false;
    pacman->timer_amount = 0;
}

bool isAbleToGo(struct Wall *map, struct GameMap *game_map, int to_x, int to_y)
{
    if (to_x < 0 || to_x >= game_map->block_width)
        return true;
    if (map[to_y * game_map->block_width + to_x].type >= 'a' && map[to_y * game_map->block_width + to_x].type <= 'y')
        return false;

    return true;
}

void pacmanMoove(struct Pacman *pacman, double delta_time, struct GameMap *map, struct Wall *walls)
{
    // moove pacman hitbox
    pacman->x = round(pacman->x + pacman->x_speed * delta_time);
    pacman->y = round(pacman->y + pacman->y_speed * delta_time);

    // over border check
    if (pacman->x < 0)
        pacman->x = map->pixel_width - BLOCK_SIZE;
    else if (pacman->x + BLOCK_SIZE > map->pixel_width)
        pacman->x = 0;

    // wall and pacman intersection
    else if (pacman->x_block_cordinates > 0 && pacman->x_block_cordinates < map->block_width - 1 && pacman->y_block_cordinates >= 0 && pacman->y_block_cordinates < map->block_height)
    {
        if (pacman->x_speed > 0) // checks if pacman is in a wall // add general if x y is not out of border
        {
            if ((walls[pacman->y_block_cordinates * map->block_width + pacman->x_block_cordinates + 1].hitBox.x <= pacman->x + BLOCK_SIZE) && walls[pacman->y_block_cordinates * map->block_width + pacman->x_block_cordinates + 1].type >= 'a' && walls[pacman->y_block_cordinates * map->block_width + pacman->x_block_cordinates + 1].type <= 'y')
            {
                pacman->x = walls[pacman->y_block_cordinates * map->block_width + pacman->x_block_cordinates + 1].hitBox.x - BLOCK_SIZE;
            }
        }
        else if (pacman->x_speed < 0)
        {
            if ((walls[pacman->y_block_cordinates * map->block_width + pacman->x_block_cordinates - 1].hitBox.x >= pacman->x - BLOCK_SIZE) && walls[pacman->y_block_cordinates * map->block_width + pacman->x_block_cordinates - 1].type >= 'a' && walls[pacman->y_block_cordinates * map->block_width + pacman->x_block_cordinates - 1].type <= 'y')
            {
                pacman->x = walls[pacman->y_block_cordinates * map->block_width + pacman->x_block_cordinates - 1].hitBox.x + BLOCK_SIZE;
            }
        }
        else if (pacman->y_speed > 0)
        {
            if ((walls[(pacman->y_block_cordinates + 1) * map->block_width + pacman->x_block_cordinates].hitBox.y <= pacman->y + BLOCK_SIZE) && walls[(pacman->y_block_cordinates + 1) * map->block_width + pacman->x_block_cordinates].type >= 'a' && walls[(pacman->y_block_cordinates + 1) * map->block_width + pacman->x_block_cordinates].type <= 'y')
            {
                pacman->y = walls[(pacman->y_block_cordinates + 1) * map->block_width + pacman->x_block_cordinates].hitBox.y - BLOCK_SIZE;
            }
        }
        else if (pacman->y_speed < 0)
        {
            if ((walls[(pacman->y_block_cordinates - 1) * map->block_width + pacman->x_block_cordinates].hitBox.y >= pacman->y - BLOCK_SIZE) && walls[(pacman->y_block_cordinates - 1) * map->block_width + pacman->x_block_cordinates].type >= 'a' && walls[(pacman->y_block_cordinates - 1) * map->block_width + pacman->x_block_cordinates].type <= 'y')
            {
                pacman->y = walls[(pacman->y_block_cordinates - 1) * map->block_width + pacman->x_block_cordinates].hitBox.y + BLOCK_SIZE;
            }
        }
    }
    // calculation actual map-orientated pacman coordinates
    pacman->x_block_cordinates = round((pacman->x / BLOCK_SIZE));
    pacman->y_block_cordinates = round((pacman->y - map->top_margin) / BLOCK_SIZE);
}

Uint32 timerCallback(Uint32 interval, void *param)
{
    // Cast the void pointer back to the original struct type
    struct Pacman *pacman = (struct Pacman *)param;

    if (pacman->timer_amount == 1)
    {
        pacman->moovement_speed = MOVEMENT_SPEED;
        pacman->isKilling = false;
        printf("pacman is not killing\n");
    }

    pacman->timer_amount--;
    return 0;
}

void point_collector(struct Pacman *pacman, struct GameMap *map, struct Wall *walls)
{
    if (walls[pacman->y_block_cordinates * map->block_width + pacman->x_block_cordinates].show)
    {
        if (walls[pacman->y_block_cordinates * map->block_width + pacman->x_block_cordinates].type == 'Z')
        {
            printf("pacman is killing\n");

            pacman->isKilling = true;
            pacman->moovement_speed = MOVEMENT_SPEED * 1.15;

            pacman->timer_amount++;
            SDL_AddTimer(4000, timerCallback, pacman);
            map->score += 20;
        }
        walls[pacman->y_block_cordinates * map->block_width + pacman->x_block_cordinates].show = false;
        map->collected_point_amount++;
        map->score += 50;
    }
}

void pacman_animate(struct Pacman *pacman, double delta_time)
{
    if (pacman->animation_frame + delta_time / 100 < 4)
    {
        pacman->animation_frame += delta_time / 100;
    }
    else
    {
        pacman->animation_frame = 0;
    }
}

void draw_pacman(SDL_Renderer *renderer, struct Pacman *pacman)
{
    // draw pacman
    if (pacman->x_speed > 0)
        renderImage(renderer, pacman->tiles[(int)pacman->animation_frame], pacman->x, pacman->y, BLOCK_SIZE, BLOCK_SIZE, 180);
    else if (pacman->x_speed < 0)
        renderImage(renderer, pacman->tiles[(int)pacman->animation_frame], pacman->x, pacman->y, BLOCK_SIZE, BLOCK_SIZE, 0);
    else if (pacman->y_speed > 0)
        renderImage(renderer, pacman->tiles[(int)pacman->animation_frame], pacman->x, pacman->y, BLOCK_SIZE, BLOCK_SIZE, 270);
    else
        renderImage(renderer, pacman->tiles[(int)pacman->animation_frame], pacman->x, pacman->y, BLOCK_SIZE, BLOCK_SIZE, 90);
}
#endif