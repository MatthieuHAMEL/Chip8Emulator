#pragma once
#include <cpu.h>
#include <graphics.h>

namespace emu
	{
	struct Console
		{
		CPU cpu;
		Screen screen;
		};

	// Console is created once in the lifetime of the exe. It's deallocation does SDL_Quit.
	inline Res init_console(Console& iConsole)
		{
		Res res = init_SDL();
		if (FAILED(res)) return res; // Should have prompted a msgbox ...

		res = init_screen(iConsole.screen);
		if (FAILED(res))
			{
			destroy_SDL();
			return res;
			}

		return Rc::OK;
		}

	inline void destroy_console(Console& iConsole)
		{
		destroy_screen(iConsole.screen);
		destroy_SDL();
		}
	
	inline void run_console(Console& iConsole)
		{
		SDL_Event evt = {};
		bool done = false;
		while (!done)
			{
			while (SDL_PollEvent(&evt) != 0)
				{
				if (evt.type == SDL_QUIT) // Only event for now.
					done = true; 
				}

			/*Execute 4 instructions (250Hz freq choice)*/
			render_screen(iConsole.screen);
			SDL_Delay(16);
			}
		}

	}