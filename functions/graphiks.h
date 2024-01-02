#ifndef Graphics_h
#define Graphics_h

#include "init.h"

#include <SDL2/SDL.h>

void renderImage(SDL_Renderer *renderer, SDL_Texture *texture, int to_x, int to_y, int height, int width, double angle)
{
    // Create source and destination rectangles
    SDL_Rect srcRect = {0, 0, width, height};        // Set width and height to the dimensions of your image
    SDL_Rect destRect = {to_x, to_y, width, height}; // Set width and height to the dimensions you want at the destination

    // Set the rotation center (optional)
    SDL_Point center = {width / 2, height / 2};

    // Render the texture with rotation
    SDL_RenderCopyEx(renderer, texture, &srcRect, &destRect, angle, &center, SDL_FLIP_NONE);
}

void printText(SDL_Renderer *renderer, const char *text, int x, int y, int fontSize)
{
    TTF_Font *font = TTF_OpenFont("CrackMan.TTF", fontSize);
    if (!font)
    {
        printf("TTF_OpenFont: %s\n", TTF_GetError());
        return;
    }

    SDL_Color textColor = {255, 255, 255, 255}; // White color

    SDL_Surface *surface = TTF_RenderText_Solid(font, text, textColor);
    if (!surface)
    {
        printf("TTF_RenderText_Solid: %s\n", TTF_GetError());
        TTF_CloseFont(font);
        return;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture)
    {
        printf("SDL_CreateTextureFromSurface: %s\n", SDL_GetError());
        SDL_FreeSurface(surface);
        TTF_CloseFont(font);
        return;
    }

    SDL_Rect destinationRect = {x, y, surface->w, surface->h};

    SDL_RenderCopy(renderer, texture, NULL, &destinationRect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
    TTF_CloseFont(font);
}

void renderMap(SDL_Renderer *renderer, struct Wall *map, int rows, int cols)
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            if (map[i * cols + j].type >= 'a' && map[i * cols + j].type <= 'y')
            {
                renderImage(renderer, map[i * cols + j].texture, map[i * cols + j].hitBox.x, map[i * cols + j].hitBox.y, BLOCK_SIZE, BLOCK_SIZE, 0);
                // SDL_SetRenderDrawColor(renderer, 0, 120, 255, 255); // block color
                // SDL_RenderFillRect(renderer, &map[i * cols + j].hitBox);
            }
            if (map[i * cols + j].type == 'z' && map[i * cols + j].show)
            {

                SDL_SetRenderDrawColor(renderer, 241, 188, 179, 255);
                SDL_RenderFillRect(renderer, &map[i * cols + j].hitBox);
            }
        }
    }
}

void drawPacmanLives(SDL_Renderer *renderer, struct Pacman *pacman)
{
    int height = (int)(BLOCK_SIZE * 1.5);
    for (int i = 0; i < pacman->lives; i++)
    {
        renderImage(renderer, pacman->tiles[0], i * height + 4 * i + 10, 34 * BLOCK_SIZE, height, height, 0);
    }
}

void mapUxDraw(SDL_Renderer *renderer, struct Wall *map, struct Pacman *pacman, struct GameMap *game_map)
{
    // draw background
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // draw mapux
    renderMap(renderer, map, game_map->block_height, game_map->block_width);

    // draw pacman position map(greeen)
    // SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    // SDL_Rect pacman_box1 = {.x = pacman->x_block_cordinates * BLOCK_SIZE, .y = pacman->y_block_cordinates * BLOCK_SIZE + game_map->top_margin, .w = BLOCK_SIZE, .h = BLOCK_SIZE};
    // SDL_RenderFillRect(renderer, &pacman_box1);

    // draw pacman image hitbox
    // SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    //  SDL_Rect pacman_box = {.x = pacman->x, .y = pacman->y, .w = BLOCK_SIZE, .h = BLOCK_SIZE};
    //  SDL_RenderFillRect(renderer, &pacman_box);

    // draw pacman
    if (pacman->x_speed > 0)
        renderImage(renderer, pacman->tiles[(int)pacman->animation_frame], pacman->x, pacman->y, BLOCK_SIZE, BLOCK_SIZE, 180);
    else if (pacman->x_speed < 0)
        renderImage(renderer, pacman->tiles[(int)pacman->animation_frame], pacman->x, pacman->y, BLOCK_SIZE, BLOCK_SIZE, 0);
    else if (pacman->y_speed > 0)
        renderImage(renderer, pacman->tiles[(int)pacman->animation_frame], pacman->x, pacman->y, BLOCK_SIZE, BLOCK_SIZE, 270);
    else
        renderImage(renderer, pacman->tiles[(int)pacman->animation_frame], pacman->x, pacman->y, BLOCK_SIZE, BLOCK_SIZE, 90);
    // print score
    printText(renderer, "High score:1000", 0, 0, 36);
    printText(renderer, "Score:2000", 0, 36, 36);

    // print lives
    drawPacmanLives(renderer, pacman);
}

#endif