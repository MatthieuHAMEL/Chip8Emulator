#include "console.h"

using namespace emu;

// Returns true if application should quit.
bool emu::poll_events(Console& ioConsole)
	{
	SDL_Event evt = {};
	while (SDL_PollEvent(&evt) != 0)
		{
		if (evt.type == SDL_QUIT) // Only event for now.
			ioConsole.quit = true;
		else if (evt.type == SDL_KEYDOWN)
			ioConsole.keyboard.sdl_keys[evt.key.keysym.scancode] = true;
		else if (evt.type == SDL_KEYUP)
			ioConsole.keyboard.sdl_keys[evt.key.keysym.scancode] = false;
		}

	// Update the console keys based on sdl keys (cf. keyboard.h) :
	for (size_t i = 0; i < 16; ++i)
		ioConsole.keyboard.console_keys[i] = ioConsole.keyboard.sdl_keys[EMU_KEY_TO_SDL_KEY[i]];

	return ioConsole.quit;
	}

void emu::run_console(Console& ioConsole)
	{
	SDL_Event evt = {};
	while (!ioConsole.quit)
		{
		poll_events(ioConsole);

		/*Execute 4 instructions (250Hz freq choice)*/
		exec_instructions(4, ioConsole);
		render_screen(ioConsole.screen);
		SDL_Delay(16);
		}
	}