#pragma once

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
	}
