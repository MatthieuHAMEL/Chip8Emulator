#include "console.h"
#include <cstdlib>

using namespace emu;

// (1NNN) - jump to address NNN
void emu_JMP(uint16_t iInstr, emu::Console* ioConsole)
	{
	// -2 because pc is incremented by 2 after every instruction
	ioConsole->cpu.pc = (iInstr & 0x0FFF) - 2;
	}

void emu_CLS(uint16_t iInstr, emu::Console* ioConsole)
	{
	clear_screen(ioConsole->screen);
	}

// (00EE) - return from function
void emu_RET(uint16_t iInstr, emu::Console* ioConsole)
	{
	auto& cpu = ioConsole->cpu; // for readability
	assert(cpu.stackpos >= 1);
	cpu.stackpos--;
	// Jump back to previous instruction in the stack 
	cpu.pc = cpu.stack[cpu.stackpos];
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
	else 
		{
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

// (4XNN) - skip next instruction if VX != NN
void emu_SKNE(uint16_t iInstr, emu::Console* ioConsole)
	{
	if (ioConsole->cpu.regs[(iInstr & 0x0F00) >> 8] != (iInstr & 0x00FF))
		{
		ioConsole->cpu.pc += 2;
		}
	}

// (5XY0) - skip next instruction if VX == VY
void emu_SKREQ(uint16_t iInstr, emu::Console* ioConsole)
	{
	if (ioConsole->cpu.regs[(iInstr & 0x0F00) >> 8] == (iInstr & 0x00FF))
		{
		ioConsole->cpu.pc += 2;
		}
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
	ioConsole->cpu.regs[(iInstr & 0x0F00) >> 8] = ioConsole->cpu.regs[(iInstr & 0x00F0) >> 4];
	}

// (8XY1) - sets VX to (VX | VY)
void emu_OR(uint16_t iInstr, emu::Console* ioConsole)
	{
	auto& cpu = ioConsole->cpu;
	cpu.regs[iInstr & 0x0F00 >> 8] = cpu.regs[(iInstr & 0x0F00) >> 8] | cpu.regs[(iInstr & 0x00F0) >> 4];
	}

// (8XY2) - sets VX to (VX & VY)
void emu_AND(uint16_t iInstr, emu::Console* ioConsole)
	{
	auto& cpu = ioConsole->cpu;
	cpu.regs[iInstr & 0x0F00 >> 8] = cpu.regs[(iInstr & 0x0F00) >> 8] & cpu.regs[(iInstr & 0x00F0) >> 4];
	}

// (8XY3) sets VX to (VX ^ VY)
void emu_XOR(uint16_t iInstr, emu::Console* ioConsole)
	{
	auto& cpu = ioConsole->cpu;
	cpu.regs[iInstr & 0x0F00 >> 8] = cpu.regs[(iInstr & 0x0F00) >> 8] ^ cpu.regs[(iInstr & 0x00F0) >> 4];
	}

// (8XY4) - adds VY to VX. Sets VF to 1 if carry, else 0.
void emu_ADDR(uint16_t iInstr, emu::Console* ioConsole)
	{
	uint8_t& vx = ioConsole->cpu.regs[(iInstr & 0x0F00) >> 8];   // (chip8->current_op & 0x0F00) >> 8;
	uint8_t& vy = ioConsole->cpu.regs[(iInstr & 0x00F0) >> 4];
	uint16_t sum = vx + vy;
	ioConsole->cpu.regs[0xF] = (sum > 255) ? 1 : 0; // Carry ?
	vx = (sum & 0xFF);
	}

// (8XY5) - substracts VY to VX. Sets VF to 1 if NOT borrow, else 0.
void emu_SUB(uint16_t iInstr, emu::Console* ioConsole)
	{
	auto& vx = ioConsole->cpu.regs[(iInstr & 0x0F00) >> 8];
	auto& vy = ioConsole->cpu.regs[(iInstr & 0x00F0) >> 4];
	
	ioConsole->cpu.regs[0xF] = (vx > vy) ? 1 : 0;
	vx -= vy;
	}

// (8XY6) - 1 bit right shift of VX. VF is set to VX's former LSB.
void emu_SHR(uint16_t iInstr, emu::Console* ioConsole)
	{
	auto& vx = ioConsole->cpu.regs[(iInstr & 0x0F00) >> 8];

	// Set carry to LSB of vx
	ioConsole->cpu.regs[0xF] = (vx % 2);

	// Right shift vx
	vx = vx >> 1;
	}

// (9XY0) - skips next instruction if VX != VY
void emu_SKRNE(uint16_t iInstr, emu::Console* ioConsole)
	{
	if (ioConsole->cpu.regs[(iInstr & 0x0F00) >> 8] != ioConsole->cpu.regs[(iInstr & 0x00F0) >> 4])
		{
		ioConsole->cpu.pc += 2;
		}
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
Display, for more information on the Chip-8 screen and sprites. */
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
					{
					ioConsole->cpu.regs[0xF] = 1;
					}
				pixel = !pixel;
				}
			}
		++px_idx;
		}
	}

