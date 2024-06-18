#ifndef EMULATOR_OPCODE_HANDLERS_H
#define EMULATOR_OPCODE_HANDLERS_H

#include "../cpu_i386.hpp"
#include "x86_instruction.h"
#include "instr_executer.h"

void retf(class cpu_i386 *proc, x86_instr_t *instruction);

void iret(class cpu_i386 *proc, x86_instr_t *instruction);

void add_rm8_r8(class cpu_i386 *proc, x86_instr_t *instruction);

void add_r8_rm8(class cpu_i386 *proc, x86_instr_t *instruction);

void add_al_imm8(class cpu_i386 *proc, x86_instr_t *instruction);

void or_rm8_r8(class cpu_i386 *proc, x86_instr_t *instruction);

void or_al_imm8(class cpu_i386 *proc, x86_instr_t *instruction);

void or_r8_rm8(class cpu_i386 *proc, x86_instr_t *instruction);

void and_rm8_r8(class cpu_i386 *proc, x86_instr_t *instruction);

void and_r8_rm8(class cpu_i386 *proc, x86_instr_t *instruction);

void and_al_imm8(class cpu_i386 *proc, x86_instr_t *instruction);

void sub_rm8_r8(class cpu_i386 *proc, x86_instr_t *instruction);

void sub_r8_rm8(class cpu_i386 *proc, x86_instr_t *instruction);

void sub_al_imm8(class cpu_i386 *proc, x86_instr_t *instruction);

void xor_rm8_r8(class cpu_i386 *proc, x86_instr_t *instruction);

void xor_r8_rm8(class cpu_i386 *proc, x86_instr_t *instruction);

void xor_al_imm8(class cpu_i386 *proc, x86_instr_t *instruction);

void cmp_rm8_r8(class cpu_i386 *proc, x86_instr_t *instruction);

void cmp_r8_rm8(class cpu_i386 *proc, x86_instr_t *instruction);

void cmp_al_imm8(class cpu_i386 *proc, x86_instr_t *instruction);

#define JCC_REL8_DECL(cc, is_flag) \
void j ## cc ## _rel8(class cpu_i386 *proc, x86_instr_t *instruction);

JCC_REL8_DECL(o, proc->regs.flags_register.is_overflow())

JCC_REL8_DECL(no, !proc->regs.flags_register.is_overflow())

JCC_REL8_DECL(b, proc->regs.flags_register.is_carry())

JCC_REL8_DECL(nb, !proc->regs.flags_register.is_carry())

JCC_REL8_DECL(z, proc->regs.flags_register.is_zero())

JCC_REL8_DECL(nz, !proc->regs.flags_register.is_zero())

JCC_REL8_DECL(be, proc->regs.flags_register.is_carry() || proc->regs.flags_register.is_zero())

JCC_REL8_DECL(a, !(proc->regs.flags_register.is_carry() || proc->regs.flags_register.is_zero()))

JCC_REL8_DECL(s, proc->regs.flags_register.is_sign())

JCC_REL8_DECL(ns, !proc->regs.flags_register.is_sign())

JCC_REL8_DECL(p, proc->regs.flags_register.is_parity())

JCC_REL8_DECL(np, !proc->regs.flags_register.is_parity())

JCC_REL8_DECL(l, proc->regs.flags_register.is_sign() != proc->regs.flags_register.is_overflow())

JCC_REL8_DECL(nl, proc->regs.flags_register.is_sign() == proc->regs.flags_register.is_overflow())

JCC_REL8_DECL(le, proc->regs.flags_register.is_zero() ||
                  (proc->regs.flags_register.is_sign() != proc->regs.flags_register.is_overflow()))

JCC_REL8_DECL(nle, !proc->regs.flags_register.is_zero() &&
                   (proc->regs.flags_register.is_sign() == proc->regs.flags_register.is_overflow()))

void test_rm8_r8(class cpu_i386 *proc, x86_instr_t *instruction);

void xchg_r8_rm8(class cpu_i386 *proc, x86_instr_t *instruction);

void mov_rm8_r8(class cpu_i386 *proc, x86_instr_t *instruction);

void mov_r8_rm8(class cpu_i386 *proc, x86_instr_t *instruction);

void mov_sreg_rm16(class cpu_i386 *proc, x86_instr_t *instruction);

