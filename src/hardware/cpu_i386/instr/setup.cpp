#include "setup.h"

#include "x86_instruction.h"

#include "instr_decoder.h"
#include "instr_executer.h"

#include "helper_funcs.h"
#include "instr_handlers.h"

void setup_common();

void setup16();

void setup32();

void setup_instr_handlers() {
    setup_common();
    setup16();
    setup32();
}

void setup_opcode_common(opcode_t opcode, instr_handler handler, uint8_t flags) {
    opcode_handlers_common[opcode] = handler;
    opcode_flags_common[opcode].raw = flags;
}

void setup_opcode_mode16(opcode_t opcode, instr_handler handler, uint8_t flags) {
    opcode_handlers_mode16[opcode] = handler;
    opcode_flags_mode16[opcode].raw = flags;
}

void setup_opcode_mode32(opcode_t opcode, instr_handler handler, uint8_t flags) {
    opcode_handlers_mode32[opcode] = handler;
    opcode_flags_mode32[opcode].raw = flags;
}

void setup_common() {
    setup_opcode_common(0x00, &add_rm8_r8, CHK_MODRM);
    setup_opcode_common(0x02, &add_r8_rm8, CHK_MODRM);
    setup_opcode_common(0x04, &add_al_imm8, CHK_IMM8);
    setup_opcode_common(0x08, &or_rm8_r8, CHK_MODRM);
    setup_opcode_common(0x0a, &or_r8_rm8, CHK_MODRM);
    setup_opcode_common(0x0c, &or_al_imm8, CHK_IMM8);
    setup_opcode_common(0x20, &and_rm8_r8, CHK_MODRM);
    setup_opcode_common(0x22, &and_r8_rm8, CHK_MODRM);
    setup_opcode_common(0x24, &and_al_imm8, CHK_IMM8);
    setup_opcode_common(0x28, &sub_rm8_r8, CHK_MODRM);
    setup_opcode_common(0x2a, &sub_r8_rm8, CHK_MODRM);
    setup_opcode_common(0x2c, &sub_al_imm8, CHK_IMM8);
    setup_opcode_common(0x30, &xor_rm8_r8, CHK_MODRM);
    setup_opcode_common(0x32, &xor_r8_rm8, CHK_MODRM);
    setup_opcode_common(0x34, &xor_al_imm8, CHK_IMM8);
    setup_opcode_common(0x38, &cmp_rm8_r8, CHK_MODRM);
    setup_opcode_common(0x3a, &cmp_r8_rm8, CHK_MODRM);
    setup_opcode_common(0x3c, &cmp_al_imm8, CHK_IMM8);
    setup_opcode_common(0x70, &jo_rel8, CHK_IMM8);
    setup_opcode_common(0x71, &jno_rel8, CHK_IMM8);
    setup_opcode_common(0x72, &jb_rel8, CHK_IMM8);
    setup_opcode_common(0x73, &jnb_rel8, CHK_IMM8);
    setup_opcode_common(0x74, &jz_rel8, CHK_IMM8);
    setup_opcode_common(0x75, &jnz_rel8, CHK_IMM8);
    setup_opcode_common(0x76, &jbe_rel8, CHK_IMM8);
    setup_opcode_common(0x77, &ja_rel8, CHK_IMM8);
    setup_opcode_common(0x78, &js_rel8, CHK_IMM8);
    setup_opcode_common(0x79, &jns_rel8, CHK_IMM8);
    setup_opcode_common(0x7a, &jp_rel8, CHK_IMM8);
    setup_opcode_common(0x7b, &jnp_rel8, CHK_IMM8);
    setup_opcode_common(0x7c, &jl_rel8, CHK_IMM8);
    setup_opcode_common(0x7d, &jnl_rel8, CHK_IMM8);
    setup_opcode_common(0x7e, &jle_rel8, CHK_IMM8);
    setup_opcode_common(0x7f, &jnle_rel8, CHK_IMM8);
    setup_opcode_common(0x84, &test_rm8_r8, CHK_MODRM);
    setup_opcode_common(0x86, &xchg_r8_rm8, CHK_MODRM);
    setup_opcode_common(0x88, &mov_rm8_r8, CHK_MODRM);
    setup_opcode_common(0x8a, &mov_r8_rm8, CHK_MODRM);
    setup_opcode_common(0x8e, &mov_sreg_rm16, CHK_MODRM);

    setup_opcode_common(0x90, &nop, 0);

    setup_opcode_common(0xa0, &mov_al_moffs8, CHK_MOFFS);
    setup_opcode_common(0xa2, &mov_moffs8_al, CHK_MOFFS);
    setup_opcode_common(0xa8, &test_al_imm8, CHK_IMM8);

    for (int i = 0; i < 8; i++)
        setup_opcode_common(0xb0 + i, &mov_r8_imm8, CHK_IMM8);

    setup_opcode_common(0xc6, &mov_rm8_imm8, CHK_MODRM | CHK_IMM8);
    setup_opcode_common(0xcb, &retf, 0);
    setup_opcode_common(0xcc, &int3, 0);
    setup_opcode_common(0xcd, &int_imm8, CHK_IMM8);
    setup_opcode_common(0xcf, &iret, 0);
    setup_opcode_common(0xe4, &in_al_imm8, CHK_IMM8);
    setup_opcode_common(0xe6, &out_imm8_al, CHK_IMM8);
    setup_opcode_common(0xeb, &jmp, CHK_IMM8);
    setup_opcode_common(0xec, &in_al_dx, 0);
    setup_opcode_common(0xee, &out_dx_al, 0);

    setup_opcode_common(0xf4, &hlt, 0);

    setup_opcode_common(0xfa, &cli, 0);
    setup_opcode_common(0xfb, &sti, 0);
    setup_opcode_common(0xfc, &cld, 0);
    setup_opcode_common(0xfd, &STD, 0);

    setup_opcode_common(0x0f20, &mov_r32_crn, CHK_MODRM);
    setup_opcode_common(0x0f22, &mov_crn_r32, CHK_MODRM);
    setup_opcode_common(0x0f90, &seto_rm8, CHK_MODRM);
    setup_opcode_common(0x0f91, &setno_rm8, CHK_MODRM);
    setup_opcode_common(0x0f92, &setb_rm8, CHK_MODRM);
    setup_opcode_common(0x0f93, &setnb_rm8, CHK_MODRM);
    setup_opcode_common(0x0f94, &setz_rm8, CHK_MODRM);
    setup_opcode_common(0x0f95, &setnz_rm8, CHK_MODRM);
    setup_opcode_common(0x0f96, &setbe_rm8, CHK_MODRM);
    setup_opcode_common(0x0f97, &seta_rm8, CHK_MODRM);
    setup_opcode_common(0x0f98, &sets_rm8, CHK_MODRM);
    setup_opcode_common(0x0f99, &setns_rm8, CHK_MODRM);
    setup_opcode_common(0x0f9a, &setp_rm8, CHK_MODRM);
    setup_opcode_common(0x0f9b, &setnp_rm8, CHK_MODRM);
    setup_opcode_common(0x0f9c, &setl_rm8, CHK_MODRM);
    setup_opcode_common(0x0f9d, &setnl_rm8, CHK_MODRM);
    setup_opcode_common(0x0f9e, &setle_rm8, CHK_MODRM);
    setup_opcode_common(0x0f9f, &setnle_rm8, CHK_MODRM);


    setup_opcode_common(0x80, &code_80, CHK_MODRM | CHK_IMM8);
    setup_opcode_common(0x82, &code_82, CHK_MODRM | CHK_IMM8);
    setup_opcode_common(0xc0, &code_c0, CHK_MODRM | CHK_IMM8);
    setup_opcode_common(0xf6, &code_f6, CHK_MODRM);
}

