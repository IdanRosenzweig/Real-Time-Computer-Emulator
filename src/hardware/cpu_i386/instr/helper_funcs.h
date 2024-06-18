#ifndef EMULATOR_HELPER_FUNCS_H
#define EMULATOR_HELPER_FUNCS_H

#include "../cpu_i386.hpp"
#include "x86_instruction.h"

inline bool get_is_mode32(cpu_i386 *proc, x86_instr_t *instr) {
    return proc->regs.is_mode32() ^ instr->operand_override;
}

inline bool get_is_addr_override(cpu_i386 *proc, x86_instr_t *instr) {
    return instr->operand_override ^ instr->addr_override;
}

inline sgreg_t select_segment(x86_instr_t *instruction) {
    if (instruction->segment_override.used)
        return instruction->segment_override.reg;
    else
        return instruction->curr_seg;
}

void set_rm32(class cpu_i386 *proc, x86_instr_t *instruction, uint32_t value);

uint32_t get_rm32(class cpu_i386 *proc, x86_instr_t *instruction);

void set_r32(class cpu_i386 *proc, x86_instr_t *instruction, uint32_t value);

uint32_t get_r32(class cpu_i386 *proc, x86_instr_t *instruction);

void set_moffs32(class cpu_i386 *proc, x86_instr_t *instruction, uint32_t value);

uint32_t get_moffs32(class cpu_i386 *proc, x86_instr_t *instruction);

void set_rm16(class cpu_i386 *proc, x86_instr_t *instruction, uint16_t value);

uint16_t get_rm16(class cpu_i386 *proc, x86_instr_t *instruction);

void set_r16(class cpu_i386 *proc, x86_instr_t *instruction, uint16_t value);

uint16_t get_r16(class cpu_i386 *proc, x86_instr_t *instruction);

void set_moffs16(class cpu_i386 *proc, x86_instr_t *instruction, uint16_t value);

uint16_t get_moffs16(class cpu_i386 *proc, x86_instr_t *instruction);

void set_rm8(class cpu_i386 *proc, x86_instr_t *instruction, uint8_t value);

uint8_t get_rm8(class cpu_i386 *proc, x86_instr_t *instruction);

void set_r8(class cpu_i386 *proc, x86_instr_t *instruction, uint8_t value);

void set_moffs8(class cpu_i386 *proc, x86_instr_t *instruction, uint8_t value);

uint8_t get_moffs8(class cpu_i386 *proc, x86_instr_t *instruction);

uint8_t get_r8(class cpu_i386 *proc, x86_instr_t *instruction);

uint32_t get_m(class cpu_i386 *proc, x86_instr_t *instruction);

void set_sreg(class cpu_i386 *proc, x86_instr_t *instruction, uint16_t value);

uint16_t get_sreg(class cpu_i386 *proc, x86_instr_t *instruction);

void set_crn(class cpu_i386 *proc, x86_instr_t *instruction, uint32_t value);

uint32_t get_crn(class cpu_i386 *proc, x86_instr_t *instruction);

uint32_t calc_modrm(class cpu_i386 *proc, x86_instr_t *instruction);

uint32_t calc_modrm16(class cpu_i386 *proc, x86_instr_t *instruction);

uint32_t calc_modrm32(class cpu_i386 *proc, x86_instr_t *instruction);

uint32_t calc_sib(class cpu_i386 *proc, x86_instr_t *instruction);



uint8_t type_descriptor(class cpu_i386 *proc, uint16_t sel);

void set_ldtr(class cpu_i386 *proc, uint16_t sel);

void set_tr(class cpu_i386 *proc, uint16_t sel);

void switch_task(class cpu_i386 *proc, uint16_t sel);

void jmpf(class cpu_i386 *proc, uint16_t sel, uint32_t eip);

void callf(class cpu_i386 *proc, uint16_t sel, uint32_t eip);


bool chk_ring(class cpu_i386 *proc, uint8_t DPL);


void push32(class cpu_i386 *proc, uint32_t v);

uint32_t pop32(class cpu_i386 *proc);

void push16(class cpu_i386 *proc, uint16_t v);

uint16_t pop16(class cpu_i386 *proc);


#endif
