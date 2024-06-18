#ifndef EMULATOR_DAC_H
#define EMULATOR_DAC_H

#include <stdint.h>
#include "../../cpu_i386/io/port_mapped_io.h"

class dac : public pmio {
private:
    uint8_t progress;

    union {
        uint8_t raw[3];
        struct {
            uint8_t R: 6;
            uint8_t G: 6;
            uint8_t B: 6;
        };
    } clut[0x100];

    // register that holds the index for next writes to the dac
    union {
        uint8_t raw;
        uint8_t index;
    } w_par;

    // register that holds the index for next reads from the dac
    union {
        uint8_t raw;
        uint8_t index;
    } r_par;

    union {
        uint8_t raw;
        uint8_t color;
    } pdr;


    union {
        uint8_t raw;
        struct {
            uint8_t DACstate: 2;
        };
    } dacsr;

    // dac mask register
    union {
        uint8_t raw;
        uint8_t mask;
    } pelmr;

public:

    // get the rgb value at index
    uint32_t get_rgb(uint8_t index);

    // pmio
    uint8_t read_byte_from_port(uint16_t addr) override;

    void write_byte_to_port(uint16_t addr, uint8_t v) override;

};

#endif