void setup16() {
    // 0x00 : add_rm8_r8
    setup_opcode_mode16(0x01, &add_rm16_r16, CHK_MODRM);
    // 0x02 : add_r8_rm8
    setup_opcode_mode16(0x03, &add_r16_rm16, CHK_MODRM);
    // 0x04 : add_al_imm8
    setup_opcode_mode16(0x05, &add_ax_imm16, CHK_IMM16);
    setup_opcode_mode16(0x06, &push_es16, 0);
    setup_opcode_mode16(0x07, &pop_es16, 0);
    // 0x08 : or_rm8_r8
    setup_opcode_mode16(0x09, &or_rm16_r16, CHK_MODRM);
    // 0x0a : or_r8_rm8
    setup_opcode_mode16(0x0b, &or_r16_rm16, CHK_MODRM);
    // 0x0c : or_al_imm8
    setup_opcode_mode16(0x0d, &or_ax_imm16, CHK_IMM16);

    setup_opcode_mode16(0x11, &adc_rm16_r16, CHK_MODRM);

    setup_opcode_mode16(0x16, &push_ss16, 0);
    setup_opcode_mode16(0x17, &pop_ss16, 0);

    setup_opcode_mode16(0x1e, &push_ds16, 0);
    setup_opcode_mode16(0x1f, &pop_ds16, 0);
    // 0x20 : and_rm8_r8
    setup_opcode_mode16(0x21, &and_rm16_r16, CHK_MODRM);
    // 0x22 : and_r8_rm8
    setup_opcode_mode16(0x23, &and_r16_rm16, CHK_MODRM);
    // 0x24 : and_al_imm8
    setup_opcode_mode16(0x25, &and_ax_imm16, CHK_IMM16);

    // 0x28 : sub_rm8_r8
    setup_opcode_mode16(0x29, &sub_rm16_r16, CHK_MODRM);
    // 0x2a : sub_r8_rm8
    setup_opcode_mode16(0x2b, &sub_r16_rm16, CHK_MODRM);
    // 0x2c : sub_al_imm8
    setup_opcode_mode16(0x2d, &sub_ax_imm16, CHK_IMM16);

    // 0x30 : xor_rm8_r8
    setup_opcode_mode16(0x31, &xor_rm16_r16, CHK_MODRM);
    // 0x16 : xor_r8_rm8
    setup_opcode_mode16(0x33, &xor_r16_rm16, CHK_MODRM);
    // 0x34 : xor_al_imm8
    setup_opcode_mode16(0x35, &xor_ax_imm16, CHK_IMM16);

    // 0x38 : cmp_rm8_r8
    setup_opcode_mode16(0x39, &cmp_rm16_r16, CHK_MODRM);
    // 0x3a : cmp_r8_rm8
    setup_opcode_mode16(0x3b, &cmp_r16_rm16, CHK_MODRM);
    // 0x3c : cmp_al_imm8
    setup_opcode_mode16(0x3d, &cmp_ax_imm16, CHK_IMM16);

    for (int i = 0; i < 8; i++)
        setup_opcode_mode16(0x40 + i, &inc_r16, 0);
    for (int i = 0; i < 8; i++)
        setup_opcode_mode16(0x48 + i, &dec_r16, 0);
    for (int i = 0; i < 8; i++)
        setup_opcode_mode16(0x50 + i, &push_r16, 0);
    for (int i = 0; i < 8; i++)
        setup_opcode_mode16(0x58 + i, &pop_r16, 0);

    setup_opcode_mode16(0x60, &pusha, 0);
    setup_opcode_mode16(0x61, &popa, 0);

    setup_opcode_mode16(0x68, &push_imm16_16, CHK_IMM16);
    setup_opcode_mode16(0x69, &imul_r16_rm16_imm16, CHK_MODRM | CHK_IMM16);
    setup_opcode_mode16(0x6a, &push_imm8_16, CHK_IMM8);
    setup_opcode_mode16(0x6b, &imul_r16_rm16_imm8, CHK_MODRM | CHK_IMM8);

    // 0x70-0x7f : jcc

    // 0x84 : test_rm8_r8
    setup_opcode_mode16(0x85, &test_rm16_r16, CHK_MODRM);
    // 0x86 : xchg_r8_rm8
    setup_opcode_mode16(0x87, &xchg_r16_rm16, CHK_MODRM);
    // 0x88 : mov_rm8_r8
    setup_opcode_mode16(0x89, &mov_rm16_r16, CHK_MODRM);
    // 0x8a : mov_r8_rm8
    setup_opcode_mode16(0x8b, &mov_r16_rm16, CHK_MODRM);
    setup_opcode_mode16(0x8c, &mov_rm16_sreg, CHK_MODRM);
    setup_opcode_mode16(0x8d, &lea_r16_m16, CHK_MODRM);
    // 0x8e : mov_sreg_rm16

    // 0x90 : nop
    for (int i = 1; i < 8; i++)
        setup_opcode_mode16(0x90 + i, &xchg_r16_ax, CHK_IMM16);
    setup_opcode_mode16(0x98, &cbw, 0);
    setup_opcode_mode16(0x99, &cwd, 0);
    setup_opcode_mode16(0x9a, &callf_ptr16_16, CHK_PTR16 | CHK_IMM16);

    setup_opcode_mode16(0x9c, &pushf_16, 0);
    setup_opcode_mode16(0x9d, &popf_16, 0);

    // 0xa0 : mov_al_moffs8
    setup_opcode_mode16(0xa1, &mov_ax_moffs16, CHK_MOFFS);
    // 0xa2 : mov_moffs8_al
    setup_opcode_mode16(0xa3, &mov_moffs16_ax, CHK_MOFFS);

    setup_opcode_mode16(0xa5, &movsw_m16_m16, 0);

    setup_opcode_mode16(0xa6, &cmps_m8_m8_16, 0);
    setup_opcode_mode16(0xa7, &cmps_m16_m16, 0);
    // 0xa8 : test_al_imm8
    setup_opcode_mode16(0xa9, &test_ax_imm16, CHK_IMM16);

    setup_opcode_mode16(0xac, &lodsb, 0);

    // 0xb0-0xb7 : mov_r8_imm
    for (int i = 0; i < 8; i++)
        setup_opcode_mode16(0xb8 + i, &mov_r16_imm16, CHK_IMM16);

    setup_opcode_mode16(0xc3, &ret_16, 0);

    setup_opcode_mode16(0xc7, &mov_rm16_imm16, CHK_MODRM | CHK_IMM16);

    setup_opcode_mode16(0xc9, &leave_16, 0);

    // 0xcb : retf
    // 0xcc : int3
    // 0xcd : int_imm8

    // 0xcf : iret

    // 0xe4 : in_al_imm8
    setup_opcode_mode16(0xe5, &in_ax_imm8, CHK_IMM8);
    // 0xe6 : out_imm8_al
    setup_opcode_mode16(0xe7, &out_imm8_ax, CHK_IMM8);
    setup_opcode_mode16(0xe8, &call_rel16, CHK_IMM16);
    setup_opcode_mode16(0xe9, &jmp_rel16, CHK_IMM16);
    setup_opcode_mode16(0xea, &jmpf_ptr16_16, CHK_PTR16 | CHK_IMM16);
    // 0xeb : jmp
    // 0xec : in_al_dx
    setup_opcode_mode16(0xed, &in_ax_dx, 0);
    // 0xee : out_dx_al
    setup_opcode_mode16(0xef, &out_dx_ax, 0);

    setup_opcode_mode16(0x0f80, &jo_rel16, CHK_IMM16);
    setup_opcode_mode16(0x0f81, &jno_rel16, CHK_IMM16);
    setup_opcode_mode16(0x0f82, &jb_rel16, CHK_IMM16);
    setup_opcode_mode16(0x0f83, &jnb_rel16, CHK_IMM16);
    setup_opcode_mode16(0x0f84, &jz_rel16, CHK_IMM16);
    setup_opcode_mode16(0x0f85, &jnz_rel16, CHK_IMM16);
    setup_opcode_mode16(0x0f86, &jbe_rel16, CHK_IMM16);
    setup_opcode_mode16(0x0f87, &ja_rel16, CHK_IMM16);
    setup_opcode_mode16(0x0f88, &js_rel16, CHK_IMM16);
    setup_opcode_mode16(0x0f89, &jns_rel16, CHK_IMM16);
    setup_opcode_mode16(0x0f8a, &jp_rel16, CHK_IMM16);
    setup_opcode_mode16(0x0f8b, &jnp_rel16, CHK_IMM16);
    setup_opcode_mode16(0x0f8c, &jl_rel16, CHK_IMM16);
    setup_opcode_mode16(0x0f8d, &jnl_rel16, CHK_IMM16);
    setup_opcode_mode16(0x0f8e, &jle_rel16, CHK_IMM16);
    setup_opcode_mode16(0x0f8f, &jnle_rel16, CHK_IMM16);

    setup_opcode_mode16(0x0faf, &imul_r16_rm16, CHK_MODRM);

    setup_opcode_mode16(0x0fb6, &movzx_r16_rm8, CHK_MODRM);
    setup_opcode_mode16(0x0fb7, &movzx_r16_rm16, CHK_MODRM);

    setup_opcode_mode16(0x0fbe, &movsx_r16_rm8, CHK_MODRM);
    setup_opcode_mode16(0x0fbf, &movsx_r16_rm16, CHK_MODRM);

    // 0x80 : code_80
    setup_opcode_mode16(0x81, &code_81_16, CHK_MODRM | CHK_IMM16);
    // 0x82 : code_82
    setup_opcode_mode16(0x83, &code_83_16, CHK_MODRM | CHK_IMM8);
    // 0xc0 : code_c0
    setup_opcode_mode16(0xc1, &code_c1_16, CHK_MODRM | CHK_IMM8);
    setup_opcode_mode16(0xd3, &code_d3_16, CHK_MODRM);
    setup_opcode_mode16(0xf7, &code_f7_16, CHK_MODRM);
    setup_opcode_mode16(0xff, &code_ff_16, CHK_MODRM);
    setup_opcode_mode16(0x0f00, &code_0f00_16, CHK_MODRM);
    setup_opcode_mode16(0x0f01, &code_0f01_16, CHK_MODRM);
}

