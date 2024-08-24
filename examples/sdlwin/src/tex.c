// Texture loading functionality

#include <stdlib.h>
#include <SDL.h>
#include <SDL_image.h>
#include <err.h>
#include <tex.h>

tex_res_t tex_load(SDL_Renderer *rndrr, const char *path) {
    SDL_Surface *loaded_sfc = IMG_Load(path);
    if (!loaded_sfc) {
        return ERR(
            tex_res_t,
            ((err_t) { .code = EC_FAILED_LOAD_IMG, .extra_info = (char *) path })
        );
    }
    int width = loaded_sfc->w;
    int height = loaded_sfc->h;

    SDL_Texture *new_tex = SDL_CreateTextureFromSurface(rndrr, loaded_sfc);
    if (!new_tex) {
        return ERR(
            tex_res_t,
            ((err_t) { .code = EC_FAILED_SFC_TO_TEX, .extra_info = (char *) path })
        );
    }

    SDL_FreeSurface(loaded_sfc);
    return OK(
        tex_res_t,
        ((tex_t) { .tex = new_tex, .width = width, .height = height })
    );
}

void tex_draw(SDL_Renderer *rndrr, tex_t *tex, int x, int y, int width, int height) {
    SDL_Rect src_rect = (SDL_Rect) {
        .x = 0,
        .y = 0,
        .w = tex->width,
        .h = tex->height
    };
    SDL_Rect dest_rect = (SDL_Rect) {
        .x = x,
        .y = y,
        .w = width,
        .h = height
    };
    SDL_RenderCopy(rndrr, tex->tex, &src_rect, &dest_rect);
}

void tex_cleanup(tex_t *tex) {
    SDL_DestroyTexture(tex->tex);
    tex->tex = NULL;
}

