#include "attr.h"
#include "../vga.hpp"

#include "attr_pmio_codes.h"


attribute::attribute(class vga *v) {
    vga = v;
    for (uint8_t i = 0; i < sizeof(regs) / sizeof(uint8_t * ); i++)
        if (regs[i] != nullptr) *regs[i] = 0;
}


uint8_t attribute::dac_index(uint8_t index) {

    union {
        uint8_t raw;
        struct {
            uint8_t low: 4; // P0 - P3
            uint8_t high: 2; // P4 - P5
        };
    } palette_reg;
    palette_reg.raw = ipr[index & 0xf].raw;

    uint8_t dac_idx = palette_reg.low;

    if (amcr.GAM) { // graphics mode of operation
        if (amcr.P54S) // use the csr for bits 4 and 5
            dac_idx += csr.SC45 << 4;
        else // use the original palette_reg value for bits 4 and 5
            dac_idx += palette_reg.high << 4;

        dac_idx += csr.SC67 << 6; // add the 2 most significant bits from the csr
    }

    return dac_idx;
}


uint8_t attribute::read_byte_from_port(uint16_t addr) {
    switch (addr) {
        // get the index of the currently selected register
        case attr_pmio_input_codes::GET_SELECTED_REG:
            return acar.raw;

            // read the register that is currently selected
        case attr_pmio_input_codes::ACCESS_REG:
            return *regs[acar.INDX];
    }
    return -1;
}

void attribute::write_byte_to_port(uint16_t addr, uint8_t v) {
    switch (addr) {

        // set the index of the currently selected register (for future access)
        case attr_pmio_output_codes::SELECT_REG:
            chk_regidx(v);
            acar.raw = v;
            break;

            // write to the register that is currently selected
        case attr_pmio_output_codes::ACCESS_REG:
            *regs[acar.INDX] = v;
            break;
    }
}