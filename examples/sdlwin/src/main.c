// Simple SDL project to draw an image to the screen

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>

#define SCREEN_WIDTH        640
#define SCREEN_HEIGHT       360

int main(int argc, char **argv) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *win = SDL_CreateWindow(
        "SDL Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN
    );
    if (!win) {
        fprintf(stderr, "Window could not be created! SDL Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Surface *sfc = SDL_GetWindowSurface(win);
    SDL_FillRect(sfc, NULL, SDL_MapRGB(sfc->format, 0xFF, 0xFF, 0xFF));
    SDL_UpdateWindowSurface(win);

    SDL_Event e;
    bool quit = false;
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_QUIT:
                    quit = true;
                    break;
                default:
                    break;
            }
        }
    }

    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}

