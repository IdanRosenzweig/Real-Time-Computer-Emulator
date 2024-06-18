#ifndef EMULATOR_FLOPPY_INPUT_CODES_H
#define EMULATOR_FLOPPY_INPUT_CODES_H

namespace floppy_input_codes {
    constexpr uint16_t
    STATUS_REGISTER_A = 0x3F0;
    constexpr uint16_t
    STATUS_REGISTER_B = 0x3F1;
    constexpr uint16_t
    TAPE_DRIVE_REGISTER = 0x3F3;
    constexpr uint16_t
    MAIN_STATUS_REGISTER = 0x3F4;
    constexpr uint16_t
    DATA_FIFO = 0x3F5;
    constexpr uint16_t
    DIGITAL_INPUT_REGISTER = 0x3F7;
}

#endif
