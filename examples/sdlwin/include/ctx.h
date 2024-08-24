// Create global window and renderer

#pragma once

#include <SDL.h>
#include <err.h>

#define SCREEN_WIDTH        640
#define SCREEN_HEIGHT       360

typedef struct {
    SDL_Window *win;
    SDL_Renderer *rndrr;
} ctx_t;

RESULT(ctx_t, err_t, ctx_res_t)

ctx_res_t ctx_init(void);
void ctx_deinit(ctx_t *ctx);

