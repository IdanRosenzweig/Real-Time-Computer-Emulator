#ifndef EMULATOR_PIT_H
#define EMULATOR_PIT_H

#include <stdint.h>

#include "../pic/interrupt_caller.hpp"
#include "../io/port_mapped_io.h"

#include "pit_internal_timer.h"

#define NO_INTERNAL_TIMERS 3

// programmable interval timer
class pit : public device_interrupt_caller, public pmio {
private:

    // mode/command register
    // controls various properties of the pit and its timers
    union {
        uint8_t raw;
        struct {
            uint8_t BCD: 1;

            // operation mode
            // 0 0 0 (mode 0) = interrupt on terminal count
            // 0 0 1 (mode 1) = hardware re-triggerable one-shot
            // 0 1 0 (mode 2) = rate generator
            // 0 1 1 (mode 3) = square wave generator
            // 1 0 0 (mode 4) = software triggered strobe
            // 1 0 1 (mode 5) = hardware triggered strobe
            // 1 1 0 (mode 2) = rate generator, same as 010b
            // 1 1 1 (mode 3) = square wave generator, same as 011b
            uint8_t MODE: 3;

            // access mode
            // 0 0 = used for latch count value command
            // 0 1 = lobyte only
            // 1 0 = hibyte only
            // 1 1 = lobyte or hibyte
            uint8_t RL: 2;

            // selected channel
            uint8_t SC: 2;
        };
    } cwr;

    // switch for accessing lobyte or hibyte
    bool r_lobyte;

    // internal timers
    struct internal_timer_t timers[NO_INTERNAL_TIMERS];

public:
    pit();

    ~pit();


    uint8_t read_byte_from_port(uint16_t addr) override;

    void write_byte_to_port(uint16_t addr, uint8_t v) override;


    // active function for each timer's thread
    void worker(internal_timer_t *t);

};

#endif
