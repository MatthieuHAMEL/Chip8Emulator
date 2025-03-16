#pragma once

#include "SDL.h"
#include <cstdint>
#include <string>

namespace emu
	{
	enum class Rc : uint32_t
		{
		OK,
		FOPEN_FAILED,
		ROM_TOO_LARGE,
		READFILE_FAILED,
		SDL_ERROR,
		};

	struct Res
		{
		Rc rc;
		uint32_t sys_code;

		// Implicit Res created from a simple return code
		Res(Rc rc) : rc(rc), sys_code(0) {}
		};

	inline bool FAILED(Res r)
		{
		return (r.rc != Rc::OK);
		}

	inline void prompt_error_box(const char* iContext, const char* iErrorMsg = nullptr, Res* iErrc = nullptr)
		{
		std::string error = iContext;
		error += "\nFull message is :";
		error += iErrorMsg ? iErrorMsg : "[none]";
		if (iErrc)
			{
			error += "\nCode is " + std::to_string(static_cast<size_t>(iErrc->rc)) 
				+ " / syserr " + std::to_string(iErrc->sys_code);
			}
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Chip8Emu Error", error.c_str(), nullptr);
		}

	static_assert(sizeof(Res) == 8, "Unexpected Res size!");
	}
