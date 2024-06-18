#include "dac.h"

#include "dac_pmio_codes.h"

uint32_t dac::get_rgb(uint8_t index) {
    // the r g b  values are stored in unsigned 6 bit.
    // convert them to unsigned 8 bit
    uint8_t r = clut[index].R << 2;
    uint8_t g = clut[index].G << 2;
    uint8_t b = clut[index].B << 2;

    // sum the r g b values and put them in correct order
    uint32_t rgb = r;
    rgb += g << 8;
    rgb += b << 16;

    return rgb;
}


uint8_t dac::read_byte_from_port(uint16_t addr) {
    switch (addr) {
        case 0x3c6:
            return pelmr.raw;

        case 0x3c7:
            return dacsr.raw;

        case dac_pmio_input_codes::READ_COLOR:
            uint8_t v = clut[r_par.index].raw[progress];
            if (++progress == 3) {
                progress = 0;
                r_par.index++;
            }
            return v;
    }
    return -1;
}

void dac::write_byte_to_port(uint16_t addr, uint8_t v) {
    switch (addr) {

        case dac_pmio_output_codes::SET_READ_INDX:
            if (v > 0xff) DEBUG_ERROR("");
            r_par.raw = v;
            progress = 0;
            break;

        case dac_pmio_output_codes::SET_WRITE_INDX:
            if (v > 0xff) DEBUG_ERROR("");
            w_par.raw = v;
            progress = 0;
            break;

            // write
        case dac_pmio_output_codes::WRITE_COLOR:
            clut[w_par.index].raw[progress] = v;
            if (++progress == 3) {
                progress = 0;
                w_par.index++;
            }
            break;
    }
}