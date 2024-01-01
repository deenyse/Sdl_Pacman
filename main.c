#include "functions/game.h"
#include "functions/graphiks.h"
#include "functions/pacman.h"
#include "functions/init.h"

int main()
{
    struct GameMap game_map = {
        .block_width = 28,
        .block_height = 31,
        .pixel_height = BLOCK_SIZE * game_map.block_height,
        .pixel_width = BLOCK_SIZE * game_map.block_width,
        .top_margin = BLOCK_SIZE * 3,
        .bottom_margin = BLOCK_SIZE * 2,
        .point_amount = 0,
        .collected_point_amount = 0,
    };
    struct Pacman pacman = {
        .x = 7 * BLOCK_SIZE + game_map.bottom_margin,
        .y = 17 * BLOCK_SIZE,
        .x_speed = 0,
        .y_speed = 0,
        .moovement_speed = MOVEMENT_SPEED,
        .animation_frame = 0,
        .lives = 4};
    // Inicializace SDL
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow(
        "SDL Pac-Man",                                                        // Titulek okna
        10,                                                                   // Souřadnice x
        10,                                                                   // Souřadnice y
        game_map.pixel_width,                                                 // Šířka
        game_map.pixel_height + game_map.top_margin + game_map.bottom_margin, // Výška
        SDL_WINDOW_SHOWN                                                      // Okno se má po vytvoření rovnou zobrazit
    );
    // inicializace Font
    if (TTF_Init() == -1)
    {
        printf("SDL_ttf could not initialize! SDL_Error: %s\n", TTF_GetError());
        SDL_Quit();
        return 0;
    }
    //
    // Creating renderer window
    SDL_Renderer *renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    SDL_Event event = {};
    bool running = 1;

    Uint64 delta_now = SDL_GetPerformanceCounter();
    Uint64 delta_last = 0;
    double delta_time = 0;

    // image init
    struct Wall *map = readMapFromFile("map.txt", &game_map, renderer);
    initPacmanTiles(renderer, &pacman);
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