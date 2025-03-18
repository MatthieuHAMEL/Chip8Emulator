#pragma once

#include <cstdint>

namespace emu{ struct Console; }

void emu_JMP(uint16_t, emu::Console*);
void emu_CLS(uint16_t, emu::Console*);
void emu_RET(uint16_t, emu::Console*);
void emu_CALL(uint16_t, emu::Console*);
void emu_SKEQ(uint16_t, emu::Console*);
void emu_SKNE(uint16_t, emu::Console*);
void emu_SKREQ(uint16_t, emu::Console*);
void emu_LD(uint16_t, emu::Console*);
void emu_ADD(uint16_t, emu::Console*);
void emu_MOV(uint16_t, emu::Console*);
void emu_OR(uint16_t, emu::Console*);
void emu_AND(uint16_t, emu::Console*);
void emu_XOR(uint16_t, emu::Console*);
void emu_ADDR(uint16_t, emu::Console*);
void emu_SUB(uint16_t, emu::Console*);
void emu_SHR(uint16_t, emu::Console*);
void emu_SKRNE(uint16_t, emu::Console*);
void emu_LDI(uint16_t, emu::Console*);
void emu_JMPI(uint16_t, emu::Console*);
void emu_RND(uint16_t, emu::Console*);
void emu_DRW(uint16_t, emu::Console*);
void emu_SKPR(uint16_t, emu::Console*);
void emu_SKUP(uint16_t, emu::Console*);
void emu_MOVDT(uint16_t, emu::Console*);
void emu_KWAIT(uint16_t, emu::Console*);
void emu_LDDT(uint16_t, emu::Console*);
void emu_LDST(uint16_t, emu::Console*);
void emu_ADDI(uint16_t, emu::Console*);
void emu_LDSPR(uint16_t, emu::Console*);
void emu_BCD(uint16_t, emu::Console*);
void emu_STOR(uint16_t, emu::Console*);
void emu_READ(uint16_t, emu::Console*);
void emu_SYS(uint16_t, emu::Console*);