void nop(class cpu_i386 *proc, x86_instr_t *instruction);

void mov_al_moffs8(class cpu_i386 *proc, x86_instr_t *instruction);

void mov_moffs8_al(class cpu_i386 *proc, x86_instr_t *instruction);

void test_al_imm8(class cpu_i386 *proc, x86_instr_t *instruction);

void mov_r8_imm8(class cpu_i386 *proc, x86_instr_t *instruction);

void mov_rm8_imm8(class cpu_i386 *proc, x86_instr_t *instruction);

//void retf(class cpu_i386 *proc, x86_instr_t *instr) {
//    EmuInstr::retf(proc);
//}

void int3(class cpu_i386 *proc, x86_instr_t *instruction);

void int_imm8(class cpu_i386 *proc, x86_instr_t *instruction);

//void iret(class cpu_i386 *proc, x86_instr_t *instr) {
//    EmuInstr::iret(proc);
//}

void in_al_imm8(class cpu_i386 *proc, x86_instr_t *instruction);

void out_imm8_al(class cpu_i386 *proc, x86_instr_t *instruction);

void jmp(class cpu_i386 *proc, x86_instr_t *instruction);

void in_al_dx(class cpu_i386 *proc, x86_instr_t *instruction);

void out_dx_al(class cpu_i386 *proc, x86_instr_t *instruction);

void cli(class cpu_i386 *proc, x86_instr_t *instruction);

void sti(class cpu_i386 *proc, x86_instr_t *instruction);

void cld(class cpu_i386 *proc, x86_instr_t *instruction);

void STD(class cpu_i386 *proc, x86_instr_t *instruction);

void hlt(class cpu_i386 *proc, x86_instr_t *instruction);

void ltr_rm16(class cpu_i386 *proc, x86_instr_t *instruction);

void mov_r32_crn(class cpu_i386 *proc, x86_instr_t *instruction);

void mov_crn_r32(class cpu_i386 *proc, x86_instr_t *instruction);

#define SETCC_RM8_DECL(cc, is_flag) \
void set ## cc ## _rm8(class cpu_i386 *proc, x86_instr_t *instruction);

SETCC_RM8_DECL(o, EFLAGS_OF)

SETCC_RM8_DECL(no, !EFLAGS_OF)

SETCC_RM8_DECL(b, EFLAGS_CF)

SETCC_RM8_DECL(nb, !EFLAGS_CF)

SETCC_RM8_DECL(z, EFLAGS_ZF)

SETCC_RM8_DECL(nz, !EFLAGS_ZF)

SETCC_RM8_DECL(be, EFLAGS_CF || EFLAGS_ZF)

SETCC_RM8_DECL(a, !(EFLAGS_CF || EFLAGS_ZF))

SETCC_RM8_DECL(s, EFLAGS_SF)

SETCC_RM8_DECL(ns, !EFLAGS_SF)

SETCC_RM8_DECL(p, EFLAGS_PF)

SETCC_RM8_DECL(np, !EFLAGS_PF)

SETCC_RM8_DECL(l, EFLAGS_SF != EFLAGS_OF)

SETCC_RM8_DECL(nl, EFLAGS_SF == EFLAGS_OF)

SETCC_RM8_DECL(le, EFLAGS_ZF || (EFLAGS_SF != EFLAGS_OF))

SETCC_RM8_DECL(nle, !EFLAGS_ZF && (EFLAGS_SF == EFLAGS_OF))


void code_80(class cpu_i386 *proc, x86_instr_t *instruction);

void code_82(class cpu_i386 *proc, x86_instr_t *instruction);

void code_c0(class cpu_i386 *proc, x86_instr_t *instruction);

void code_f6(class cpu_i386 *proc, x86_instr_t *instruction);


void add_rm8_imm8(class cpu_i386 *proc, x86_instr_t *instruction);

void or_rm8_imm8(class cpu_i386 *proc, x86_instr_t *instruction);

void adc_rm8_imm8(class cpu_i386 *proc, x86_instr_t *instruction);

void sbb_rm8_imm8(class cpu_i386 *proc, x86_instr_t *instruction);

void and_rm8_imm8(class cpu_i386 *proc, x86_instr_t *instruction);

