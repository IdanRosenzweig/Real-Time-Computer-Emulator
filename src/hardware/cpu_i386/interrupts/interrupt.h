#ifndef EMULATOR_INTERRUPT_H
#define EMULATOR_INTERRUPT_H

#include <stdint.h>

struct interrupt {
    uint8_t id; // interrupt number
    bool hard; // hardware of software interrupt
};

#endif