void setup32() {
    // 0x00 : add_rm8_r8
    setup_opcode_mode32(0x01, &add_rm32_r32, CHK_MODRM);
    // 0x02 : add_r8_rm8
    setup_opcode_mode32(0x03, &add_r32_rm32, CHK_MODRM);
    // 0x04 : add_al_imm8
    setup_opcode_mode32(0x05, &add_eax_imm32, CHK_IMM32);
    setup_opcode_mode32(0x06, &push_es32, 0);
    setup_opcode_mode32(0x07, &pop_es32, 0);
    // 0x08 : or_rm8_r8
    setup_opcode_mode32(0x09, &or_rm32_r32, CHK_MODRM);
    // 0x0a : or_r8_rm8
    setup_opcode_mode32(0x0b, &or_r32_rm32, CHK_MODRM);
    // 0x0c : or_al_imm8
    setup_opcode_mode32(0x0d, &or_eax_imm32, CHK_IMM32);

    setup_opcode_mode32(0x16, &push_ss32, 0);
    setup_opcode_mode32(0x17, &pop_ss32, 0);

    setup_opcode_mode32(0x1e, &push_ds32, 0);
    setup_opcode_mode32(0x1f, &pop_ds32, 0);
    // 0x20 : and_rm8_r8
    setup_opcode_mode32(0x21, &and_rm32_r32, CHK_MODRM);
    // 0x22 : and_r8_rm8
    setup_opcode_mode32(0x23, &and_r32_rm32, CHK_MODRM);
    // 0x24 : and_al_imm8
    setup_opcode_mode32(0x25, &and_eax_imm32, CHK_IMM32);

    // 0x28 : sub_rm8_r8
    setup_opcode_mode32(0x29, &sub_rm32_r32, CHK_MODRM);
    // 0x2a : sub_r8_rm8
    setup_opcode_mode32(0x2b, &sub_r32_rm32, CHK_MODRM);
    // 0x2c : sub_al_imm8
    setup_opcode_mode32(0x2d, &sub_eax_imm32, CHK_IMM32);

    // 0x30 : xor_rm8_r8
    setup_opcode_mode32(0x31, &xor_rm32_r32, CHK_MODRM);
    // 0x32 : xor_r8_rm8
    setup_opcode_mode32(0x33, &xor_r32_rm32, CHK_MODRM);
    // 0x34 : xor_al_imm8
    setup_opcode_mode32(0x35, &xor_eax_imm32, CHK_IMM32);

    // 0x38 : cmp_rm8_r8
    setup_opcode_mode32(0x39, &cmp_rm32_r32, CHK_MODRM);
    // 0x3a : cmp_r8_rm8
    setup_opcode_mode32(0x3b, &cmp_r32_rm32, CHK_MODRM);
    // 0x3c : cmp_al_imm8
    setup_opcode_mode32(0x3d, &cmp_eax_imm32, CHK_IMM32);

    for (int i = 0; i < 8; i++)
        setup_opcode_mode32(0x40 + i, &inc_r32, 0);
    for (int i = 0; i < 8; i++)
        setup_opcode_mode32(0x48 + i, &dec_r32, 0);
    for (int i = 0; i < 8; i++)
        setup_opcode_mode32(0x50 + i, &push_r32, 0);
    for (int i = 0; i < 8; i++)
        setup_opcode_mode32(0x58 + i, &pop_r32, 0);

    setup_opcode_mode32(0x60, &pushad, 0);
    setup_opcode_mode32(0x61, &popad, 0);

    setup_opcode_mode32(0x68, &push_imm32_32, CHK_IMM32);
    setup_opcode_mode32(0x69, &imul_r32_rm32_imm32, CHK_MODRM | CHK_IMM32);
    setup_opcode_mode32(0x6a, &push_imm8_32, CHK_IMM8);
    setup_opcode_mode32(0x6b, &imul_r32_rm32_imm8, CHK_MODRM | CHK_IMM8);

    // 0x70-0x7f : jcc

    // 0x84 : test_rm8_r8
    setup_opcode_mode32(0x85, &test_rm32_r32, CHK_MODRM);
    // 0x86 : xchg_r8_rm8
    setup_opcode_mode32(0x87, &xchg_r32_rm32, CHK_MODRM);
    // 0x88 : mov_rm8_r8
    setup_opcode_mode32(0x89, &mov_rm32_r32, CHK_MODRM);
    // 0x8a : mov_r8_rm8
    setup_opcode_mode32(0x8b, &mov_r32_rm32, CHK_MODRM);
    setup_opcode_mode32(0x8c, &mov_rm32_sreg, CHK_MODRM);
    setup_opcode_mode32(0x8d, &lea_r32_m32, CHK_MODRM);
    // 0x8e : mov_sreg_rm16

    // 0x90 : nop
    for (int i = 1; i < 8; i++)
        setup_opcode_mode32(0x90 + i, &xchg_r32_eax, CHK_IMM32);
    setup_opcode_mode32(0x98, &cwde, 0);
    setup_opcode_mode32(0x99, &cdq, 0);
    setup_opcode_mode32(0x9a, &callf_ptr16_32, CHK_PTR16 | CHK_IMM32);

    setup_opcode_mode32(0x9c, &pushf_32, 0);
    setup_opcode_mode32(0x9d, &popf_32, 0);

    // 0xa0 : mov_al_moffs8
    setup_opcode_mode32(0xa1, &mov_eax_moffs32, CHK_MOFFS);
    // 0xa2 : mov_moffs8_al
    setup_opcode_mode32(0xa3, &mov_moffs32_eax, CHK_MOFFS);

    setup_opcode_mode32(0xa6, &cmps_m8_m8_32, 0);
    setup_opcode_mode32(0xa7, &cmps_m32_m32, 0);
    // 0xa8 : test_al_imm8
    setup_opcode_mode32(0xa9, &test_eax_imm32, CHK_IMM32);

    // 0xb0-0xb7 : mov_r8_imm
    for (int i = 0; i < 8; i++)
        setup_opcode_mode32(0xb8 + i, &mov_r32_imm32, CHK_IMM32);

    setup_opcode_mode32(0xc3, &ret_32, 0);

    setup_opcode_mode32(0xc7, &mov_rm32_imm32, CHK_MODRM | CHK_IMM32);

    setup_opcode_mode32(0xc9, &leave_32, 0);

    // 0xcb : retf
    // 0xcc : int3
    // 0xcd : int_imm8

    // 0xcf : iret

    // 0xe4 : in_al_imm8
    setup_opcode_mode32(0xe5, &in_eax_imm8, CHK_IMM8);
    // 0xe6 : out_imm8_al
    setup_opcode_mode32(0xe7, &out_imm8_eax, CHK_IMM8);
    setup_opcode_mode32(0xe8, &call_rel32, CHK_IMM32);
    setup_opcode_mode32(0xe9, &jmp_rel32, CHK_IMM32);
    setup_opcode_mode32(0xea, &jmpf_ptr16_32, CHK_PTR16 | CHK_IMM32);
    // 0xeb : jmp
    // 0xec : in_al_dx
    setup_opcode_mode32(0xed, &in_eax_dx, 0);
    // 0xee : out_dx_al
    setup_opcode_mode32(0xef, &out_dx_eax, 0);

    setup_opcode_mode32(0x0f80, &jo_rel32, CHK_IMM32);
    setup_opcode_mode32(0x0f81, &jno_rel32, CHK_IMM32);
    setup_opcode_mode32(0x0f82, &jb_rel32, CHK_IMM32);
    setup_opcode_mode32(0x0f83, &jnb_rel32, CHK_IMM32);
    setup_opcode_mode32(0x0f84, &jz_rel32, CHK_IMM32);
    setup_opcode_mode32(0x0f85, &jnz_rel32, CHK_IMM32);
    setup_opcode_mode32(0x0f86, &jbe_rel32, CHK_IMM32);
    setup_opcode_mode32(0x0f87, &ja_rel32, CHK_IMM32);
    setup_opcode_mode32(0x0f88, &js_rel32, CHK_IMM32);
    setup_opcode_mode32(0x0f89, &jns_rel32, CHK_IMM32);
    setup_opcode_mode32(0x0f8a, &jp_rel32, CHK_IMM32);
    setup_opcode_mode32(0x0f8b, &jnp_rel32, CHK_IMM32);
    setup_opcode_mode32(0x0f8c, &jl_rel32, CHK_IMM32);
    setup_opcode_mode32(0x0f8d, &jnl_rel32, CHK_IMM32);
    setup_opcode_mode32(0x0f8e, &jle_rel32, CHK_IMM32);
    setup_opcode_mode32(0x0f8f, &jnle_rel32, CHK_IMM32);

    setup_opcode_mode32(0x0faf, &imul_r32_rm32, CHK_MODRM);

    setup_opcode_mode32(0x0fb6, &movzx_r32_rm8, CHK_MODRM);
    setup_opcode_mode32(0x0fb7, &movzx_r32_rm16, CHK_MODRM);

    setup_opcode_mode32(0x0fbe, &movsx_r32_rm8, CHK_MODRM);
    setup_opcode_mode32(0x0fbf, &movsx_r32_rm16, CHK_MODRM);

    // 0x80 : code_80
    setup_opcode_mode32(0x81, &code_81_32, CHK_MODRM | CHK_IMM32);
    // 0x82 : code_82
    setup_opcode_mode32(0x83, &code_83_32, CHK_MODRM | CHK_IMM8);
    // 0xc0 : code_c0
    setup_opcode_mode32(0xc1, &code_c1_32, CHK_MODRM | CHK_IMM8);
    setup_opcode_mode32(0xd3, &code_d3_32, CHK_MODRM);
    setup_opcode_mode32(0xf7, &code_f7_32, CHK_MODRM);
    setup_opcode_mode32(0xff, &code_ff_32, CHK_MODRM);
    setup_opcode_mode32(0x0f00, &code_0f00_32, CHK_MODRM);
    setup_opcode_mode32(0x0f01, &code_0f01_32, CHK_MODRM);
}
