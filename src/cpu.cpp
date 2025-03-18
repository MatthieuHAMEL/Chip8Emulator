#include "cpu.h"
#include "console.h"

using namespace emu;

void emu::exec_instructions(size_t N, Console& ioConsole)
	{
	for (int cnt = 0; cnt < N; ++cnt)
		{
		// Get the 16 bits opcode at the program counter
		uint16_t opcode = cur_opcode(ioConsole.cpu);

		// Match the opcode with an instruction
		bool found = false;
		for (int i = 0; i < NB_INSTRUCTIONS; ++i)
			{
			if ((opcode & OPCODE_MASKS[i].mask) == OPCODE_MASKS[i].id)
				{
				found = true;

				// Execute the instruction
				OPCODE_MASKS[i].handler(opcode, &ioConsole);

				// Increment pc for next instruction
				ioConsole.cpu.pc += 2;
				break;
				}
			}
		if (!found)
			{
			prompt_error_box("Unknown instruction");
			}
		} // 4 instructions have been executed
	}
