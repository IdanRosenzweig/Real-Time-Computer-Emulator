#ifndef EMULATOR_PIC_OUTPUT_CODES_H
#define EMULATOR_PIC_OUTPUT_CODES_H

#include <stdint.h>

namespace pic_output_codes {
    constexpr uint16_t
    MASTER_COMMAND = 0x20;
    constexpr uint16_t
    MASTER_DATA_PORT = 0x21;

    constexpr uint16_t
    SLAVE_COMMAND = 0xa0;
    constexpr uint16_t
    SLAVE_DATA_PORT = 0xa1;
};

#endif
