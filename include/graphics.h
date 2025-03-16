#pragma once

#include "res.h"

#include <SDL.h>
#include <iostream>

namespace emu
    {
    // A pixel is a boolean (black or white). Screen resolution is 64x32. 
    constexpr size_t SCREEN_W = 64;
    constexpr size_t SCREEN_H = 32;
    constexpr size_t PIX_DIM = 8; // The "real" size of a chip8 pixel on our screen. 
    constexpr size_t WINDOW_W = SCREEN_W * PIX_DIM;
    constexpr size_t WINDOW_H = SCREEN_H * PIX_DIM;

    struct Screen
        {
        SDL_Window* window = nullptr;
        SDL_Renderer* renderer = nullptr;
        bool pix_grid[SCREEN_W][SCREEN_H] = {};
        };

    inline Res init_screen(Screen& iScreen)
        {
        iScreen.window = SDL_CreateWindow(
            "Chip8 Emulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
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

    inline void clear_screen(Screen& iScreen)
        {
        memset(iScreen.pix_grid, 0, sizeof(iScreen.pix_grid)); // Sets everything to false
        }

    inline void destroy_screen(Screen& iScreen)
        {
        SDL_DestroyRenderer(iScreen.renderer);
        SDL_DestroyWindow(iScreen.window);
        }

    inline Res render_screen(Screen const& iScreen)
        {
        SDL_SetRenderDrawColor(iScreen.renderer, 0, 0, 0, 255); // black
        SDL_RenderClear(iScreen.renderer);
        SDL_SetRenderDrawColor(iScreen.renderer, 255, 255, 255, 255); // white
        SDL_Rect rect{ 0, 0, PIX_DIM, PIX_DIM };
        for (int i = 0; i < SCREEN_W; ++i)
            {
            for (int j = 0; j < SCREEN_H; ++j)
                {
                if (iScreen.pix_grid[i][j])
                    {
                    rect.x = i * PIX_DIM;
                    rect.y = j * PIX_DIM;
                    SDL_RenderFillRect(iScreen.renderer, &rect);
                    }
                }
            }
        SDL_RenderPresent(iScreen.renderer);
        return Rc::OK;
        }   

    inline Res init_SDL()
        {
        if (SDL_Init(SDL_INIT_VIDEO) < 0)
            {
            prompt_error_box("SDL_Init", SDL_GetError());
            return Rc::SDL_ERROR;
            }
        return Rc::OK;
        }

    inline void destroy_SDL()
        {
        SDL_Quit();
        }
    }


