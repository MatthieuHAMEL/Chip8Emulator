#pragma once
#include <cpu.h>
#include <graphics.h>
#include <keyboard.h>

namespace emu
	{
	struct Console
		{
		CPU cpu;
		Screen screen;
		Keyboard keyboard;

		bool quit = false;
		};

	// Console is created once in the lifetime of the exe. It's deallocation does SDL_Quit.
	inline Res init_console(Console& iConsole)
		{
		initialize_memory_fonts(iConsole.cpu);

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
	
	inline Res load_rom(const std::filesystem::path& iRomPath, Console& ioConsole)
		{
		return load_rom(iRomPath, ioConsole.cpu);
		}

	bool poll_events(Console& ioConsole);
	void run_console(Console& ioConsole);
	}