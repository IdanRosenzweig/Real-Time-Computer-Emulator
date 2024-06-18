#ifndef EMULATOR_PIT_INTERNAL_TIMER_H
#define EMULATOR_PIT_INTERNAL_TIMER_H

#include <thread>

// timer runs at frequency of 1.193182 MHz
#define FREQUENCY 1193182

struct internal_timer_t {
    uint8_t mode; // mode of this timer

    uint16_t count;
    uint16_t def;

    bool running; // is this timer running
    std::thread th; // (the thead that executes the timer when it runs)
};

#endif
