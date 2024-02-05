# SDL_Pacman

Recreated pacman game by using C and SDL librurary

## Installation(Linux, Windows, Mac_INTEL)
Make sure that you have installed SDL2

## Installation(Mac_ARM)
For this project i've intalled by brew SDL_2, SDL_lSDL2_image, SDL2_ttf at path
```bash
/opt/homebrew/include/
```
to run this program delete original Makefile and rename MAC_M1_Makefile to Makefile and change path the libraries, then follow next commands
## Compile and run

Run the following command to compile main

```bash
make
```

Run the following command to run the compiled application

```bash
make run
```

Run the following command to delete result of compilation

```bash
make clean
```

## Gaming

You can move pacman by pressing and holding keyboard arrows.
On start of the game press any arrow, and game will immediately begin.
You able to play until you see victory, or loose screen
