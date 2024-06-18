#ifndef EMULATOR_X86_INSTRUCTION_H
#define EMULATOR_X86_INSTRUCTION_H

#include "../registers/segment_reg.h"

enum rep_pre {
    NONE, REPZ, REPNZ
};

struct segment_pre {
    bool used;
    enum sgreg_t reg;
};

typedef uint16_t opcode_t;
struct x86_instr_t {

    // prefixes (optional)
    bool mutual_exc_pre; // mutual exclusion
    rep_pre repetition_pre; // repetition
    segment_pre segment_override; // segment override
    bool operand_override; // operand override
    bool addr_override; // addr override

    // opcode (mandatory)
    opcode_t opcode;

    // Mod R/M (optional)
    union modrm_t {
        uint8_t raw;
        struct {
            uint8_t rm: 3;
            uint8_t reg: 3;
            uint8_t mod: 2;
        };
    } modrm;

    // SIB (optional)
    union {
        uint8_t raw;
        struct {
            uint8_t base: 3;
            uint8_t index: 3;
            uint8_t scale: 2;
        };
    } sib;

    // displacement (optional)
    union {
        int8_t disp8;
        int16_t disp16;
        int32_t disp32;
    } disp;

    // immediate (optional)
    union {
        int8_t imm8;
        int16_t imm16;
        int32_t imm32;
    } imm;

    int16_t ptr16;
    uint32_t moffs;

    // if needed, curr_seg refers to the default segment register
    // that should be used for this instruction.
    // the default segment register differs from one instruction to another
    sgreg_t curr_seg;

};

union x86_instr_flags_t {
    uint8_t raw;
    struct {
        uint8_t has_modrm: 1;
        uint8_t has_imm32: 1;
        uint8_t has_imm16: 1;
        uint8_t has_imm8: 1;
        uint8_t has_ptr16: 1;
        uint8_t has_moffs: 1;
        uint8_t has_moffs8: 1;
    } flags;
};

#define CHK_MODRM   (1 << 0)
#define CHK_IMM32   (1 << 1)
#define CHK_IMM16   (1 << 2)
#define CHK_IMM8    (1 << 3)
#define CHK_PTR16   (1 << 4)
#define CHK_MOFFS   (1 << 5)


#endif
