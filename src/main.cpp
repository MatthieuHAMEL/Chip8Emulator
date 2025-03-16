//#define SDL_MAIN_HANDLED
#include <SDL.h>
#include "cpu.h"
#include "res.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <iostream>

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
		fprintf(stderr, "couldn't load rom Error=<%u>\n", res.rc);
		return 1;
		}

	return 0;
	}
