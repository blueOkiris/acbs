// Simple SDL project to draw an image to the screen

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>
#include <err.h>
#include <tex.h>
#include <ctx.h>

#define SCREEN_WIDTH        640
#define SCREEN_HEIGHT       360

int main(int argc, char **argv) {
    ctx_res_t ctx_res = ctx_init();
    if (ctx_res.is_err) {
        print_err(ctx_res.err);
        return 1;
    }
    ctx_t ctx = ctx_res.ok;

    tex_res_t test_img_res = tex_load(ctx.rndrr, "examples/sdlwin/img/test-img.png");
    if (test_img_res.is_err) {
        print_err(test_img_res.err);
        return 1;
    }
    tex_t test_img = test_img_res.ok;

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

        SDL_RenderClear(ctx.rndrr);
        tex_draw(ctx.rndrr, &test_img, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
        SDL_RenderPresent(ctx.rndrr);
    }

    tex_cleanup(&test_img);
    ctx_deinit(&ctx);
    return 0;
}

