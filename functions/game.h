#ifndef Game_h
#define Game_h

#include "init.h"

SDL_Texture *loadImage(SDL_Renderer *renderer, const char *imagePath)
{
    // Load the image from the specified path using SDL_image
    SDL_Surface *surface = IMG_Load(imagePath);
    // Create a texture from the loaded surface
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    // Free the surface, as it's no longer needed
    SDL_FreeSurface(surface);

    return texture;
}

struct Wall *readMapFromFile(char *fileName, struct GameMap *game_map, SDL_Renderer *renderer)
{
    FILE *file = fopen(fileName, "r");

    if (file == NULL)
    {
        printf("Error opening file");
        exit(EXIT_FAILURE);
    }

    struct Wall *map = (struct Wall *)malloc((game_map->block_height * game_map->block_width) * sizeof(struct Wall));

    for (int i = 0; i < game_map->block_height; i++)
    {
        for (int j = 0; j < game_map->block_width; j++)
        {
            char c = fgetc(file);
            while (c == '\n')
            {
                c = fgetc(file);
            }
            if (c >= 'a' && c <= 'x')
            {
                char path[18] = {'.', '/', 't', 'i', 'l', 'e', 's', '/', 'M', 'a', 'p', '/', c, '.', 'p', 'n', 'g', '\0'};

                map[i * (game_map->block_width) + j].hitBox.x = j * BLOCK_SIZE;
                map[i * (game_map->block_width) + j].hitBox.y = i * BLOCK_SIZE + game_map->top_margin;
                map[i * (game_map->block_width) + j].hitBox.w = BLOCK_SIZE;
                map[i * (game_map->block_width) + j].hitBox.h = BLOCK_SIZE;
                map[i * (game_map->block_width) + j].x_block_coordinates = j;
                map[i * (game_map->block_width) + j].y_block_coordinates = i;
                map[i * (game_map->block_width) + j].type = c;
                map[i * (game_map->block_width) + j].texture = loadImage(renderer, path);
            }
            if (c == 'y')
            {
                game_map->point_amount++;
                map[i * (game_map->block_width) + j].hitBox.w = BLOCK_SIZE / 2;                                         // decrease size 2 times
                map[i * (game_map->block_width) + j].hitBox.h = BLOCK_SIZE / 2;                                         // decrease size 2 times
                map[i * (game_map->block_width) + j].hitBox.x = j * BLOCK_SIZE + BLOCK_SIZE / 4;                        // - decreased size divided by 2
                map[i * (game_map->block_width) + j].hitBox.y = i * BLOCK_SIZE + game_map->top_margin + BLOCK_SIZE / 4; // - decreased size divided by 2
                map[i * (game_map->block_width) + j].x_block_coordinates = j;
                map[i * (game_map->block_width) + j].y_block_coordinates = i;
                map[i * (game_map->block_width) + j].type = c;
                map[i * (game_map->block_width) + j].show = true;
            }
        }
    }
    fclose(file);
    return map;
}

void initPacmanTiles(SDL_Renderer *renderer, struct Pacman *pacman)
{
    for (int i = 0; i < 4; i++)
    {
        char path[21] = {'t', 'i', 'l', 'e', 's', '/', 'P', 'a', 'c', 'm', 'a', 'n', '/', (char)(i + '0'), '.', 'p', 'n', 'g', '\0'};
        pacman->tiles[i] = loadImage(renderer, path);
    }
}

#endif