void sub_rm8_imm8(class cpu_i386 *proc, x86_instr_t *instruction);

void xor_rm8_imm8(class cpu_i386 *proc, x86_instr_t *instruction);

void cmp_rm8_imm8(class cpu_i386 *proc, x86_instr_t *instruction);


void shl_rm8_imm8(class cpu_i386 *proc, x86_instr_t *instruction);

void shr_rm8_imm8(class cpu_i386 *proc, x86_instr_t *instruction);

void sal_rm8_imm8(class cpu_i386 *proc, x86_instr_t *instruction);

void sar_rm8_imm8(class cpu_i386 *proc, x86_instr_t *instruction);


void test_rm8_imm8(class cpu_i386 *proc, x86_instr_t *instruction);

void not_rm8(class cpu_i386 *proc, x86_instr_t *instruction);

void neg_rm8(class cpu_i386 *proc, x86_instr_t *instruction);

void mul_ax_al_rm8(class cpu_i386 *proc, x86_instr_t *instruction);

void imul_ax_al_rm8(class cpu_i386 *proc, x86_instr_t *instruction);

void div_al_ah_rm8(class cpu_i386 *proc, x86_instr_t *instruction);

void idiv_al_ah_rm8(class cpu_i386 *proc, x86_instr_t *instruction);

/******************************************************************/


void add_rm16_r16(class cpu_i386 *proc, x86_instr_t *instruction);

void add_r16_rm16(class cpu_i386 *proc, x86_instr_t *instruction);

void add_ax_imm16(class cpu_i386 *proc, x86_instr_t *instruction);

void push_es16(class cpu_i386 *proc, x86_instr_t *instruction);

void pop_es16(class cpu_i386 *proc, x86_instr_t *instruction);

void or_rm16_r16(class cpu_i386 *proc, x86_instr_t *instruction);

void or_r16_rm16(class cpu_i386 *proc, x86_instr_t *instruction);

void or_ax_imm16(class cpu_i386 *proc, x86_instr_t *instruction);

void adc_rm16_r16(class cpu_i386 *proc, x86_instr_t *instruction);

void push_ss16(class cpu_i386 *proc, x86_instr_t *instruction);

void pop_ss16(class cpu_i386 *proc, x86_instr_t *instruction);

void push_ds16(class cpu_i386 *proc, x86_instr_t *instruction);

void pop_ds16(class cpu_i386 *proc, x86_instr_t *instruction);

void and_rm16_r16(class cpu_i386 *proc, x86_instr_t *instruction);

void and_r16_rm16(class cpu_i386 *proc, x86_instr_t *instruction);

void and_ax_imm16(class cpu_i386 *proc, x86_instr_t *instruction);

void sub_rm16_r16(class cpu_i386 *proc, x86_instr_t *instruction);

void sub_r16_rm16(class cpu_i386 *proc, x86_instr_t *instruction);

void sub_ax_imm16(class cpu_i386 *proc, x86_instr_t *instruction);

void xor_rm16_r16(class cpu_i386 *proc, x86_instr_t *instruction);

void xor_r16_rm16(class cpu_i386 *proc, x86_instr_t *instruction);

void xor_ax_imm16(class cpu_i386 *proc, x86_instr_t *instruction);

void cmp_rm16_r16(class cpu_i386 *proc, x86_instr_t *instruction);

void cmp_r16_rm16(class cpu_i386 *proc, x86_instr_t *instruction);

void cmp_ax_imm16(class cpu_i386 *proc, x86_instr_t *instruction);

void inc_r16(class cpu_i386 *proc, x86_instr_t *instruction);

void dec_r16(class cpu_i386 *proc, x86_instr_t *instruction);

void push_r16(class cpu_i386 *proc, x86_instr_t *instruction);

void pop_r16(class cpu_i386 *proc, x86_instr_t *instruction);

void pusha(class cpu_i386 *proc, x86_instr_t *instruction);

void popa(class cpu_i386 *proc, x86_instr_t *instruction);

void push_imm16_16(class cpu_i386 *proc, x86_instr_t *instruction);

void imul_r16_rm16_imm16(class cpu_i386 *proc, x86_instr_t *instruction);

void push_imm8_16(class cpu_i386 *proc, x86_instr_t *instruction);

