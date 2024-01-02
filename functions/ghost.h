#include "init.h"
#include "pacman.h"

void initRedGhost(struct Ghost *ghost)
{
    ghost->x_block_cordinates = 13;
    ghost->y_block_cordinates = 11;
    ghost->x = ghost->x_block_cordinates * BLOCK_SIZE;
    ghost->y = (ghost->y_block_cordinates + 3) * BLOCK_SIZE;

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
void initPinkGhost(struct Ghost *ghost)
{
    ghost->x_block_cordinates = 13;
    ghost->y_block_cordinates = 14;
    ghost->x = ghost->x_block_cordinates * BLOCK_SIZE;
    ghost->y = (ghost->y_block_cordinates + 3) * BLOCK_SIZE;

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
void initBlueGhost(struct Ghost *ghost)
{
    ghost->x_block_cordinates = 14;
    ghost->y_block_cordinates = 14;
    ghost->x = ghost->x_block_cordinates * BLOCK_SIZE;
    ghost->y = (ghost->y_block_cordinates + 3) * BLOCK_SIZE;

    ghost->moovement_speed = MOVEMENT_SPEED * 0.75;
    ghost->x_speed = ghost->moovement_speed;
    ghost->y_speed = 0;

    ghost->animation_frame = 0;
    ghost->tiles[0] = NULL;
    ghost->tiles[1] = NULL;
    ghost->tiles[2] = NULL;
    ghost->tiles[3] = NULL;

    ghost->character = 'b';
}
void initOrangeGhost(struct Ghost *ghost)
{
    ghost->x_block_cordinates = 15;
    ghost->y_block_cordinates = 14;
    ghost->x = ghost->x_block_cordinates * BLOCK_SIZE;
    ghost->y = (ghost->y_block_cordinates + 3) * BLOCK_SIZE;

    ghost->moovement_speed = MOVEMENT_SPEED * 0.75;
    ghost->x_speed = ghost->moovement_speed;
    ghost->y_speed = 0;

    ghost->animation_frame = 0;
    ghost->tiles[0] = NULL;
    ghost->tiles[1] = NULL;
    ghost->tiles[2] = NULL;
    ghost->tiles[3] = NULL;

    ghost->character = 'o';
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

    if (ghost->x < 0)
        ghost->x = game_map->pixel_width - BLOCK_SIZE;
    else if (ghost->x + BLOCK_SIZE > game_map->pixel_width)
        ghost->x = 0;
    else if (ghost->x_block_cordinates > 0 && ghost->x_block_cordinates < game_map->block_width - 1 && ghost->y_block_cordinates >= 0 && ghost->y_block_cordinates < game_map->block_height)
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
void mooveBlueGhost(struct Ghost *ghost, struct GameMap *game_map, double delta_time, struct Wall *walls, struct Pacman *pacman, struct Ghost *red_ghost)
{
    if (pacman->y_speed < 0)
    {
        ghostMove(ghost, game_map, delta_time, walls, 2 * (pacman->x_block_cordinates - 2) - red_ghost->x_block_cordinates, 2 * (pacman->y_block_cordinates - 2) - red_ghost->y_block_cordinates);
    }
    else if (pacman->y_speed > 0)
    {
        ghostMove(ghost, game_map, delta_time, walls, 2 * (pacman->x_block_cordinates) - red_ghost->x_block_cordinates, 2 * (pacman->y_block_cordinates + 2) - red_ghost->y_block_cordinates);
    }
    else if (pacman->x_speed < 0)
    {
        ghostMove(ghost, game_map, delta_time, walls, 2 * (pacman->x_block_cordinates - 2) - red_ghost->x_block_cordinates, 2 * (pacman->y_block_cordinates) - red_ghost->y_block_cordinates);
    }
    else if (pacman->x_speed > 0)
    {
        ghostMove(ghost, game_map, delta_time, walls, 2 * (pacman->x_block_cordinates + 2) - red_ghost->x_block_cordinates, 2 * (pacman->y_block_cordinates) - red_ghost->y_block_cordinates);
    }
}
void mooveOrangeGhost(struct Ghost *ghost, struct GameMap *game_map, double delta_time, struct Wall *walls, struct Pacman *pacman)
{
    if (sqrt(pow(ghost->x_block_cordinates - pacman->x_block_cordinates, 2) + pow(ghost->y_block_cordinates - pacman->y_block_cordinates, 2)) > 8)
    {
        ghostMove(ghost, game_map, delta_time, walls, pacman->x_block_cordinates, pacman->y_block_cordinates);
    }
    else
    {
        ghostMove(ghost, game_map, delta_time, walls, 0, game_map->block_height);
    }
}

void ghostRelease(struct Ghost *ghost)
{
    ghost->x = 13 * BLOCK_SIZE;
    ghost->y = 14 * BLOCK_SIZE;
}