#ifndef EMULATOR_GENERAL_REGISTER_H
#define EMULATOR_GENERAL_REGISTER_H

// general purpose register
struct general_reg {
    union {
        struct {
            uint8_t reg8_l;
            uint8_t reg8_h;
        };
        uint16_t reg16;
        uint32_t reg32;
    };
};

enum general_regs_t {
//    Eax, Ecx, Edx, Ebx, Esp, Ebp, Esi, Edi,
//    Ax, Cx, Dx, Bx, Sp, Bp, Si, Di,
//    Al, Cl, Dl, Bl,
//    Ah, Ch, Dh, Bh
    EAX, ECX, EDX, EBX, ESP, EBP, ESI, EDI,
    AX, CX, DX, BX, SP, BP, SI, DI,
    AL, CL, DL, BL, AH, CH, DH, BH
};

#define GPREGS_COUNT 8
//enum reg32_t {
//    EAX, ECX, EDX, EBX, ESP, EBP, ESI, EDI, GPREGS_COUNT
//};
//
//enum reg16_t {
//    AX, CX, DX, BX, SP, BP, SI, DI
//};
//enum reg8_t {
//    AL, CL, DL, BL, AH, CH, DH, BH
//};

//enum reg8l_t { AL, CL, DL, BL, SPL, BPL, SIL, DIL };

#endif
