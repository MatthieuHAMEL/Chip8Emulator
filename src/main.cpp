//#define SDL_MAIN_HANDLED
#include <SDL.h>
#include "cpu.h"
#include "res.h"
#include "graphics.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <iostream>
#include <console.h>


using namespace emu;

int main(int argc, char** argv)
	{
	//SDL_SetMainReady();
	printf("Matoutest\n");

	std::filesystem::path const rom_path = "C:/tmp/myRom.rom";

	CPU cpu;
	Res res = load_rom(rom_path, cpu);
	if (FAILED(res))
		{
		//prompt_error_box("load_rom error", nullptr, &res);
		//return 1;
		}

	Console console;
	init_console(console);

	run_console(console);

	destroy_console(console);

	return 0;
	}
