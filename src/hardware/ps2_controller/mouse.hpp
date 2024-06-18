#ifndef EMULATOR_MOUSE_H
#define EMULATOR_MOUSE_H

#include <stdint.h>
#include "../../common.hpp"
#include "../cpu_i386/pic/interrupt_caller.hpp"

class ps2_controller;

class mouse : public device_interrupt_caller {
    ps2_controller *controller;

public:
    mouse(ps2_controller *controller) : controller(controller) {
    };

    // send a command for the mouse to execute
    void exec_command(uint8_t code);

    // the mouse sends a scancode
    void send_scancode(uint8_t code);

    struct scancode_packet {
        // left button
        uint8_t bl: 1;
        // right button
        uint8_t br: 1;
        // middle button
        uint8_t bm: 1;

        uint8_t : 1;

        // x axis sign (plus/minus)
        uint8_t xs: 1;
        // y axis sign (plus/minus)
        uint8_t ys: 1;

        uint8_t xo: 1;
        uint8_t yo: 1;

        // value of movement in x axis
        uint8_t x_axis_val;
        // value of movement in y axis
        uint8_t y_axis_val;

    };

    void send_packet_scancode(scancode_packet pckt);
};

#endif
