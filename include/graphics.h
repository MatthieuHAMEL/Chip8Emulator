#pragma once

#include "res.h"

#include <SDL.h>
#include <iostream>

namespace emu
	{
	// A pixel is a boolean (black or white). Screen resolution is 64x32. 
	constexpr size_t SCREEN_W = 64;
	constexpr size_t SCREEN_H = 32;
	constexpr size_t PIX_DIM = 16; // The "real" size of a chip8 pixel on our screen. 
    constexpr size_t WINDOW_W = SCREEN_W * PIX_DIM;
    constexpr size_t WINDOW_H = SCREEN_H * PIX_DIM;

	struct Screen
		{
		SDL_Window* window = nullptr;
		SDL_Renderer* renderer = nullptr;
		bool screen[SCREEN_W][SCREEN_H] = {};
		};

    inline Res init_screen(Screen& iScreen)
        {
        iScreen.window = SDL_CreateWindow(
            "My Emulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            WINDOW_W, WINDOW_H, SDL_WINDOW_SHOWN);

        if (!iScreen.window)
            {
            prompt_error_box("SDL_CreateWindow", SDL_GetError());
            return Rc::SDL_ERROR;
            }

        iScreen.renderer = SDL_CreateRenderer(iScreen.window, -1, SDL_RENDERER_ACCELERATED);
        if (!iScreen.renderer)
            {
            prompt_error_box("SDL_CreateRenderer", SDL_GetError());
            SDL_DestroyWindow(iScreen.window);
            return Rc::SDL_ERROR;
            }
        return Rc::OK;
        }
	}


