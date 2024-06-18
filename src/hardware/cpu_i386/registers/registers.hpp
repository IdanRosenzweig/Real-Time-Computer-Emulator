#ifndef EMULATOR_REGS_H
#define EMULATOR_REGS_H

#include "../../../common.hpp"

#include "flags_reg.hpp"
#include "control_reg.hpp"
#include "ip_register.h"

#include "general_reg.h"
#include "segment_reg.h"
#include "dt_reg.h"

struct registers {
public:
    struct ip_reg ip_reg;

    struct general_reg general_regs[GPREGS_COUNT];
    struct segment_reg segment_regs[SGREGS_COUNT];
    struct dt_reg dt_regs[DTREGS_COUNT];

    struct flags_reg flags_reg;

    struct control_reg control_reg;

    registers(uint32_t init_ip_reg, uint16_t init_cs_reg);

    // general registers
    uint32_t get_gpreg(enum general_regs_t n); // get value of general register

    void set_gpreg(enum general_regs_t n, uint32_t v);

    void update_gpreg(enum general_regs_t n, uint32_t v);

    // segment registers
    void get_sgreg(enum sgreg_t n, segment_reg *reg); // get value of segment register into reg

    uint16_t get_sgreg(enum sgreg_t reg); // get the value of segment register

    void set_sgreg(enum sgreg_t n, segment_reg *reg); // set the value of segment register into reg


    inline uint16_t get_cpl() {return get_sgreg(CS) & 3;}


    // descriptor table registers
    uint32_t get_dtreg_selector(enum dt_reg_t n);

    uint32_t get_dtreg_base(enum dt_reg_t n);

    uint16_t get_dtreg_limit(enum dt_reg_t n);

    void set_dtreg(enum dt_reg_t n, uint16_t sel, uint32_t base, uint16_t limit);


    inline bool is_protected(void) { return control_reg.is_protected(); }

    inline bool is_mode32(void) { return segment_regs[CS].cache.flags.DB; }

};


#endif
