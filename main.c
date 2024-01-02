#include "functions/game.h"
#include "functions/graphiks.h"
#include "functions/pacman.h"
#include "functions/init.h"
#include "functions/ghost.h"

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

    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    WindowGameInit(&game_map, &pacman, &window, &renderer);

    ImageInit(&pacman, &map, renderer, &game_map);

    int game_state = 0;
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
    //.
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

        if (game_map.collected_point_amount < game_map.point_amount)
        {

            mapUxDraw(renderer, map, &pacman, &game_map);
            drawGhost(renderer, &redGhost, &game_map);
            drawGhost(renderer, &pinkGhost, &game_map);
            drawGhost(renderer, &blueGhost, &game_map);
            drawGhost(renderer, &orangeGhost, &game_map);

            if (game_state >= 1)
            {
                pacmanMoove(&pacman, delta_time, &game_map, map);
                pacman_animate(&pacman, delta_time);
                point_collector(&pacman, &game_map, map);
                mooveRedGhost(&redGhost, &game_map, delta_time, map, &pacman);
                moovePinkGhost(&pinkGhost, &game_map, delta_time, map, &pacman);
                mooveBlueGhost(&blueGhost, &game_map, delta_time, map, &pacman, &redGhost);
                mooveOrangeGhost(&orangeGhost, &game_map, delta_time, map, &pacman);
            }
            if (game_state == 0 && (key_left_pressed + key_right_pressed + key_up_pressed + key_down_pressed) > 0)
            {
                game_state = 1;
                redGhost.isActive = true;
            }
            if (game_map.collected_point_amount == (int)(game_map.point_amount * 0.2))
            {
                game_state = 2;
                pinkGhost.isActive = true;
                ghostRelease(&pinkGhost);
            }
            if (game_map.collected_point_amount == (int)(game_map.point_amount * 0.4))
            {
                game_state = 3;
                blueGhost.isActive = true;
                ghostRelease(&blueGhost);
            }

            if (game_map.collected_point_amount == (int)(game_map.point_amount * 0.6))
            {

                game_state = 4;
                orangeGhost.isActive = true;
                ghostRelease(&orangeGhost);
            }
        }
        else
        {
            printf("You won\n");
        }
        // update renderer
        SDL_RenderPresent(renderer);
    }
    ////////.......///////////////
    // Uvolnění prostředků
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    free(map);

    return 0;
}