// (EX9E)-Skips next instruction if key whose value is in VX is pressed.
void emu_SKPR(uint16_t iInstr, emu::Console* ioConsole)
	{
	auto& cpu = ioConsole->cpu;
	if (ioConsole->keyboard.console_keys[cpu.regs[(iInstr & 0x0F00) >> 8]])
		{
		cpu.pc += 2;
		}
	}

void emu_SKUP(uint16_t iInstr, emu::Console*)
	{
	throw 1;
	}

// (FX07)-Sets VX to sys_cnt
void emu_MOVDT(uint16_t iInstr, emu::Console* ioConsole)
	{
	ioConsole->cpu.regs[(iInstr & 0x0F00) >> 8] = ioConsole->cpu.sys_cnt;
	}

// (FX0A) - Waits for a key to be pressed, then store its value in VX
void emu_KWAIT(uint16_t iInstr, emu::Console* ioConsole)
	{	
	// Copy current console key state :
	bool old_key_state[NB_KEYS];
	memcpy(old_key_state, ioConsole->keyboard.console_keys, sizeof(old_key_state));

	// Start event subloop 
	while (!poll_events(*ioConsole))
		{
		for (size_t i = 0; i < NB_KEYS; ++i)
			{
			if (ioConsole->keyboard.console_keys[i] && !old_key_state[i]) // A new key has been pressed
				{
				// "The value of that key is stored in VX"
				ioConsole->cpu.regs[(iInstr & 0x0F00) >> 8] = i;
				return;
				}
			}
		}
	fprintf(stderr, "Quit in subloop... (propagating...)");
	}

// (FX15)-Sets sys_cnt to VX
void emu_LDDT(uint16_t iInstr, emu::Console* ioConsole)
	{
	ioConsole->cpu.sys_cnt = ioConsole->cpu.regs[(iInstr & 0x0F00) >> 8];
	}	

// (FX18) - Sets sound_cnt to VX
void emu_LDST(uint16_t iInstr, emu::Console* ioConsole)
	{
	ioConsole->cpu.sound_cnt = ioConsole->cpu.regs[(iInstr & 0x0F00) >> 8];
	}

// (FX1E) - Adds VX to I. Sets VF to 1 if overflow, else 0.
void emu_ADDI(uint16_t iInstr, emu::Console* ioConsole)
	{
	if (ioConsole->cpu.I + ioConsole->cpu.regs[(iInstr & 0x0F00) >> 8] > 0xFFF)
		ioConsole->cpu.regs[0xF] = 1;
	else
		ioConsole->cpu.regs[0xF] = 0;
	ioConsole->cpu.I += ioConsole->cpu.regs[(iInstr & 0x0F00) >> 8];
	}

//  (FX29) - Sets I to the location for the hexadecimal sprite corresponding VX value.
// Characters 0-F (in hexadecimal) are represented by a 4x5 font
void emu_LDSPR(uint16_t iInstr, emu::Console* ioConsole)
	{
	// cf. initialize_memory_fonts
	ioConsole->cpu.I = 5 * ioConsole->cpu.regs[(iInstr & 0x0F00) >> 8];
	}

// (FX33) - Store BCD (binary coded decimal) representation of Vx in I, I+1, I+2
void emu_BCD(uint16_t iInstr, emu::Console* ioConsole)
	{
	auto& cpu = ioConsole->cpu;
	uint16_t nb = cpu.regs[(iInstr & 0x0F00) >> 8];
	cpu.mem[cpu.I + 2] = nb % 10;
	cpu.mem[cpu.I + 1] = (nb / 10) % 10;
	cpu.mem[cpu.I] = nb / 100;
	}

// (FX55) - Store registers V0 to Vx in memory starting at address I.
void emu_STOR(uint16_t iInstr, emu::Console* ioConsole)
	{
	auto& cpu = ioConsole->cpu;
	memcpy((void*) &cpu.mem[cpu.I], (void*) &cpu.regs[0], (1 + ((iInstr & 0x0F00) >> 8)) * sizeof(uint16_t));
	}

// (FX65) - Sets registers V0 to VX with memory value starting from address I.
void emu_READ(uint16_t iInstr, emu::Console* ioConsole)
	{
	auto& cpu = ioConsole->cpu;
	memcpy((void*)&cpu.regs[0], (void*)&cpu.mem[cpu.I], (1 + ((iInstr & 0x0F00) >> 8)) * sizeof(uint16_t));
	}

void emu_SYS(uint16_t, emu::Console*)
	{
	fprintf(stderr, "emu_SYS (not implemented)\n");
	}
