#include "cpu.h"
#include "console.h"

using namespace emu;


void emu::initialize_memory_fonts(CPU& ioCpu)
	{
	auto& memory = ioCpu.mem;
	memory[0] = 0xF0; memory[1] = 0x90; memory[2] = 0x90; memory[3] = 0x90; memory[4] = 0xF0; // O
	memory[5] = 0x20; memory[6] = 0x60; memory[7] = 0x20; memory[8] = 0x20; memory[9] = 0x70; // 1
	memory[10] = 0xF0; memory[11] = 0x10; memory[12] = 0xF0; memory[13] = 0x80; memory[14] = 0xF0; // 2
	memory[15] = 0xF0; memory[16] = 0x10; memory[17] = 0xF0; memory[18] = 0x10; memory[19] = 0xF0; // 3
	memory[20] = 0x90; memory[21] = 0x90; memory[22] = 0xF0; memory[23] = 0x10; memory[24] = 0x10; // 4
	memory[25] = 0xF0; memory[26] = 0x80; memory[27] = 0xF0; memory[28] = 0x10; memory[29] = 0xF0; // 5
	memory[30] = 0xF0; memory[31] = 0x80; memory[32] = 0xF0; memory[33] = 0x90; memory[34] = 0xF0; // 6
	memory[35] = 0xF0; memory[36] = 0x10; memory[37] = 0x20; memory[38] = 0x40; memory[39] = 0x40; // 7
	memory[40] = 0xF0; memory[41] = 0x90; memory[42] = 0xF0; memory[43] = 0x90; memory[44] = 0xF0; // 8
	memory[45] = 0xF0; memory[46] = 0x90; memory[47] = 0xF0; memory[48] = 0x10; memory[49] = 0xF0; // 9
	memory[50] = 0xF0; memory[51] = 0x90; memory[52] = 0xF0; memory[53] = 0x90; memory[54] = 0x90; // A
	memory[55] = 0xE0; memory[56] = 0x90; memory[57] = 0xE0; memory[58] = 0x90; memory[59] = 0xE0; // B
	memory[60] = 0xF0; memory[61] = 0x80; memory[62] = 0x80; memory[63] = 0x80; memory[64] = 0xF0; // C
	memory[65] = 0xE0; memory[66] = 0x90; memory[67] = 0x90; memory[68] = 0x90; memory[69] = 0xE0; // D
	memory[70] = 0xF0; memory[71] = 0x80; memory[72] = 0xF0; memory[73] = 0x80; memory[74] = 0xF0; // E
	memory[75] = 0xF0; memory[76] = 0x80; memory[77] = 0xF0; memory[78] = 0x80; memory[79] = 0x80; // F
	}

void emu::exec_instructions(size_t N, Console& ioConsole)
	{
	for (int cnt = 0; cnt < N; ++cnt)
		{
		// Get the 16 bits opcode at the program counter
		uint16_t opcode = cur_opcode(ioConsole.cpu);

		// Match the opcode with an instruction
		bool found = false;
		for (int i = 0; i < NB_INSTRUCTIONS && !ioConsole.quit; ++i)
			{
			if ((opcode & OPCODE_MASKS[i].mask) == OPCODE_MASKS[i].id)
				{
				found = true;

				count(ioConsole.cpu);
				
				// Execute the instruction
				OPCODE_MASKS[i].handler(opcode, &ioConsole);
				
				// Increment pc for next instruction
				ioConsole.cpu.pc += 2;
				break;
				}
			}
		if (!found && !ioConsole.quit)
			{
			prompt_error_box("Unknown instruction");
			}
		} // 4 instructions have been executed
	}