void imul_r16_rm16_imm8(class cpu_i386 *proc, x86_instr_t *instruction);

void test_rm16_r16(class cpu_i386 *proc, x86_instr_t *instruction);

void xchg_r16_rm16(class cpu_i386 *proc, x86_instr_t *instruction);

void mov_rm16_r16(class cpu_i386 *proc, x86_instr_t *instruction);

void mov_r16_rm16(class cpu_i386 *proc, x86_instr_t *instruction);

void mov_rm16_sreg(class cpu_i386 *proc, x86_instr_t *instruction);

void lea_r16_m16(class cpu_i386 *proc, x86_instr_t *instruction);

void xchg_r16_ax(class cpu_i386 *proc, x86_instr_t *instruction);

void cbw(class cpu_i386 *proc, x86_instr_t *instruction);

void cwd(class cpu_i386 *proc, x86_instr_t *instruction);

void callf_ptr16_16(class cpu_i386 *proc, x86_instr_t *instruction);

void pushf_16(class cpu_i386 *proc, x86_instr_t *instruction);

void popf_16(class cpu_i386 *proc, x86_instr_t *instruction);

void mov_ax_moffs16(class cpu_i386 *proc, x86_instr_t *instruction);

void mov_moffs16_ax(class cpu_i386 *proc, x86_instr_t *instruction);

void movsw_m16_m16(class cpu_i386 *proc, x86_instr_t *instruction);

void cmps_m8_m8_16(class cpu_i386 *proc, x86_instr_t *instruction);

void cmps_m16_m16(class cpu_i386 *proc, x86_instr_t *instruction);

void test_ax_imm16(class cpu_i386 *proc, x86_instr_t *instruction);

void lodsb(class cpu_i386 *proc, x86_instr_t *instruction);

void mov_r16_imm16(class cpu_i386 *proc, x86_instr_t *instruction);

void ret_16(class cpu_i386 *proc, x86_instr_t *instruction);

void mov_rm16_imm16(class cpu_i386 *proc, x86_instr_t *instruction);

void leave_16(class cpu_i386 *proc, x86_instr_t *instruction);

void in_ax_imm8(class cpu_i386 *proc, x86_instr_t *instruction);

void out_imm8_ax(class cpu_i386 *proc, x86_instr_t *instruction);

void call_rel16(class cpu_i386 *proc, x86_instr_t *instruction);

void jmp_rel16(class cpu_i386 *proc, x86_instr_t *instruction);

void jmpf_ptr16_16(class cpu_i386 *proc, x86_instr_t *instruction);

void in_ax_dx(class cpu_i386 *proc, x86_instr_t *instruction);

void out_dx_ax(class cpu_i386 *proc, x86_instr_t *instruction);

#define JCC_REL16_DECL(cc, is_flag) \
void j ## cc ## _rel16(class cpu_i386 *proc, x86_instr_t *instruction);

JCC_REL16_DECL(o, proc->regs.flags_register.is_overflow())

JCC_REL16_DECL(no, !proc->regs.flags_register.is_overflow())

JCC_REL16_DECL(b, proc->regs.flags_register.is_carry())

JCC_REL16_DECL(nb, !proc->regs.flags_register.is_carry())

JCC_REL16_DECL(z, proc->regs.flags_register.is_zero())

JCC_REL16_DECL(nz, !proc->regs.flags_register.is_zero())

JCC_REL16_DECL(be, proc->regs.flags_register.is_carry() || proc->regs.flags_register.is_zero())

JCC_REL16_DECL(a, !(proc->regs.flags_register.is_carry() || proc->regs.flags_register.is_zero()))

JCC_REL16_DECL(s, proc->regs.flags_register.is_sign())

JCC_REL16_DECL(ns, !proc->regs.flags_register.is_sign())

JCC_REL16_DECL(p, proc->regs.flags_register.is_parity())

JCC_REL16_DECL(np, !proc->regs.flags_register.is_parity())

JCC_REL16_DECL(l, proc->regs.flags_register.is_sign() != proc->regs.flags_register.is_overflow())

JCC_REL16_DECL(nl, proc->regs.flags_register.is_sign() == proc->regs.flags_register.is_overflow())

JCC_REL16_DECL(le, proc->regs.flags_register.is_zero() ||
                   (proc->regs.flags_register.is_sign() != proc->regs.flags_register.is_overflow()))

