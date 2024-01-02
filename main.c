#include "functions/game.h"
#include "functions/graphiks.h"
#include "functions/pacman.h"
#include "functions/init.h"
#include "functions/ghost.h"

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

    // Convert the integer to string
    char numStr[10];
    sprintf(numStr, "%d", num);

    // Find the correct position to insert the number
    for (i = 0; i < size; i++)
    {
        if (atoi(array[i]) < num)
        {
            break;
        }
    }

    // Open the file for writing
    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    // Write the modified array to the file
    for (int j = 0; j < size; j++)
    {
        if (j == i)
        {
            // Insert the new number at the correct position
            fprintf(file, "%s\n", numStr);
        }
        fprintf(file, "%s\n", array[j]);
    }

    // Close the file
    fclose(file);
}

void drawStartMenu(struct SDL_Renderer *renderer, char **top_positions)
{
    printText(renderer, "Pacman", 0, 0, 141);
    printText(renderer, "Top 3:", 260, 200, 40);

    for (int i = 0; i < 3; ++i)
    {
        printText(renderer, top_positions[i], 260, 240 + 40 * i, 40);
    }
    printText(renderer, "press any arrow to start", 5, 800, 40);
}

void drawVictoryScreen(struct SDL_Renderer *renderer, char **top_positions) {}
Uint32 ghostRelease(Uint32 interval, void *param)
{
    struct Ghost *ghost = (struct Ghost *)param;
    ghost->x = 13 * BLOCK_SIZE;
    ghost->y = 14 * BLOCK_SIZE;
    ghost->isActive = true;
    return 0;
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
int main()
{
    struct Ghost redGhost;
    struct Ghost pinkGhost;
    struct Ghost blueGhost;
    struct Ghost orangeGhost;
    struct GameMap game_map;
    struct Pacman pacman = {.lives = 4};
    struct Wall *map = NULL;

    initRedGhost(&redGhost);
    initPinkGhost(&pinkGhost);
    initBlueGhost(&blueGhost);
    initOrangeGhost(&orangeGhost);
    ghostRelease(0, &redGhost);
    redGhost.isActive = false;

    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    WindowGameInit(&game_map, &pacman, &window, &renderer);

    ImageInit(&pacman, &map, renderer, &game_map);
    char **top_positions = readIntegersFromFile("results.txt");

    int game_state = 0;
    bool running = true;
    SDL_Event event = {};

    Uint64 delta_now = SDL_GetPerformanceCounter();
    Uint64 delta_last = 0;
    double delta_time = 0;

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

        delta_last = delta_now;
        delta_now = SDL_GetPerformanceCounter();
        delta_time = (double)((delta_now - delta_last) * 1000 / (double)SDL_GetPerformanceFrequency());

        if (game_state == 0)
        {
            drawStartMenu(renderer, top_positions);
            draw_pacman(renderer, &pacman);
        }
        else if (game_state >= 1 && game_state <= 4)
        {
            pacmanMoove(&pacman, delta_time, &game_map, map);
            pacman_animate(&pacman, delta_time);
            point_collector(&pacman, &game_map, map);
            mooveRedGhost(&redGhost, &game_map, delta_time, map, &pacman);
            moovePinkGhost(&pinkGhost, &game_map, delta_time, map, &pacman);
            mooveBlueGhost(&blueGhost, &game_map, delta_time, map, &pacman, &redGhost);
            mooveOrangeGhost(&orangeGhost, &game_map, delta_time, map, &pacman);
            intersectionMechanik(&pacman, &redGhost, &pinkGhost, &blueGhost, &orangeGhost, &game_map, game_state);

            mapUxDraw(renderer, map, &pacman, &game_map, top_positions);
            draw_pacman(renderer, &pacman);
            drawGhost(renderer, &redGhost, &game_map);
            drawGhost(renderer, &pinkGhost, &game_map);
            drawGhost(renderer, &blueGhost, &game_map);
            drawGhost(renderer, &orangeGhost, &game_map);
        }
        else if (game_state == 5)
        {
            drawVictoryScreen(renderer, top_positions);
        }
        else if (game_state == 6)
        {
            printf("you loose");
            // drawGameOverScreen(renderer, top_positions);
        }
        if (game_state == 0 && (key_left_pressed + key_right_pressed + key_up_pressed + key_down_pressed) > 0)
        {
            game_state = 1;
            redGhost.isActive = true;
        }
        if (game_map.collected_point_amount == (int)(game_map.point_amount * 0.2))
        {
            game_state = 2;
            ghostRelease(0, &pinkGhost);
        }
        if (game_map.collected_point_amount == (int)(game_map.point_amount * 0.4))
        {
            game_state = 3;
            ghostRelease(0, &blueGhost);
        }

        if (game_map.collected_point_amount == (int)(game_map.point_amount * 0.6))
        {
            game_state = 4;
            ghostRelease(0, &orangeGhost);
        }
        if (game_map.collected_point_amount >= game_map.point_amount && game_state != 0)
        {
            insertNumber(top_positions, 3, game_map.score, "results.txt");
            game_state = 5;
        }
        else if (pacman.lives <= 0 && game_state != 0)
        {
            insertNumber(top_positions, 3, game_map.score, "results.txt");
            game_state = 6;
        }

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    free(map);

    return 0;
}