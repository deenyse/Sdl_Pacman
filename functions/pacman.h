#ifndef Pacman_h
#define Pacman_h

#include "init.h"

bool isAbleToGo(struct Wall *map, struct GameMap *game_map, int to_x, int to_y)
{
    if (to_x < -1 || to_x > game_map->block_width)
        return true;
    if (map[to_y * game_map->block_width + to_x].type >= 'a' && map[to_y * game_map->block_width + to_x].type <= 'x')
        return false;
    return true;
}

void pacmanMoove(struct Pacman *pacman, double delta_time, struct GameMap *map, struct Wall *walls)
{
    // moove pacman hitbox
    pacman->x = round(pacman->x + pacman->x_speed * delta_time);
    pacman->y = round(pacman->y + pacman->y_speed * delta_time);

    // pacman->isMooving = true; // for animation

    // over border check
    if (pacman->x_block_cordinates < -1)
        pacman->x = map->pixel_width;
    else if (pacman->x_block_cordinates > map->block_width)
        pacman->x = -1 * BLOCK_SIZE;
    // wall and pacman intersection
    else if (pacman->x_block_cordinates >= 0 && pacman->x_block_cordinates < map->block_width && pacman->y_block_cordinates >= 0 && pacman->y_block_cordinates < map->block_height)
    {
        if (pacman->x_speed > 0) // checks if pacman is in a wall // add general if x y is not out of border
        {
            if ((walls[pacman->y_block_cordinates * map->block_width + pacman->x_block_cordinates + 1].hitBox.x <= pacman->x + BLOCK_SIZE) && walls[pacman->y_block_cordinates * map->block_width + pacman->x_block_cordinates + 1].type >= 'a' && walls[pacman->y_block_cordinates * map->block_width + pacman->x_block_cordinates + 1].type <= 'x')
            {
                pacman->x = walls[pacman->y_block_cordinates * map->block_width + pacman->x_block_cordinates + 1].hitBox.x - BLOCK_SIZE;
            }
        }
        else if (pacman->x_speed < 0)
        {
            if ((walls[pacman->y_block_cordinates * map->block_width + pacman->x_block_cordinates - 1].hitBox.x >= pacman->x - BLOCK_SIZE) && walls[pacman->y_block_cordinates * map->block_width + pacman->x_block_cordinates - 1].type >= 'a' && walls[pacman->y_block_cordinates * map->block_width + pacman->x_block_cordinates - 1].type <= 'x')
            {
                pacman->x = walls[pacman->y_block_cordinates * map->block_width + pacman->x_block_cordinates - 1].hitBox.x + BLOCK_SIZE;
            }
        }
        else if (pacman->y_speed > 0)
        {
            if ((walls[(pacman->y_block_cordinates + 1) * map->block_width + pacman->x_block_cordinates].hitBox.y <= pacman->y + BLOCK_SIZE) && walls[(pacman->y_block_cordinates + 1) * map->block_width + pacman->x_block_cordinates].type >= 'a' && walls[(pacman->y_block_cordinates + 1) * map->block_width + pacman->x_block_cordinates].type <= 'x')
            {
                pacman->y = walls[(pacman->y_block_cordinates + 1) * map->block_width + pacman->x_block_cordinates].hitBox.y - BLOCK_SIZE;
            }
        }
        else if (pacman->y_speed < 0)
        {
            if ((walls[(pacman->y_block_cordinates - 1) * map->block_width + pacman->x_block_cordinates].hitBox.y >= pacman->y - BLOCK_SIZE) && walls[(pacman->y_block_cordinates - 1) * map->block_width + pacman->x_block_cordinates].type >= 'a' && walls[(pacman->y_block_cordinates - 1) * map->block_width + pacman->x_block_cordinates].type <= 'x')
            {
                pacman->y = walls[(pacman->y_block_cordinates - 1) * map->block_width + pacman->x_block_cordinates].hitBox.y + BLOCK_SIZE;
            }
        }
    }
    // calculation actual map-orientated pacman coordinates
    pacman->x_block_cordinates = round((pacman->x / BLOCK_SIZE));
    pacman->y_block_cordinates = round((pacman->y - map->top_margin) / BLOCK_SIZE);
}

void point_collector(struct Pacman *pacman, struct GameMap *map, struct Wall *walls)
{
    if (walls[pacman->y_block_cordinates * map->block_width + pacman->x_block_cordinates].show)
    {
        walls[pacman->y_block_cordinates * map->block_width + pacman->x_block_cordinates].show = false;
        map->collected_point_amount++;
        printf("collected %d points\n", map->collected_point_amount);
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
#endif