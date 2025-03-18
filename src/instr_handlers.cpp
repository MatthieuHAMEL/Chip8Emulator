#include "console.h"
#include <cstdlib>

using namespace emu;

// (1NNN) - jump to address NNN
void emu_JMP(uint16_t iInstr, emu::Console* ioConsole)
	{
	ioConsole->cpu.pc = (iInstr & 0x0FFF);
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
	cpu.stack[cpu.stackpos] = cpu.pc;
	if (cpu.stackpos < STACK_SZ - 1)
		cpu.stackpos++;
	else
		fprintf(stderr, "stack overflow!");

	// Now jump to the function
	cpu.pc = iInstr & 0x0FFF - 2;
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
(DXYN)-Draws a sprite at VX, VY coordinates.Sprite is 8px large
and Npx tall. Every 8px row is read in binary from address I. Doesn't change I.
*/
void emu_DRW(uint16_t iInstr, emu::Console* ioConsole)
	{
	uint8_t const height_px = iInstr & 0x000F;
	uint16_t const vx = ioConsole->cpu.regs[(iInstr & 0x0F00) >> 8];
	uint16_t const vy = ioConsole->cpu.regs[(iInstr & 0x00F0) >> 4];

	for (int i = 0; i < height_px; ++i)
		{
		for (int j = 0; j < 8; ++j)
			{
			ioConsole->screen.pix_grid[vx + j][vy + i] = !ioConsole->screen.pix_grid[vx + j][vy + i];
			} // TODO VF.
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