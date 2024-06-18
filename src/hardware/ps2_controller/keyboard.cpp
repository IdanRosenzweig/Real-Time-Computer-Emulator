#include "keyboard.hpp"
#include "ps2_controller.h"

#include <iostream>

using namespace std;;

void keyboard::send_scancode(uint8_t scancode) {
    if (controller->ccb->KE) // disabled
        return;

    controller->write_out_buff(scancode);

    if (controller->ccb->KIE) {
        intr_req = true;
    }
}
