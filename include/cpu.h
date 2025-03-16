#pragma once

#include "res.h"
#include <fstream>
#include <filesystem>

namespace Emu
	{
	constexpr size_t MEM_SZ = 4096;
	constexpr size_t START_ADDR = 512;

	struct CPU
		{
		/// 1. Memory
		/* "Memory goes from 0x200 to 0xFFF. The 512 first bytes were
		reserved for the interpreter.
		0xF00-0xFFF (sz 256) were reserved for screen refreshing,
		0xEA0 - 0xEFF (sz 96) for the callstack and variables. */
		unsigned char mem[MEM_SZ] = {};
		unsigned short pc = START_ADDR; // 0x200

		/// 2. Registers
		/* There are 16 registers of 8 bits each.
		V0 -> VF. VF is used for deductions in calculations.
		There is also a 16 bits register, I, used for memory
		operations. */
		unsigned char regs[16] = {};
		unsigned short I = 0;

		/// 3. Stack - 16 levels 
		unsigned short stack[16] = {}; // stores pc values
		unsigned char stackpos = 0; // Position in the stack

		/// 4. Timers : 60Hz decrementing counters 
		// There is a system timer for game events
		// And a sound timer which emits a sound when value is != 0
		unsigned char sys_cnt = 0;
		unsigned char sound_cnt = 0;
		};

	static void count(CPU& cpu)
		{
		if (cpu.sys_cnt > 0)
			cpu.sys_cnt--;
		if (cpu.sound_cnt > 0)
			cpu.sound_cnt--;
		}

	Res load_rom(const std::filesystem::path& iRomPath, CPU& ioCpu)
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
	}
