#ifndef Graphics_h
#define Graphics_h

#include "init.h"

#include <SDL2/SDL.h>

char *intToStringWithScore(int number)
{
    // Determine the length of the string representation
    int length = snprintf(NULL, 0, "score:%d", number);

    // Allocate memory for the string representation
    char *result = (char *)malloc(length + 1); // +1 for the null terminator

    // Convert the integer to a string with "score:" prefix
    snprintf(result, length + 1, "score:%d", number);

    return result;
}

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
            else if (map[i * cols + j].type == 'z' && map[i * cols + j].show)
            {

                SDL_SetRenderDrawColor(renderer, 241, 188, 179, 255);
                SDL_RenderFillRect(renderer, &map[i * cols + j].hitBox);
            }
            else if (map[i * cols + j].type == 'Z' && map[i * cols + j].show)
            {
                SDL_SetRenderDrawColor(renderer, 62, 180, 137, 255);
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

void mapUxDraw(SDL_Renderer *renderer, struct Wall *map, struct Pacman *pacman, struct GameMap *game_map, char **top_positions)
{
    // draw background
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // draw mapux
    renderMap(renderer, map, game_map->block_height, game_map->block_width);

    // print score
    char *currentScore = intToStringWithScore(game_map->score);
    char highScoreString[strlen("High score:") + strlen(top_positions[0]) + 1];
    strcpy(highScoreString, "High score:");
    strcat(highScoreString, top_positions[0]);

    printText(renderer, highScoreString, 0, 0, 36);
    printText(renderer, currentScore, 0, 36, 36);
    free(currentScore);

    // print lives
    drawPacmanLives(renderer, pacman);
}

void drawGhost(struct SDL_Renderer *renderer, struct Ghost *ghost)
{
    // draw ghost position map(greeen)
    // SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    // SDL_Rect ghost_box1 = {.x = ghost->x_block_cordinates * BLOCK_SIZE, .y = ghost->y_block_cordinates * BLOCK_SIZE + game_map->top_margin, .w = BLOCK_SIZE, .h = BLOCK_SIZE};
    // SDL_RenderFillRect(renderer, &ghost_box1);
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
    else if (ghost->character == 'b')
    {
        SDL_SetRenderDrawColor(renderer, 135, 206, 250, 255);
        SDL_Rect ghost_box = {.x = ghost->x, .y = ghost->y, .w = BLOCK_SIZE, .h = BLOCK_SIZE};
        SDL_RenderFillRect(renderer, &ghost_box);
    }
    else if (ghost->character == 'o')
    {
        SDL_SetRenderDrawColor(renderer, 255, 165, 0, 255);
        SDL_Rect ghost_box = {.x = ghost->x, .y = ghost->y, .w = BLOCK_SIZE, .h = BLOCK_SIZE};
        SDL_RenderFillRect(renderer, &ghost_box);
    }
}
#endif