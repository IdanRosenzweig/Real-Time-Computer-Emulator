#ifndef EMULATOR_SEQ_PMIO_CODES_H
#define EMULATOR_SEQ_PMIO_CODES_H

namespace seq_pmio_input_codes {
    constexpr uint16_t
            GET_SELECTED_REG = 0x3c4;
    constexpr uint16_t
            ACCESS_REG = 0x3c5;
}

namespace seq_pmio_output_codes {
    constexpr uint16_t
            SELECT_REG = 0x3c4;
    constexpr uint16_t
            ACCESS_REG = 0x3c5;
}

#endif
