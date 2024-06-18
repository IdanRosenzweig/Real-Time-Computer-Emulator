#ifndef EMULATOR_DT_REGISTER_H
#define EMULATOR_DT_REGISTER_H

struct dt_reg {
    uint16_t selector;
    uint32_t base;
    uint16_t limit;
};

enum dt_reg_t {
    GDTR, // points to the global descriptor table
    IDTR, // points to the interrupt descriptor table
    LDTR, // points to the local descriptor table
    TR,
    DTREGS_COUNT
};


#endif
