#include "instr_executer.h"
#include "../cpu_i386.hpp"

#include "helper_funcs.h"

#include <iostream>

using namespace std;;

std::map <opcode_t, instr_handler> opcode_handlers_common;
std::map <opcode_t, instr_handler> opcode_handlers_mode16;
std::map <opcode_t, instr_handler> opcode_handlers_mode32;

void execute(cpu_i386 *proc, x86_instr_t *instr) {
    opcode_t opcode = instr->opcode;

    if (opcode_handlers_common.count(opcode)) {
        (*opcode_handlers_common[opcode])(proc, instr);
        return;
    }

    if (get_is_mode32(proc, instr)) {
        if (opcode_handlers_mode32.count(opcode)) {
            (*opcode_handlers_mode32[opcode])(proc, instr);
            return;
        }
    } else {
        if (opcode_handlers_mode16.count(opcode)) {
            (*opcode_handlers_mode16[opcode])(proc, instr);
            return;
        }
    }

    DEBUG_ERROR("not implemented OPCODE: 0x%02x", opcode);

}
