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
	printf("Matoutest\n");

	std::filesystem::path const rom_path = "C:\\tmp\\rndtest.ch8";

	Console console;
	init_console(console);

	Res res = load_rom(rom_path, console);
	if (FAILED(res))
		{
		prompt_error_box("load_rom error", nullptr, &res);
		return 1;
		}

	run_console(console);

	destroy_console(console);
	return 0;
	}
