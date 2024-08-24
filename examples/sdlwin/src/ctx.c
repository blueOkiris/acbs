// Implement graphical context functionality

#include <SDL.h>
#include <err.h>
#include <ctx.h>

#define WIN_TITLE           "SDL Example"
#define IMG_INIT_FLAGS      (IMG_INIT_PNG)

ctx_res_t ctx_init(void) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        return ERR(
            ctx_res_t,
            ((err_t) { .code = EC_FAILED_SDL_INIT, .extra_info = NULL })
        );
    }

    SDL_Window *win = SDL_CreateWindow(
        WIN_TITLE,
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN
    );
    if (!win) {
        return ERR(
            ctx_res_t,
            ((err_t) { .code = EC_FAILED_CREATE_WIN, .extra_info = NULL })
        );
    }

    SDL_Renderer *rndrr = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    if (!rndrr) {
        return ERR(
            ctx_res_t,
            ((err_t) { .code = EC_FAILED_CREATE_WIN, .extra_info = NULL })
        );
    }
    SDL_SetRenderDrawColor(rndrr, 0x00, 0x00, 0x00, 0xFF);

    if (!(IMG_Init(IMG_INIT_FLAGS) & IMG_INIT_FLAGS)) {
        return ERR(
            ctx_res_t,
            ((err_t) { .code = EC_FAILED_INIT_IMG, .extra_info = NULL })
        );
    }

    return OK(
        ctx_res_t,
        ((ctx_t) { .win = win, .rndrr = rndrr })
    );
}

void ctx_deinit(ctx_t *ctx) {
    SDL_DestroyRenderer(ctx->rndrr);
    ctx->rndrr = NULL;
    SDL_DestroyWindow(ctx->win);
    ctx->win = NULL;

    IMG_Quit();
    SDL_Quit();
}

