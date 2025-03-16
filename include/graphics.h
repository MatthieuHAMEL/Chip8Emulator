#pragma once

#define SDL_MAIN_HANDLED
#include <SDL.h>

namespace Emu
	{
	// A pixel is a boolean (black or white). Screen resolution is 64x32. 
	constexpr size_t SCREEN_W = 64;
	constexpr size_t SCREEN_H = 32;
	constexpr size_t PIX_DIM = 16; // The "real" size of a chip8 pixel on our screen. 

	struct Screen
		{
		SDL_Window* window = nullptr;
		SDL_Renderer* renderer = nullptr;
		bool screen[SCREEN_W][SCREEN_H] = {};
		};


	}


