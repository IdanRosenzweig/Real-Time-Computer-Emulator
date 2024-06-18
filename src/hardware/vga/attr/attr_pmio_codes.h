#ifndef EMULATOR_ATTR_PMIO_CODES_H
#define EMULATOR_ATTR_PMIO_CODES_H

namespace attr_pmio_input_codes {
    constexpr uint16_t GET_SELECTED_REG = 0x3c0;
    constexpr uint16_t ACCESS_REG = 0x3c1;
}

namespace attr_pmio_output_codes {
    constexpr uint16_t SELECT_REG = 0x3c0;
    constexpr uint16_t ACCESS_REG = 0x3c1;
}

#endif
