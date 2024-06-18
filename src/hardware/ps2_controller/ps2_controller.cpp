#include "ps2_controller.h"
#include <thread>
#include <iostream>
using namespace std;

uint8_t ps2_controller::read_byte_from_port(uint16_t addr) {
    switch (addr) {
        case ps2_controller_input_codes::DATA_PORT:
            return read_out_buff();
        case ps2_controller_input_codes::STATUS_REGISTER:
            return sr.raw;
        default:
            DEBUG_ERROR("");
            return -1;
    }
}

void ps2_controller::write_byte_to_port(uint16_t addr, uint8_t v) {
    switch (addr) {
        case ps2_controller_output_codes::DATA_PORT:
            // marking the last port that received data
            sr.F1 = 0;
            break;

        case ps2_controller_output_codes::COMMAND:
            // marking the last port that received data
            sr.F1 = 1;
            break;
    }
    exec_command(v);
}

void ps2_controller::exec_command(uint8_t v) {
    if (!sr.ST6) { // currently a 1 byte command

        if (sr.F1 == 1) { // only if the data was received from COMMAND port
            switch (v) {

                // enable the second controller device port
                case ps2_controller_commands::ENABLE_SECOND_DEVICE_PORT:
                    ccb->ME = 0; // enable
                    return;
                    // disable the second controller device port
                case ps2_controller_commands::DISABLE_SECOND_DEVICE_PORT:
                    ccb->ME = 1; // disable
                    return;

                    // enable the first controller device port
                case ps2_controller_commands::ENABLE_FIRST_DEVICE_PORT:
                    ccb->KE = 0; // enable
                    return;
                    // disable the first controller device port
                case ps2_controller_commands::DISABLE_FIRST_DEVICE_PORT:
                    ccb->KE = 1; // disable
                    return;

                default:
                    if (ps2_controller_commands::READ_RAM_START <= v
                        && v < ps2_controller_commands::READ_RAM_END) { // access internal ram
                        write_out_buff(controller_ram[v - ps2_controller_commands::READ_RAM_START]);
                        return;
                    }

            }

        }

        // todo move this block inside the if (sr.F1 == 1) ?
        // going towards a 2 byte command
        curr_command = v;
        sr.ST6 = 1;

    } else { // this is a 2 byte command

        if (sr.F1 == 0) { // only if the second byte was received from DATA_PORT
            switch (curr_command) {

                case 0xd1:
                    write_to_controller_output_port(v);
                    break;

                    // write a byte to the output buffer of the first device
                    // (looks like the device sent it)
                case ps2_controller_commands::WRITE_OUTPUT_OF_FIRST_DEVICE:
                    keyboard.send_scancode(v);
                    break;
                    // write a byte to the output buffer of the second device
                    // (looks like the device sent it)
                case ps2_controller_commands::WRITE_OUTPUT_OF_SECOND_DEVICE:
                    mouse.send_scancode(v);
                    break;

                    // put byte at the second device's input buffer
                case ps2_controller_commands::SEND_INPUT_TO_SECOND_DEVICE:
                    mouse.exec_command(v);
                    break;

                default:
//                    if (curr_command >= 0x40 && curr_command < 0x80) {
//                        controller_ram[(curr_command - 0x40) % 0x20] = v;
//                    }
                    if (ps2_controller_commands::WRITE_TO_RAM_START <= curr_command
                        && curr_command < ps2_controller_commands::WRITE_TO_RAM_END) { // write to internal ram
                        controller_ram[curr_command - ps2_controller_commands::WRITE_TO_RAM_START] = v;
                    }
            }
        }

        sr.ST6 = 0;
    }

}

void ps2_controller::write_out_buff(uint8_t v) {
    // wait until the output buffer is empty
    while (sr.OBF)
        std::this_thread::sleep_for(std::chrono::microseconds(10));


    // immediately mark it as full
    sr.OBF = 1;
    // and write the data to the buffer
    output_buff = v;
}

uint8_t ps2_controller::read_out_buff(void) {
    // mark the output buffer as empty
    sr.OBF = 0;
    // the return its value
    return output_buff;
}

void ps2_controller::write_to_controller_output_port(uint8_t v) {
    output_port.raw = v;

    if (output_port.raw & (1 << 1)) {
        mem->a20_gate.is_enabled = true;
    } else {
        mem->a20_gate.is_enabled = false;
    }
//    switch (v) {
//        case 0b11011101:
//            mem->a20_gate.is_enabled = false;
//            break;
//        case 0b11011111:
//            mem->a20_gate.is_enabled = true;
//            break;
//    }
}

uint8_t ps2_controller::read_controller_output_port(void) {
    return output_port.raw;
}
