#ifndef EMULATOR_OS_TERMINAL_IO_H
#define EMULATOR_OS_TERMINAL_IO_H

#include "../common.hpp"
#include "../hardware/cpu_i386/io/port_mapped_io.h"

// operating system terminal io
class os_terminal_io : public pmio {
public:
    uint8_t read_byte_from_port(uint16_t addr) override;

    void write_byte_to_port(uint16_t addr, uint8_t v) override;
};

#endif
