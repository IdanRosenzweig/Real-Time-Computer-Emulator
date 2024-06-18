#ifndef EMULATOR_INSTRUCTION_DECODER_H
#define EMULATOR_INSTRUCTION_DECODER_H

#include <unordered_map>

#include "../cpu_i386.hpp"
#include "x86_instruction.h"

extern std::unordered_map <opcode_t, x86_instr_flags_t> opcode_flags_common;
extern std::unordered_map <opcode_t, x86_instr_flags_t> opcode_flags_mode16;
extern std::unordered_map <opcode_t, x86_instr_flags_t> opcode_flags_mode32;

void decode_prefixes(cpu_i386 *processor, x86_instr_t *instruction);

void decode_opcode(cpu_i386 *processor, x86_instr_t *instruction);

void decode_modrm_sib_disp(cpu_i386 *processor, x86_instr_t *instruction);

void decode_moffs(cpu_i386 *processor, x86_instr_t *instruction);

void decode(cpu_i386 *processor, x86_instr_t *instruction);


#endif