JCC_REL16_DECL(nle, !proc->regs.flags_register.is_zero() &&
                    (proc->regs.flags_register.is_sign() == proc->regs.flags_register.is_overflow()))

void imul_r16_rm16(class cpu_i386 *proc, x86_instr_t *instruction);

void movzx_r16_rm8(class cpu_i386 *proc, x86_instr_t *instruction);

void movzx_r16_rm16(class cpu_i386 *proc, x86_instr_t *instruction);

void movsx_r16_rm8(class cpu_i386 *proc, x86_instr_t *instruction);

void movsx_r16_rm16(class cpu_i386 *proc, x86_instr_t *instruction);


void code_81_16(class cpu_i386 *proc, x86_instr_t *instruction);

void code_83_16(class cpu_i386 *proc, x86_instr_t *instruction);

void code_c1_16(class cpu_i386 *proc, x86_instr_t *instruction);

void code_d3_16(class cpu_i386 *proc, x86_instr_t *instruction);

void code_f7_16(class cpu_i386 *proc, x86_instr_t *instruction);

void code_ff_16(class cpu_i386 *proc, x86_instr_t *instruction);

void code_0f00_16(class cpu_i386 *proc, x86_instr_t *instruction);

void code_0f01_16(class cpu_i386 *proc, x86_instr_t *instruction);


void add_rm16_imm16(class cpu_i386 *proc, x86_instr_t *instruction);

void or_rm16_imm16(class cpu_i386 *proc, x86_instr_t *instruction);

void adc_rm16_imm16(class cpu_i386 *proc, x86_instr_t *instruction);

void sbb_rm16_imm16(class cpu_i386 *proc, x86_instr_t *instruction);

void and_rm16_imm16(class cpu_i386 *proc, x86_instr_t *instruction);

void sub_rm16_imm16(class cpu_i386 *proc, x86_instr_t *instruction);

void xor_rm16_imm16(class cpu_i386 *proc, x86_instr_t *instruction);

void cmp_rm16_imm16(class cpu_i386 *proc, x86_instr_t *instruction);


void add_rm16_imm8(class cpu_i386 *proc, x86_instr_t *instruction);

void or_rm16_imm8(class cpu_i386 *proc, x86_instr_t *instruction);

void adc_rm16_imm8(class cpu_i386 *proc, x86_instr_t *instruction);

void sbb_rm16_imm8(class cpu_i386 *proc, x86_instr_t *instruction);

void and_rm16_imm8(class cpu_i386 *proc, x86_instr_t *instruction);

void sub_rm16_imm8(class cpu_i386 *proc, x86_instr_t *instruction);

void xor_rm16_imm8(class cpu_i386 *proc, x86_instr_t *instruction);

void cmp_rm16_imm8(class cpu_i386 *proc, x86_instr_t *instruction);


void shl_rm16_imm8(class cpu_i386 *proc, x86_instr_t *instruction);

void shr_rm16_imm8(class cpu_i386 *proc, x86_instr_t *instruction);

void sal_rm16_imm8(class cpu_i386 *proc, x86_instr_t *instruction);

void sar_rm16_imm8(class cpu_i386 *proc, x86_instr_t *instruction);


void shl_rm16_cl(class cpu_i386 *proc, x86_instr_t *instruction);

void shr_rm16_cl(class cpu_i386 *proc, x86_instr_t *instruction);

void sal_rm16_cl(class cpu_i386 *proc, x86_instr_t *instruction);

void sar_rm16_cl(class cpu_i386 *proc, x86_instr_t *instruction);


void test_rm16_imm16(class cpu_i386 *proc, x86_instr_t *instruction);

void not_rm16(class cpu_i386 *proc, x86_instr_t *instruction);

void neg_rm16(class cpu_i386 *proc, x86_instr_t *instruction);

void mul_dx_ax_rm16(class cpu_i386 *proc, x86_instr_t *instruction);

void imul_dx_ax_rm16(class cpu_i386 *proc, x86_instr_t *instruction);

void div_dx_ax_rm16(class cpu_i386 *proc, x86_instr_t *instruction);

void idiv_dx_ax_rm16(class cpu_i386 *proc, x86_instr_t *instruction);


