#ifndef EMULATOR_FLOPPY_OUTPUT_CODES_H
#define EMULATOR_FLOPPY_OUTPUT_CODES_H

namespace floppy_output_codes {
    constexpr uint16_t
    DIGITAL_OUTPUT_REGISTER = 0x3F2;
    constexpr uint16_t
    TAPE_DRIVE_REGISTER = 0x3F3;
    constexpr uint16_t
    DATARATE_SELECT_REGISTER = 0x3F4;
    constexpr uint16_t
    DATA_FIFO = 0x3F5;
    constexpr uint16_t
    CONFIGURATION_CONTROL_REGISTER = 0x3F7;
}

#endif
