#include "SDL.h"
#include "cpu.h"
#include "res.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <iostream>

using namespace Emu;

int main(int argc, char* args[])
	{
	printf("Matoutest\n");

	std::filesystem::path const rom_path = "C:/tmp/myRom.rom";

	CPU cpu;
	Res res = load_rom(rom_path, cpu);


	return 0;
	}
