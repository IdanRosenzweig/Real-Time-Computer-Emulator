#include <stdio.h>
#include "os_terminal_io.hpp"

uint8_t os_terminal_io::read_byte_from_port(uint16_t addr) {
    return getchar();
}

void os_terminal_io::write_byte_to_port(uint16_t addr, uint8_t v) {
    putchar(v);
}
