#ifndef EMULATOR_GC_PMIO_CODES_H
#define EMULATOR_GC_PMIO_CODES_H

namespace gc_pmio_input_codes {
    constexpr uint16_t
            GET_SELECTED_REG = 0x3ce;
    constexpr uint16_t
            ACCESS_REG = 0x3cf;
}

namespace gc_pmio_output_codes {
    constexpr uint16_t
            SELECT_REG = 0x3ce;
    constexpr uint16_t
            ACCESS_REG = 0x3cf;
}

#endif
