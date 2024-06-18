#include "mouse.hpp"
#include "ps2_controller.h"

#include <thread>

#include <iostream>
using namespace std;

void mouse::exec_command(uint8_t v) {
    switch (v) {
        case 0xf4:
//            // wait until the output buffer is empty
//			while(controller->sr.OBF);
//
//            // mark it as full
//			controller->sr.OBF = 1;
//            // and write to the output buffer
//			controller->output_buff = 0xfa;
            controller->write_out_buff(0xfa);

            if (controller->ccb->MIE) {
                intr_req = true;
            }

            break;
    }
}

void mouse::send_scancode(uint8_t code) {
    if (controller->ccb->ME)    // disabled
        return;

//    // wait until the output buffer is empty
//	while(controller->sr.OBF)
//		std::this_thread::sleep_for(std::chrono::microseconds(10));
//
//    // mark it as full
//	controller->sr.OBF = 1;
//    // write to the output buffer
//	controller->output_buff = code;
    controller->write_out_buff(code);

    if (controller->ccb->MIE) {
        intr_req = true;
    }
}

void mouse::send_packet_scancode(scancode_packet pckt) {
    uint8_t code = (pckt.ys << 5) + (pckt.xs << 4) + (1 << 3) + (pckt.br << 1) + pckt.bl;
    send_scancode(code);

    std::this_thread::sleep_for(std::chrono::microseconds(100));

    send_scancode(pckt.x_axis_val);

    std::this_thread::sleep_for(std::chrono::microseconds(100));

    send_scancode(pckt.y_axis_val);
}
