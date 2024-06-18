#ifndef EMULATOR_INSTRUCTION_EXECUTER_H
#define EMULATOR_INSTRUCTION_EXECUTER_H

#include "x86_instruction.h"

#include <unordered_map>
#include <map>

typedef void (*instr_handler)(class cpu_i386 *proc, x86_instr_t *);

extern std::map <opcode_t, instr_handler> opcode_handlers_common;
extern std::map <opcode_t, instr_handler> opcode_handlers_mode16;
extern std::map <opcode_t, instr_handler> opcode_handlers_mode32;

void execute(class cpu_i386 *proc, x86_instr_t *instruction);


#endif
