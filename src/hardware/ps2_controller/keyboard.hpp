#ifndef EMULATOR_KEYBOARD_H
#define EMULATOR_KEYBOARD_H

#include "../../common.hpp"
#include "../cpu_i386/pic/interrupt_caller.hpp"

class ps2_controller;

class keyboard : public device_interrupt_caller {
private:
    ps2_controller *controller;

public:
    keyboard(ps2_controller *controller) : controller(controller) {
    };

    // the keyboard sends a scancode
    void send_scancode(uint8_t scancode);
};

#endif