void inc_rm16(class cpu_i386 *proc, x86_instr_t *instruction);

void dec_rm16(class cpu_i386 *proc, x86_instr_t *instruction);

void call_rm16(class cpu_i386 *proc, x86_instr_t *instruction);

void callf_m16_16(class cpu_i386 *proc, x86_instr_t *instruction);

void jmp_rm16(class cpu_i386 *proc, x86_instr_t *instruction);

void jmpf_m16_16(class cpu_i386 *proc, x86_instr_t *instruction);

void push_rm16(class cpu_i386 *proc, x86_instr_t *instruction);


void lgdt_m24(class cpu_i386 *proc, x86_instr_t *instruction);

void lidt_m24(class cpu_i386 *proc, x86_instr_t *instruction);

/******************************************************************/


void add_rm32_r32(class cpu_i386 *proc, x86_instr_t *instruction);

void add_r32_rm32(class cpu_i386 *proc, x86_instr_t *instruction);

void add_eax_imm32(class cpu_i386 *proc, x86_instr_t *instruction);

void push_es32(class cpu_i386 *proc, x86_instr_t *instruction);

void pop_es32(class cpu_i386 *proc, x86_instr_t *instruction);

void or_rm32_r32(class cpu_i386 *proc, x86_instr_t *instruction);

void or_r32_rm32(class cpu_i386 *proc, x86_instr_t *instruction);

void or_eax_imm32(class cpu_i386 *proc, x86_instr_t *instruction);

void push_ss32(class cpu_i386 *proc, x86_instr_t *instruction);

void pop_ss32(class cpu_i386 *proc, x86_instr_t *instruction);

void push_ds32(class cpu_i386 *proc, x86_instr_t *instruction);

void pop_ds32(class cpu_i386 *proc, x86_instr_t *instruction);

void and_rm32_r32(class cpu_i386 *proc, x86_instr_t *instruction);

void and_r32_rm32(class cpu_i386 *proc, x86_instr_t *instruction);

void and_eax_imm32(class cpu_i386 *proc, x86_instr_t *instruction);

void sub_rm32_r32(class cpu_i386 *proc, x86_instr_t *instruction);

void sub_r32_rm32(class cpu_i386 *proc, x86_instr_t *instruction);

void sub_eax_imm32(class cpu_i386 *proc, x86_instr_t *instruction);

void xor_rm32_r32(class cpu_i386 *proc, x86_instr_t *instruction);

void xor_r32_rm32(class cpu_i386 *proc, x86_instr_t *instruction);

void xor_eax_imm32(class cpu_i386 *proc, x86_instr_t *instruction);

void cmp_rm32_r32(class cpu_i386 *proc, x86_instr_t *instruction);

void cmp_r32_rm32(class cpu_i386 *proc, x86_instr_t *instruction);

void cmp_eax_imm32(class cpu_i386 *proc, x86_instr_t *instruction);

void inc_r32(class cpu_i386 *proc, x86_instr_t *instruction);

void dec_r32(class cpu_i386 *proc, x86_instr_t *instruction);

void push_r32(class cpu_i386 *proc, x86_instr_t *instruction);

void pop_r32(class cpu_i386 *proc, x86_instr_t *instruction);

void pushad(class cpu_i386 *proc, x86_instr_t *instruction);

void popad(class cpu_i386 *proc, x86_instr_t *instruction);

void push_imm32_32(class cpu_i386 *proc, x86_instr_t *instruction);

void imul_r32_rm32_imm32(class cpu_i386 *proc, x86_instr_t *instruction);

void push_imm8_32(class cpu_i386 *proc, x86_instr_t *instruction);

void imul_r32_rm32_imm8(class cpu_i386 *proc, x86_instr_t *instruction);

void test_rm32_r32(class cpu_i386 *proc, x86_instr_t *instruction);

void xchg_r32_rm32(class cpu_i386 *proc, x86_instr_t *instruction);

void mov_rm32_r32(class cpu_i386 *proc, x86_instr_t *instruction);

void mov_r32_rm32(class cpu_i386 *proc, x86_instr_t *instruction);

void mov_rm32_sreg(class cpu_i386 *proc, x86_instr_t *instruction);

void lea_r32_m32(class cpu_i386 *proc, x86_instr_t *instruction);

