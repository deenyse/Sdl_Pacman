#ifndef Structures_h
#define Structures_h

#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#define BLOCK_SIZE 24
#define MOVEMENT_SPEED 0.24 // 0.24

struct Pacman
{
    double x;
    double y;
    int x_block_cordinates;
    int y_block_cordinates;

    double x_speed;
    double y_speed;
    double moovement_speed;

    double animation_frame;
    SDL_Texture *tiles[4];

    int lives;
    bool isKilling;
    int timer_amount;

} Pacman;
struct Wall
{
    SDL_Rect hitBox;
    int x_block_coordinates;
    int y_block_coordinates;
    char type;
    SDL_Texture *texture;
    bool show;
} Wall;
struct Point
{
    SDL_Rect hitBox;
    int x_block_coordinates;
    int y_block_coordinates;
    bool taken;
} Point;
struct GameMap
{
    int block_width;
    int block_height;
    int pixel_height;
    int pixel_width;

    int top_pixel_margin;
    int top_margin;
    int bottom_margin;

    int point_amount;
    int collected_point_amount;

} GameMap;
struct Ghost
{
    double x;
    double y;
    int x_block_cordinates;
    int y_block_cordinates;

    double x_speed;
    double y_speed;
    double moovement_speed;

    double animation_frame;
    SDL_Texture *tiles[4];

    char character;
    bool isActive;
} Ghost;
#endif