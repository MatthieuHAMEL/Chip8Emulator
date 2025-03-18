#pragma once

#include "res.h"
#include <fstream>
#include <filesystem>
#include "instr_handlers.h"
#include <cassert>

namespace emu
	{
	struct Console;
	typedef void (*InstrHandler)(uint16_t, Console*); // cf instr_handlers.h

	struct Instruction
		{
		uint16_t mask;
		uint16_t id;
		InstrHandler handler;
		};

	constexpr size_t NB_INSTRUCTIONS = 35;
	constexpr Instruction OPCODE_MASKS[NB_INSTRUCTIONS] =
		{
		//{mask, id}  (if we apply mask to some instruction, we should get id)
		{0xF000, 0x1000, emu_JMP}, // jmp   (1NNN) - jump to address NNN
		{0xFFFF, 0x00E0, emu_CLS}, // cls   (00E0) - clear screen
		{0xFFFF, 0x00EE, emu_RET}, // ret   (00EE) - return from function
		{0xF000, 0x2000, emu_CALL}, // call  (2NNN) - call function at address NNN
		{0xF000, 0x3000, emu_SKEQ}, // skeq  (3XNN) - skip next instruction if VX == NN
		{0xF000, 0x4000, emu_SKNE}, // skne  (4XNN) - skip next instruction if VX != NN
		{0xF00F, 0x5000, emu_SKREQ}, // skreq (5XY0) - skip next instruction if VX == VY
		{0xF000, 0x6000, emu_LD}, // ld    (6XNN) - sets VX to NN
		{0xF000, 0x7000, emu_ADD}, // add   (7XNN) - adds NN to VX (doesn't modify VF)
		{0xF00F, 0x8000, emu_MOV}, // mov   (8XY0) - sets VX to VY
		{0xF00F, 0x8001, emu_OR}, // or    (8XY1) - sets VX to (VX | VY)
		{0xF00F, 0x8002, emu_AND}, // and   (8XY2) - sets VX to (VX & VY)
		{0xF00F, 0x8003, emu_XOR}, // xor   (8XY3) - sets VX to (VX ^ VY)
		{0xF00F, 0x8004, emu_ADDR}, // addr  (8XY4) - adds VY to VX. Sets VF to 1 if carry, else 0.
		{0xF00F, 0x8005, emu_SUB}, // sub   (8XY5) - substracts VY to VX. Sets VF to 1 if NOT borrow, else 0.
		{0xF00F, 0x8006, emu_SHR}, // shr   (8XY6) - 1 bit left shift of VX. VF is set to VX's former MSB.
		{0xF00F, 0x9000, emu_SKRNE}, // skrne (9XY0) - skips next instruction if VX != VY
		{0xF000, 0xA000, emu_LDI}, // ldi   (ANNN) - Sets I to NNN
		{0xF000, 0xB000, emu_JMPI}, // jmpi  (BNNN) - Jumps to NNN+V0
		{0xF000, 0xC000, emu_RND}, // rnd   (CXNN) - Sets VX to (NN & R) where R is random in [|0, 255|]
		{0xF000, 0xD000, emu_DRW}, // drw   (DXYN) - Draws a sprite at VX, VY coordinates. Sprite is 8px large.
		// and Npx tall. Every 8px row is read in binary from address I. Doesn't change I.
		{0xF0FF, 0xE09E, emu_SKPR}, // skpr  (EX9E) - Skips next instruction if key whose value is in VX is pressed.
		{0xF0FF, 0xE0A1, emu_SKUP}, // skup  (EXA1) - Skips next instruction if key whose value is in VX is not pressed.
		{0xF0FF, 0xF007, emu_MOVDT}, // movdt (FX07) - Sets VX to sys_cnt
		{0xF0FF, 0xF00A, emu_KWAIT}, // kwait (FX0A) - Waits for a key to be pressed, then store its value in VX
		{0xF0FF, 0xF015, emu_LDDT}, // lddt  (FX15) - Sets sys_cnt to VX
		{0xF0FF, 0xF018, emu_LDST}, // ldst  (FX18) - Sets sound_cnt to VX
		{0xF0FF, 0xF01E, emu_ADDI}, // addi  (FX1E) - Adds VX to I. Sets VF to 1 if overflow, else 0.
		{0xF0FF, 0xF029, emu_LDSPR}, // ldspr (FX29) - Sets I to the location for the hexadecimal sprite corresponding VX value.
		{0xF0FF, 0xF033, emu_BCD}, // bcd   (FX33) - Store BCD (binary coded decimal) representation of Vx in I, I+1, I+2
		{0xF0FF, 0xF055, emu_STOR}, // stor  (FX55) - Store registers V0 to Vx in memory starting at address I.
		{0xF0FF, 0xF065, emu_READ}, // read  (FX65) - Sets registers V0 to VX with memory value starting from address I.
		{0xF000, 0000, emu_SYS}, // sys (not supported and deprecated - should not be seen in ROMs)
		};

	constexpr size_t MEM_SZ = 4096;
	constexpr size_t STACK_SZ = 16;
	constexpr size_t START_ADDR = 512;

	struct CPU
		{
		struct Console;
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
		unsigned short stack[STACK_SZ] = {}; // stores pc values
		unsigned char stackpos = 0; // Position in the stack

		/// 4. Timers : 60Hz decrementing counters 
		// There is a system timer for game events
		// And a sound timer which emits a sound when value is != 0
		unsigned char sys_cnt = 0;
		unsigned char sound_cnt = 0;
		};

	inline void count(CPU& cpu)
		{
		if (cpu.sys_cnt > 0)
			cpu.sys_cnt--;
		if (cpu.sound_cnt > 0)
			cpu.sound_cnt--;
		}

	inline Res load_rom(const std::filesystem::path& iRomPath, CPU& ioCpu)
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

		ifile.read(reinterpret_cast<char*>(&ioCpu.mem[START_ADDR]), static_cast<std::streamsize>(file_sz));

		if (!ifile.good() && !ifile.eof())
			{
			return Rc::READFILE_FAILED;
			}

		return Rc::OK;
		}

	// pc should not be at iCpu.mem[MEM_SZ-1] !
	inline uint16_t cur_opcode(CPU const& iCpu)
		{
		assert(iCpu.pc < MEM_SZ - 1);
		return (iCpu.mem[iCpu.pc] << 8) + iCpu.mem[iCpu.pc + 1];
		}

	void exec_instructions(size_t N, Console& ioConsole);

	void initialize_memory_fonts(CPU& ioCpu);
	}
