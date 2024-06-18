#ifndef EMULATOR_EXCEPTIONS_H
#define EMULATOR_EXCEPTIONS_H

// i386 exceptions

enum exception_t {
    EXP_DE = 0, // division err
    EXP_DB = 1, // debug
    EXP_BP = 3, // breakpoint
    EXP_OF = 4, // overflow
    EXP_BR = 5, // bound range exceed
    EXP_UD = 6, // invalid opcode
    EXP_NM = 7, // device not available
    EXP_DF = 8, // double fault
    EXP_TS = 10, // invalid tss
    EXP_NP = 11, // segment not present
    EXP_SS = 12, // stack segment fault
    EXP_GP = 13, // general protection fault
    EXP_PF = 14, // page fault
    EXP_MF = 16, // floating point exception
    EXP_AC = 17, // alignment check
    EXP_MC = 18, // machine check
    EXP_XF = 19, // simd floating point exception
    EXP_VE = 20, // virtualization exception
    EXP_CP = 21, // control protection exception
    EXP_SX = 30 // security exception
};

#define CPU_EXCEPTION(exp, condition) if (condition) throw exp;
#define CPU_EXCEPTION_DO(exp, condition, exec) if (condition) {exec; throw exp;}

#endif
