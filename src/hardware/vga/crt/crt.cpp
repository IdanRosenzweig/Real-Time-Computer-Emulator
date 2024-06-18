#include "crt.h"

#include "crt_pmio_codes.h"

#define chk_regidx(n)    do{ if(n>sizeof(regs))DEBUG_ERROR("register index out of bound", n); if(!regs[n])DEBUG_ERROR("not implemented"); }while(0);

crt::crt() {
    for (uint8_t i = 0; i < sizeof(regs) / sizeof(uint8_t * ); i++)
        if (regs[i] != nullptr)
            *regs[i] = 0;
}

void crt::get_windowsize(uint16_t *x, uint16_t *y) {
//    *x = hdeer.HDEE * 8;
//    *y = vdeer.VDEE * 8;
    *x = PIXELS_PER_LINE();
    *y = NO_SCANLINES();
}


uint8_t crt::read_byte_from_port(uint16_t addr) {
    switch (addr) {
        // get the index of the currently selected register
        case crt_pmio_input_codes::GET_SELECTED_REG1:
        case crt_pmio_input_codes::GET_SELECTED_REG2:
            return crtcar.raw;

            // read the register that is currently selected
        case crt_pmio_input_codes::ACCESS_REG1:
        case crt_pmio_input_codes::ACCESS_REG2:
            return *regs[crtcar.INDX];

        default:
            return -1;
    }
}

void crt::write_byte_to_port(uint16_t addr, uint8_t v) {
    switch (addr) {
        // set the index of the currently selected register (for future access)
        case crt_pmio_output_codes::SELECT_REG1:
        case crt_pmio_output_codes::SELECT_REG2:
            chk_regidx(v);
            crtcar.raw = v;
            break;

            // write to the register that is currently selected
        case crt_pmio_output_codes::ACCESS_REG1:
        case crt_pmio_output_codes::ACCESS_REG2:
            *regs[crtcar.INDX] = v;
            break;
    }
}