void xchg_r32_eax(class cpu_i386 *proc, x86_instr_t *instruction);

void cwde(class cpu_i386 *proc, x86_instr_t *instruction);

void cdq(class cpu_i386 *proc, x86_instr_t *instruction);

void callf_ptr16_32(class cpu_i386 *proc, x86_instr_t *instruction);

void pushf_32(class cpu_i386 *proc, x86_instr_t *instruction);

void popf_32(class cpu_i386 *proc, x86_instr_t *instruction);

void mov_eax_moffs32(class cpu_i386 *proc, x86_instr_t *instruction);

void mov_moffs32_eax(class cpu_i386 *proc, x86_instr_t *instruction);

void cmps_m8_m8_32(class cpu_i386 *proc, x86_instr_t *instruction);

void cmps_m32_m32(class cpu_i386 *proc, x86_instr_t *instruction);

void test_eax_imm32(class cpu_i386 *proc, x86_instr_t *instruction);

void mov_r32_imm32(class cpu_i386 *proc, x86_instr_t *instruction);

void ret_32(class cpu_i386 *proc, x86_instr_t *instruction);

void mov_rm32_imm32(class cpu_i386 *proc, x86_instr_t *instruction);

void leave_32(class cpu_i386 *proc, x86_instr_t *instruction);

void in_eax_imm8(class cpu_i386 *proc, x86_instr_t *instruction);

void out_imm8_eax(class cpu_i386 *proc, x86_instr_t *instruction);

void call_rel32(class cpu_i386 *proc, x86_instr_t *instruction);

void jmp_rel32(class cpu_i386 *proc, x86_instr_t *instruction);

void jmpf_ptr16_32(class cpu_i386 *proc, x86_instr_t *instruction);

void in_eax_dx(class cpu_i386 *proc, x86_instr_t *instruction);

void out_dx_eax(class cpu_i386 *proc, x86_instr_t *instruction);

#define JCC_REL32_DECL(cc, is_flag) \
void j ## cc ## _rel32(class cpu_i386 *proc, x86_instr_t *instruction);

JCC_REL32_DECL(o, proc->regs.flags_register.is_overflow())

JCC_REL32_DECL(no, !proc->regs.flags_register.is_overflow())

JCC_REL32_DECL(b, proc->regs.flags_register.is_carry())

JCC_REL32_DECL(nb, !proc->regs.flags_register.is_carry())

JCC_REL32_DECL(z, proc->regs.flags_register.is_zero())

JCC_REL32_DECL(nz, !proc->regs.flags_register.is_zero())

JCC_REL32_DECL(be, proc->regs.flags_register.is_carry() || proc->regs.flags_register.is_zero())

JCC_REL32_DECL(a, !(proc->regs.flags_register.is_carry() || proc->regs.flags_register.is_zero()))

JCC_REL32_DECL(s, proc->regs.flags_register.is_sign())

JCC_REL32_DECL(ns, !proc->regs.flags_register.is_sign())

JCC_REL32_DECL(p, proc->regs.flags_register.is_parity())

JCC_REL32_DECL(np, !proc->regs.flags_register.is_parity())

JCC_REL32_DECL(l, proc->regs.flags_register.is_sign() != proc->regs.flags_register.is_overflow())

JCC_REL32_DECL(nl, proc->regs.flags_register.is_sign() == proc->regs.flags_register.is_overflow())

JCC_REL32_DECL(le, proc->regs.flags_register.is_zero() ||
                   (proc->regs.flags_register.is_sign() != proc->regs.flags_register.is_overflow()))

JCC_REL32_DECL(nle, !proc->regs.flags_register.is_zero() &&
                    (proc->regs.flags_register.is_sign() == proc->regs.flags_register.is_overflow()))

void imul_r32_rm32(class cpu_i386 *proc, x86_instr_t *instruction);

void movzx_r32_rm8(class cpu_i386 *proc, x86_instr_t *instruction);

void movzx_r32_rm16(class cpu_i386 *proc, x86_instr_t *instruction);

void movsx_r32_rm8(class cpu_i386 *proc, x86_instr_t *instruction);

void movsx_r32_rm16(class cpu_i386 *proc, x86_instr_t *instruction);


void code_81_32(class cpu_i386 *proc, x86_instr_t *instruction);

