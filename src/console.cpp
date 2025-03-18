#include "console.h"

using namespace emu;

void emu::run_console(Console& ioConsole)
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
		exec_instructions(4, ioConsole);
		render_screen(ioConsole.screen);
		SDL_Delay(16);
		}
	}