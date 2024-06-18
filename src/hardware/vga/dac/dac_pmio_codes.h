#ifndef EMULATOR_DAC_PMIO_CODES_H
#define EMULATOR_DAC_PMIO_CODES_H

namespace dac_pmio_input_codes {
    constexpr uint16_t READ_COLOR = 0x3c9;
}

namespace dac_pmio_output_codes {
    constexpr uint16_t SET_READ_INDX = 0x3c7;
    constexpr uint16_t SET_WRITE_INDX = 0x3c8;

    constexpr uint16_t WRITE_COLOR = 0x3c9;
}

#endif
