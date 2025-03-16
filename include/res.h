#pragma once

#include <cstdint>

namespace Emu
	{
	enum class Rc : uint32_t
		{
		OK,
		FOPEN_FAILED,
		ROM_TOO_LARGE,
		READFILE_FAILED,
		};

	struct Res
		{
		Rc rc;
		uint32_t sys_code;

		// Implicit Res created from a simple return code
		Res(Rc rc) : rc(rc), sys_code(0) {}
		};

	static_assert(sizeof(Res) == 8, "Unexpected Res size!");
	}
