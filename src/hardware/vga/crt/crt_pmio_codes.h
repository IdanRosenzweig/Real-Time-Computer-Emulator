#ifndef EMULATOR_CRT_PMIO_CODES_H
#define EMULATOR_CRT_PMIO_CODES_H

namespace crt_pmio_input_codes {
    constexpr uint16_t
            GET_SELECTED_REG1 = 0x3b4;
    constexpr uint16_t
            GET_SELECTED_REG2 = 0x3d4;

    constexpr uint16_t
            ACCESS_REG1 = 0x3b5;
    constexpr uint16_t
            ACCESS_REG2 = 0x3d5;
}

namespace crt_pmio_output_codes {
    constexpr uint16_t
            SELECT_REG1 = 0x3b4;
    constexpr uint16_t
            SELECT_REG2 = 0x3d4;

    constexpr uint16_t
            ACCESS_REG1 = 0x3b5;
    constexpr uint16_t
            ACCESS_REG2 = 0x3d5;
}

#endif
