#include "console.h"
#include <cstdlib>

using namespace emu;

// (1NNN) - jump to address NNN
void emu_JMP(uint16_t iInstr, emu::Console* ioConsole)
	{
	// -2 because pc is incremented by 2 after every instruction
	ioConsole->cpu.pc = (iInstr & 0x0FFF) - 2;
	}

void emu_CLS(uint16_t iInstr, emu::Console*)
{
	throw 1;
}
void emu_RET(uint16_t iInstr, emu::Console*)
{
	throw 1;
}

// (2NNN) - call function at address NNN
void emu_CALL(uint16_t iInstr, emu::Console* ioConsole)
	{
	auto& cpu = ioConsole->cpu; // for readability...
	// Fill the stack with the current program counter
	assert(cpu.stackpos < STACK_SZ);
	cpu.stack[cpu.stackpos] = cpu.pc;
	if (cpu.stackpos < STACK_SZ - 1)
		cpu.stackpos++;
	else {
		fprintf(stderr, "stack overflow!");
		assert(false);
	}
	// Now jump to the function
	cpu.pc = iInstr & 0x0FFF - 2; // TODO 
	}

// (3XNN) - skip next instruction if VX == NN
void emu_SKEQ(uint16_t iInstr, emu::Console* ioConsole)
	{
	if (ioConsole->cpu.regs[(iInstr & 0x0F00) >> 8] == (iInstr & 0x00FF))
		{
		ioConsole->cpu.pc += 2;
		}
	}
void emu_SKNE(uint16_t iInstr, emu::Console*)
{
	throw 1;
}
void emu_SKREQ(uint16_t iInstr, emu::Console*)
{
	throw 1;
}

// (6XNN) - sets VX to NN
void emu_LD(uint16_t iInstr, emu::Console* ioConsole)
	{
	ioConsole->cpu.regs[(iInstr & 0x0F00) >> 8] = (iInstr & 0x00FF);
	}

// (7XNN) - adds NN to VX (doesn't modify VF)
void emu_ADD(uint16_t iInstr, emu::Console* ioConsole)
	{
	ioConsole->cpu.regs[(iInstr & 0x0F00) >> 8] += (iInstr & 0x00FF);
	}

// (8XY0) - sets VX to VY
void emu_MOV(uint16_t iInstr, emu::Console* ioConsole)
	{
	ioConsole->cpu.regs[iInstr & 0x0F00 >> 8] = ioConsole->cpu.regs[iInstr & 0x00F0 >> 4];
	}
void emu_OR(uint16_t iInstr, emu::Console*)
{
	throw 1;
}
void emu_AND(uint16_t iInstr, emu::Console*)
{
	throw 1;
}
void emu_XOR(uint16_t iInstr, emu::Console*)
{
	throw 1;
}
void emu_ADDR(uint16_t iInstr, emu::Console*)
{
	throw 1;
}
void emu_SUB(uint16_t iInstr, emu::Console*)
{
	throw 1;
}
void emu_SHR(uint16_t iInstr, emu::Console*)
{
	throw 1;
}
void emu_SKRNE(uint16_t iInstr, emu::Console*)
{
	throw 1;
}
void emu_LDI(uint16_t iInstr, emu::Console* iConsole)
	{
	iConsole->cpu.I = iInstr & 0x0FFF;
	}
void emu_JMPI(uint16_t iInstr, emu::Console*)
{
	throw 1;
}
void emu_RND(uint16_t iInstr, emu::Console* ioConsole)
	{
	// (CXNN)-Sets VX to(NN & R) where R is random in[| 0, 255 | ]
	// Random number 
	unsigned char r = rand() & 255; // not perfect!... (TODO)
	ioConsole->cpu.regs[(iInstr & 0x0F00) >> 8] = (iInstr & 0x00FF) & r;
	}

/*
(DXYN)-
Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision.
The interpreter reads n bytes from memory, starting at the address stored in I. 
These bytes are then displayed as sprites on screen at coordinates (Vx, Vy). 
Sprites are XORed onto the existing screen. 
If this causes any pixels to be erased, VF is set to 1, otherwise it is set to 0. 
If the sprite is positioned so part of it is outside the coordinates of the display, 
it wraps around to the opposite side of the screen. 
See instruction 8xy3 for more information on XOR, and section 2.4, 
Display, for more information on the Chip-8 screen and sprites.

*/
void emu_DRW(uint16_t iInstr, emu::Console* ioConsole)
	{
	uint8_t const height_px = iInstr & 0x000F;
	uint16_t const vx = ioConsole->cpu.regs[(iInstr & 0x0F00) >> 8];
	uint16_t const vy = ioConsole->cpu.regs[(iInstr & 0x00F0) >> 4];
	uint16_t px_idx = ioConsole->cpu.I;
	for (int i = 0; i < height_px; ++i)
		{
		for (int j = 0; j < 8; ++j)
			{
			// The XOR :
			// If memory is 0, whether the pixel is 1 or 0, don't touch it
			// If memory is 1, reverse it (if it is 1 it becomes 0, if it is 0 it becomes 1)
			if ((ioConsole->cpu.mem[px_idx] << j) & 0b10000000)
				{
				auto& pixel = ioConsole->screen.pix_grid[(vx + j) % SCREEN_W][(vy + i) % SCREEN_H];
				if (pixel)
					ioConsole->cpu.regs[0xF] = 1;
				pixel = !pixel;
				}
			}
		++px_idx;
		}
	}
void emu_SKPR(uint16_t iInstr, emu::Console*)
{
	throw 1;
}
void emu_SKUP(uint16_t iInstr, emu::Console*)
{
	throw 1;
}
void emu_MOVDT(uint16_t iInstr, emu::Console*)
{
	throw 1;
}
void emu_KWAIT(uint16_t iInstr, emu::Console*)
{
	throw 1;
}
void emu_LDDT(uint16_t iInstr, emu::Console*)
{
	throw 1;

}
void emu_LDST(uint16_t iInstr, emu::Console*)
{
	throw 1;
}
void emu_ADDI(uint16_t iInstr, emu::Console*)
{
	throw 1;
}
void emu_LDSPR(uint16_t iInstr, emu::Console*)
{
	throw 1;
}
void emu_BCD(uint16_t iInstr, emu::Console*)
{
	throw 1;
}
void emu_STOR(uint16_t iInstr, emu::Console*)
{
	throw 1;
}
void emu_READ(uint16_t iInstr, emu::Console*)
{
	throw 1;
}
void emu_SYS(uint16_t, emu::Console*)
	{
	fprintf(stderr, "emu_SYS\n");
	}