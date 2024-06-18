#ifndef EMULATOR_PIT_INPUT_CODES_H
#define EMULATOR_PIT_INPUT_CODES_H

#include <stdint.h>

namespace pit_input_codes {
    constexpr uint16_t
    CHANNEL_0_DATA_PORT = 0x40;
    constexpr uint16_t
    CHANNEL_1_DATA_PORT = 0x41;
    constexpr uint16_t
    CHANNEL_2_DATA_PORT = 0x42;
}

#endif
