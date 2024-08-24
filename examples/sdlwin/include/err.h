// Error handling for the project

#pragma once

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_image.h>

#define RESULT(ok_t, err_t, name) \
    typedef struct { \
        const bool is_err; \
        union { \
            const ok_t ok; \
            const err_t err; \
        }; \
    } name;
#define OK(res_t, val)  ((res_t) { .is_err = false, { .ok = val } })
#define ERR(res_t, val) ((res_t) { .is_err = true, { .err = val } })

#define OPTION(t, name) \
    typedef struct { \
        const bool is_none; \
        union { \
            const t value; \
            const struct {} none; \
        }; \
    } name;
#define SOME(t, val)    ((t) { .is_none = false, { .value = val } })
#define NONE(t)         ((t) { .is_none = true, { .none = {} } })

typedef enum {
    EC_FAILED_LOAD_IMG,
    EC_FAILED_SFC_TO_TEX,
    EC_FAILED_SDL_INIT,
    EC_FAILED_CREATE_WIN,
    EC_FAILED_INIT_IMG
} err_code_t;

typedef struct {
    err_code_t code;
    char *extra_info;
} err_t;

inline void print_err(err_t e) {
    switch (e.code) {
        case EC_FAILED_LOAD_IMG:
            fprintf(
                stderr,
                "Error loading file '%s'. IMG Err: %s\n",
                e.extra_info, IMG_GetError()
            );
            break;
        case EC_FAILED_SFC_TO_TEX:
            fprintf(
                stderr,
                "Failed to convert surface to texture for image '%s'. SDL Err: %s\n",
                e.extra_info, SDL_GetError()
            );
            break;
        case EC_FAILED_SDL_INIT:
            fprintf(stderr, "Failed to initialize SDL. SDL Error: %s\n", SDL_GetError());
            break;
        case EC_FAILED_CREATE_WIN:
            fprintf(stderr, "Failed to create SDL Window! SDL Error: %s\n", SDL_GetError());
            break;
        case EC_FAILED_INIT_IMG:
            fprintf(stderr, "Failed to initialize SDL Image. Img Error: %s\n", IMG_GetError());
            break;
    }
}

