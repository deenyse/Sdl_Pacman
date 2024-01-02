#ifndef Game_h
#define Game_h

#include "init.h"
#include "pacman.h"
#include "ghost.h"
void WindowGameInit(struct GameMap *game_map, struct Pacman *pacman, SDL_Window **window, SDL_Renderer **renderer)
{
    game_map->block_width = 28;
    game_map->block_height = 31;
    game_map->pixel_height = BLOCK_SIZE * game_map->block_height;
    game_map->pixel_width = BLOCK_SIZE * game_map->block_width;

    game_map->top_margin = BLOCK_SIZE * 3;
    game_map->bottom_margin = BLOCK_SIZE * 2;
    game_map->point_amount = 0;
    game_map->collected_point_amount = 0;
    game_map->score = 0;
    initializePacman(pacman);

    SDL_Init(SDL_INIT_VIDEO);

    *window = SDL_CreateWindow(
        "SDL Pac-Man",
        10,
        10,
        game_map->pixel_width,
        game_map->pixel_height + game_map->top_margin + game_map->bottom_margin,
        SDL_WINDOW_SHOWN);

    if (*window == NULL)
    {
        printf("SDL_CreateWindow failed: %s\n", SDL_GetError());
        SDL_Quit();
        exit(1);
    }

    if (TTF_Init() == -1)
    {
        printf("SDL_ttf could not initialize! SDL_Error: %s\n", TTF_GetError());
        SDL_Quit();
        exit(0);
    }

    *renderer = SDL_CreateRenderer(
        *window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (*renderer == NULL)
    {
        printf("SDL_CreateRenderer failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(*window);
        SDL_Quit();
        exit(1);
    }
}

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
            if (c >= 'a' && c <= 'y')
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
            else if (c == 'z')
            {
                game_map->point_amount++;
                map[i * (game_map->block_width) + j].hitBox.w = BLOCK_SIZE / 3;
                map[i * (game_map->block_width) + j].hitBox.h = BLOCK_SIZE / 3;
                map[i * (game_map->block_width) + j].hitBox.x = j * BLOCK_SIZE + BLOCK_SIZE / 3;
                map[i * (game_map->block_width) + j].hitBox.y = i * BLOCK_SIZE + game_map->top_margin + BLOCK_SIZE / 3;
                map[i * (game_map->block_width) + j].x_block_coordinates = j;
                map[i * (game_map->block_width) + j].y_block_coordinates = i;
                map[i * (game_map->block_width) + j].type = c;
                map[i * (game_map->block_width) + j].show = true;
            }
            else if (c == 'Z')
            {
                game_map->point_amount++;
                map[i * (game_map->block_width) + j].hitBox.w = BLOCK_SIZE / 2;
                map[i * (game_map->block_width) + j].hitBox.h = BLOCK_SIZE / 2;
                map[i * (game_map->block_width) + j].hitBox.x = j * BLOCK_SIZE + BLOCK_SIZE / 4;
                map[i * (game_map->block_width) + j].hitBox.y = i * BLOCK_SIZE + game_map->top_margin + BLOCK_SIZE / 4;
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

void ImageInit(struct Pacman *pacman, struct Wall **map, SDL_Renderer *renderer, struct GameMap *game_map)
{
    *map = readMapFromFile("map.txt", game_map, renderer);
    initPacmanTiles(renderer, pacman);
}

char **readIntegersFromFile(const char *filePath)
{
    FILE *file = fopen(filePath, "r");
    if (file == NULL)
    {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    char **integerStrings = (char **)malloc(3 * sizeof(char *));
    if (integerStrings == NULL)
    {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < 3; ++i)
    {

        int number;
        if (fscanf(file, "%d", &number) != 1)
        {
            number = 0;
        }

        int stringSize = snprintf(NULL, 0, "%d", number);
        integerStrings[i] = (char *)malloc((stringSize + 1) * sizeof(char));
        if (integerStrings[i] == NULL)
        {
            perror("Memory allocation error");
            exit(EXIT_FAILURE);
        }
        snprintf(integerStrings[i], stringSize + 1, "%d", number);
    }

    fclose(file);

    return integerStrings;
}
void insertNumber(char **array, int size, int num, const char *filename)
{
    int i;
    char numStr[10];
    snprintf(numStr, sizeof(numStr), "%d", num);
    for (i = 0; i < size; i++)
    {
        if (atoi(array[i]) < num)
        {
            break;
        }
    }
    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }
    for (int j = 0; j < size; j++)
    {
        if (j == i)
        {
            fprintf(file, "%s\n", numStr);
        }
        fprintf(file, "%s\n", array[j]);
    }
    fclose(file);
}

bool creatureIntersection(struct Pacman *pacman, struct Ghost *ghost)
{
    return (pacman->x_block_cordinates == ghost->x_block_cordinates && pacman->y_block_cordinates == ghost->y_block_cordinates);
}

void intersectionMechanik(struct Pacman *pacman, struct Ghost *redGhost, struct Ghost *pinkGhost, struct Ghost *blueGhost, struct Ghost *orangeGhost, struct GameMap *game_map, int game_state)
{
    if (pacman->isKilling)
    {
        if (creatureIntersection(pacman, redGhost))
        {
            game_map->score += 200;
            initRedGhost(redGhost);
            SDL_AddTimer(2000, ghostRelease, redGhost);
        }
        else if (creatureIntersection(pacman, pinkGhost))
        {
            game_map->score += 200;
            initPinkGhost(pinkGhost);
            SDL_AddTimer(2000, ghostRelease, pinkGhost);
        }
        else if (creatureIntersection(pacman, blueGhost))
        {
            game_map->score += 200;
            initBlueGhost(blueGhost);
            SDL_AddTimer(2000, ghostRelease, blueGhost);
        }
        else if (creatureIntersection(pacman, orangeGhost))
        {
            game_map->score += 200;
            initOrangeGhost(orangeGhost);
            SDL_AddTimer(2000, ghostRelease, orangeGhost);
        }
    }
    else
    {
        if (creatureIntersection(pacman, redGhost) || creatureIntersection(pacman, pinkGhost) || creatureIntersection(pacman, blueGhost) || creatureIntersection(pacman, orangeGhost))
        {
            pacman->lives--;
            pacman->x = 14 * BLOCK_SIZE - BLOCK_SIZE / 2;
            pacman->y = 20 * BLOCK_SIZE;
            pacman->x_speed = 0;
            pacman->y_speed = 0;
            if (game_state >= 1)
            {
                ghostRelease(0, redGhost);
            }
            if (game_state >= 2)
            {
                ghostRelease(0, pinkGhost);
            }
            if (game_state >= 3)
            {
                ghostRelease(0, blueGhost);
            }
            if (game_state >= 4)
            {
                ghostRelease(0, orangeGhost);
            }
        }
    }
}
#endif