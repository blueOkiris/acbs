// Texture API

#pragma once

#include <SDL.h>

typedef struct {
    SDL_Texture *tex;
    int width;
    int height;
} tex_t;

RESULT(tex_t, err_t, tex_res_t)

tex_res_t tex_load(SDL_Renderer *rndrr, const char *path);
void tex_draw(SDL_Renderer *rndrr, tex_t *tex, int x, int y, int width, int height);
void tex_cleanup(tex_t *tex);

