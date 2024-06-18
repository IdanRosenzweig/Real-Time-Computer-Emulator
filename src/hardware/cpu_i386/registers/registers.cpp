#include <string.h>
#include "registers.hpp"

registers::registers(uint32_t init_ip_reg, uint16_t init_cs_reg) {
    memset(general_regs, 0, sizeof(general_regs));
    memset(segment_regs, 0, sizeof(segment_regs));
    memset(dt_regs, 0, sizeof(dt_regs));

//    ip_reg.eip = 0x0000fff0;
    ip_reg.eip = init_ip_reg;

//    control_reg.set_crn(0, 0x60000010);
    control_reg.cr0.raw = 0x60000010;

//    flags_reg.set_eflags(0x00000002);
    flags_reg.eflags = 0x00000002;

//    segment_regs[CS].raw = 0xf000;
    segment_regs[CS].raw = init_cs_reg;

    segment_regs[CS].cache.base = 0xffff0000;
//    segment_regs[CS].cache.base = init_cs_reg;
    segment_regs[CS].cache.flags.type.segc = 1;
    for (int i = 0; i < SGREGS_COUNT; i++) {
        segment_regs[i].cache.limit = 0xffff;
        segment_regs[i].cache.flags.P = 1;
        segment_regs[i].cache.flags.type.A = 1;
        segment_regs[i].cache.flags.type.data.w = 1;
    }

    dt_regs[IDTR].base = 0x0000;
    dt_regs[IDTR].limit = 0xffff;
    dt_regs[GDTR].base = 0x0000;
    dt_regs[GDTR].limit = 0xffff;
    dt_regs[LDTR].base = 0x0000;
    dt_regs[LDTR].limit = 0xffff;
}

// general registers
uint32_t registers::get_gpreg(enum general_regs_t n) {
    if (n < AX)
        return general_regs[n].reg32;
    else if (n < AL)
        return general_regs[n - 8].reg16;
    else if (n < AH)
        return general_regs[n - 16].reg8_l;
    else
        return general_regs[n - 20].reg8_h;
}

void registers::set_gpreg(enum general_regs_t n, uint32_t v) {
    if (n < AX)
        general_regs[n].reg32 = v;
    else if (n < AL)
        general_regs[n - 8].reg16 = v;
    else if (n < AH)
        general_regs[n - 16].reg8_l = v;
    else
        general_regs[n - 20].reg8_h = v;
}

void registers::update_gpreg(enum general_regs_t n, uint32_t v) {
    if (n < AX)
        general_regs[n].reg32 += v;
    else if (n < AL)
        general_regs[n - 8].reg16 += v;
    else if (n < AH)
        general_regs[n - 16].reg8_l += v;
    else
        general_regs[n - 20].reg8_h += v;
}

// segment registers
void registers::get_sgreg(enum sgreg_t n, segment_reg *reg) {
    DEBUG_ASSERT(n < SGREGS_COUNT && reg);
    *reg = segment_regs[n];
}
//uint16_t get_sgreg(enum sgreg_t n){ return segment_regs[n].raw; };

uint16_t registers::get_sgreg(enum sgreg_t reg) {
    segment_reg sg;
    get_sgreg(reg, &sg);
    return sg.raw;
}

void registers::set_sgreg(enum sgreg_t n, segment_reg *reg) {
    DEBUG_ASSERT(n < SGREGS_COUNT && reg);
    segment_regs[n] = *reg;
}

// descriptor table registers
uint32_t registers::get_dtreg_selector(enum dt_reg_t n) {
    DEBUG_ASSERT(n < DTREGS_COUNT);
    return dt_regs[n].selector;
}

uint32_t registers::get_dtreg_base(enum dt_reg_t n) {
    DEBUG_ASSERT(n < DTREGS_COUNT);
    return dt_regs[n].base;
}

uint16_t registers::get_dtreg_limit(enum dt_reg_t n) {
    DEBUG_ASSERT(n < DTREGS_COUNT);
    return dt_regs[n].limit;
}

void registers::set_dtreg(enum dt_reg_t n, uint16_t sel, uint32_t base, uint16_t limit) {
    DEBUG_ASSERT(n < DTREGS_COUNT);
    dt_regs[n].selector = sel;
    dt_regs[n].base = base;
    dt_regs[n].limit = limit;
}