void code_83_32(class cpu_i386 *proc, x86_instr_t *instruction);

void code_c1_32(class cpu_i386 *proc, x86_instr_t *instruction);

void code_d3_32(class cpu_i386 *proc, x86_instr_t *instruction);

void code_f7_32(class cpu_i386 *proc, x86_instr_t *instruction);

void code_ff_32(class cpu_i386 *proc, x86_instr_t *instruction);

void code_0f00_32(class cpu_i386 *proc, x86_instr_t *instruction);

void code_0f01_32(class cpu_i386 *proc, x86_instr_t *instruction);


void add_rm32_imm32(class cpu_i386 *proc, x86_instr_t *instruction);

void or_rm32_imm32(class cpu_i386 *proc, x86_instr_t *instruction);

void adc_rm32_imm32(class cpu_i386 *proc, x86_instr_t *instruction);

void sbb_rm32_imm32(class cpu_i386 *proc, x86_instr_t *instruction);

void and_rm32_imm32(class cpu_i386 *proc, x86_instr_t *instruction);

void sub_rm32_imm32(class cpu_i386 *proc, x86_instr_t *instruction);

void xor_rm32_imm32(class cpu_i386 *proc, x86_instr_t *instruction);

void cmp_rm32_imm32(class cpu_i386 *proc, x86_instr_t *instruction);


void add_rm32_imm8(class cpu_i386 *proc, x86_instr_t *instruction);

void or_rm32_imm8(class cpu_i386 *proc, x86_instr_t *instruction);

void adc_rm32_imm8(class cpu_i386 *proc, x86_instr_t *instruction);

void sbb_rm32_imm8(class cpu_i386 *proc, x86_instr_t *instruction);

void and_rm32_imm8(class cpu_i386 *proc, x86_instr_t *instruction);

void sub_rm32_imm8(class cpu_i386 *proc, x86_instr_t *instruction);

void xor_rm32_imm8(class cpu_i386 *proc, x86_instr_t *instruction);

void cmp_rm32_imm8(class cpu_i386 *proc, x86_instr_t *instruction);


void shl_rm32_imm8(class cpu_i386 *proc, x86_instr_t *instruction);

void shr_rm32_imm8(class cpu_i386 *proc, x86_instr_t *instruction);

void sal_rm32_imm8(class cpu_i386 *proc, x86_instr_t *instruction);

void sar_rm32_imm8(class cpu_i386 *proc, x86_instr_t *instruction);


void shl_rm32_cl(class cpu_i386 *proc, x86_instr_t *instruction);

void shr_rm32_cl(class cpu_i386 *proc, x86_instr_t *instruction);

void sal_rm32_cl(class cpu_i386 *proc, x86_instr_t *instruction);

void sar_rm32_cl(class cpu_i386 *proc, x86_instr_t *instruction);


void test_rm32_imm32(class cpu_i386 *proc, x86_instr_t *instruction);

void not_rm32(class cpu_i386 *proc, x86_instr_t *instruction);

void neg_rm32(class cpu_i386 *proc, x86_instr_t *instruction);

void mul_edx_eax_rm32(class cpu_i386 *proc, x86_instr_t *instruction);

void imul_edx_eax_rm32(class cpu_i386 *proc, x86_instr_t *instruction);

void div_edx_eax_rm32(class cpu_i386 *proc, x86_instr_t *instruction);

void idiv_edx_eax_rm32(class cpu_i386 *proc, x86_instr_t *instruction);


void inc_rm32(class cpu_i386 *proc, x86_instr_t *instruction);

void dec_rm32(class cpu_i386 *proc, x86_instr_t *instruction);

void call_rm32(class cpu_i386 *proc, x86_instr_t *instruction);

void callf_m16_32(class cpu_i386 *proc, x86_instr_t *instruction);

void jmp_rm32(class cpu_i386 *proc, x86_instr_t *instruction);

void jmpf_m16_32(class cpu_i386 *proc, x86_instr_t *instruction);

void push_rm32(class cpu_i386 *proc, x86_instr_t *instruction);


void lgdt_m32(class cpu_i386 *proc, x86_instr_t *instruction);

void lidt_m32(class cpu_i386 *proc, x86_instr_t *instruction);


#endif
