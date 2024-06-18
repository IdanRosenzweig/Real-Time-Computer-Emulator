#ifndef EMULATOR_INTERRUPTS_QUEUE_H
#define EMULATOR_INTERRUPTS_QUEUE_H

#include "interrupt.h"

#include "../registers/registers.hpp"
#include "../pic/pic.hpp"

#include <queue>

struct interrupts_system {
private:
    registers *regs;

public:
    // programmable interrupt controllers
    pic pic_m; // master
    pic pic_s; // slave

    std::queue <interrupt> que;

    interrupts_system(registers *regs) : regs(regs), pic_m(), pic_s(&pic_m) {}

    // queue new interrupt
    void queue_interrupt(struct interrupt intr);

    // check if there is request for new interrupt in the pic
    // if there is not, return false
    // if there is, queue the requested interrupt to the queue and return true
    bool chk_irq(void);
};


#endif
