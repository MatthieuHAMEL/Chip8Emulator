#pragma once
#include <SDL.h>

constexpr unsigned NB_KEYS = 16;

constexpr uint32_t EMU_KEY_TO_SDL_KEY[16] =
    {
    SDL_SCANCODE_Q,
    SDL_SCANCODE_W,
    SDL_SCANCODE_E,
    SDL_SCANCODE_R,
    SDL_SCANCODE_T,
    SDL_SCANCODE_Y,
    SDL_SCANCODE_U,
    SDL_SCANCODE_I,
    SDL_SCANCODE_O,
    SDL_SCANCODE_P,
    SDL_SCANCODE_A,
    SDL_SCANCODE_S,
    SDL_SCANCODE_D,
    SDL_SCANCODE_F,
    SDL_SCANCODE_G,
    SDL_SCANCODE_H
    };

struct Keyboard
	{
    bool sdl_keys[SDL_NUM_SCANCODES] = {};
    bool console_keys[NB_KEYS] = {};
	};

