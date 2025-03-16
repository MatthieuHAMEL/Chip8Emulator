#include "SDL.h"
#include "cpu.h"
#include "res.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <iostream>
#include <fstream>


using namespace Emu;

static void count(CPU& cpu)
	{
	if (cpu.sys_cnt > 0)
		cpu.sys_cnt--;
	if (cpu.sound_cnt > 0)
		cpu.sound_cnt--;
	}

static Res load_rom(const std::filesystem::path& iRomPath, CPU& ioCpu)
	{
	// Open ROM file in binary mode 
	std::ifstream ifile(iRomPath, std::ios::binary);
	if (!ifile.is_open())
		{
		return Rc::FOPEN_FAILED; // To detail more with sys_code maybe...
		}

	auto const file_sz = std::filesystem::file_size(iRomPath);
	if (file_sz > (MEM_SZ - START_ADDR))
		{
		return Rc::ROM_TOO_LARGE;
		}

	ifile.read(reinterpret_cast<char*>(ioCpu.mem[START_ADDR]), static_cast<std::streamsize>(file_sz));

	if (!ifile.good() && !ifile.eof())
		{
		return Rc::READFILE_FAILED;
		}

	return Rc::OK;
	}

int main(int argc, char* args[])
	{
	printf("Matoutest\n");

	std::filesystem::path const rom_path = "C:/tmp/myRom.rom";

	CPU cpu;
	Res res = load_rom(rom_path, cpu);



	return 0;
	}
