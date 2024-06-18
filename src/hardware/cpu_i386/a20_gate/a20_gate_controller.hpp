#ifndef EMULATOR_A20_GATE_CONTROLLER_H
#define EMULATOR_A20_GATE_CONTROLLER_H

#include "../../../common.hpp"
#include "../io/port_mapped_io.h"

class ram;
class a20_gate_controller : public pmio {
private:
    ram *mem;
public:
    a20_gate_controller(ram *m) { mem = m; };

    uint8_t read_byte_from_port(uint16_t addr) override;

    void write_byte_to_port(uint16_t addr, uint8_t v) override;
};

#endif
