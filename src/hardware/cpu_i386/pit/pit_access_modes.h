#ifndef EMULATOR_PIT_ACCESS_MODES_H
#define EMULATOR_PIT_ACCESS_MODES_H

namespace pit_access_mode {
    constexpr uint8_t
    LATCH_COUNT_COMMAND = 0b00;

    constexpr uint8_t
    LOBYTE = 0b01;
    constexpr uint8_t
    HIBYTE = 0b10;
    constexpr uint8_t
    LOBYTE_HIBYTE = 0b11;
};

#endif
