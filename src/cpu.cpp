#include "cpu.h"
#include "console.h"

using namespace emu;

void exec_instructions(size_t N, Console& ioConsole)
{
for (int cnt = 0; cnt < N; ++cnt)
	{
	// Get the 16 bits opcode at the program counter
	uint16_t opcode = cur_opcode(ioConsole.cpu);

	// Match the opcode with an instruction
	int idx = -1;
	for (int i = 0; i < NB_INSTRUCTIONS; ++i)
		{
		if ((opcode & OPCODE_MASKS[i].mask) == OPCODE_MASKS[i].id)
			{
			idx = i;

			// Increment pc for next instruction
			ioConsole.cpu.pc += 2;
			break;
			}
		}

	prompt_error_box("Unknown instruction");
	return;


	}

}