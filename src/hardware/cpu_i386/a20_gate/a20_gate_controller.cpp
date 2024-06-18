#include "a20_gate_controller.hpp"
#include "../ram/ram.hpp"

uint8_t a20_gate_controller::read_byte_from_port(uint16_t addr) {
    return mem->a20_gate.is_enabled << 1;
}

void a20_gate_controller::write_byte_to_port(uint16_t addr, uint8_t v) {
    bool a20_ena = (v >> 1) & 0b1;
    mem->a20_gate.is_enabled = a20_ena;


}
