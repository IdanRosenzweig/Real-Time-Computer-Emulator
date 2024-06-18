#ifndef EMULATOR_PS2_CONTROLLER_H
#define EMULATOR_PS2_CONTROLLER_H

#include "../../common.hpp"
#include "../cpu_i386/ram/ram.hpp"
#include "../cpu_i386/pic/interrupt_caller.hpp"
#include "../cpu_i386/io/port_mapped_io.h"

#include "keyboard.hpp"
#include "mouse.hpp"

namespace ps2_controller_commands {
    // range for reading the internal ram: [START, END)
    constexpr uint8_t
    READ_RAM_START = 0x20;
    constexpr uint8_t
    READ_RAM_END = 0x40;

    // range for writing to the internal ram: [START, END)
    constexpr uint8_t
    WRITE_TO_RAM_START = 0x60;
    constexpr uint8_t
    WRITE_TO_RAM_END = 0x80;

    constexpr uint8_t
    ENABLE_FIRST_DEVICE_PORT = 0xad;
    constexpr uint8_t
    DISABLE_FIRST_DEVICE_PORT = 0xae;

    constexpr uint8_t
    ENABLE_SECOND_DEVICE_PORT = 0xa7;
    constexpr uint8_t
    DISABLE_SECOND_DEVICE_PORT = 0xa8;

    constexpr uint8_t
    WRITE_OUTPUT_OF_FIRST_DEVICE = 0xd2;
    constexpr uint8_t
    WRITE_OUTPUT_OF_SECOND_DEVICE = 0xd3;

    constexpr uint8_t
    SEND_INPUT_TO_SECOND_DEVICE = 0xd4;

};

namespace ps2_controller_input_codes {
    constexpr uint8_t
    DATA_PORT = 0x60;
    constexpr uint8_t
    STATUS_REGISTER = 0x64;
};

namespace ps2_controller_output_codes {
    constexpr uint8_t
    DATA_PORT = 0x60;
    constexpr uint8_t
    COMMAND = 0x64;
};

class ps2_controller : public pmio {
private:
    ram *mem; // ptr to cpu's ram (to be able to control a20 gate)


    // stores the latest command id sent to the controller (used in 2 byte commands)
    uint8_t curr_command;

    // output buffer holds output data of devices connected to the controller
    // or output data of the controller itself.
    uint8_t output_buff;
    uint8_t in_buf;


    // the controller has internal small ram.
    uint8_t controller_ram[0x20];


    // status register
    union {
        uint8_t raw;
        struct {
            // output buffer status (0=empty, 1=full)
            uint8_t OBF: 1;
            // input buffer status (0=empty, 1=full)
            uint8_t IBF: 1;

            // system flag
            uint8_t F0: 1;

            // type of data currently stored in the input buffer
            // 0=data is meant for a device connected to the controller
            // 1=data is meant for a controller command
            // practically this just refers to the last port that received data
            uint8_t F1: 1;

            // not defined
            uint8_t : 1;
            uint8_t : 1;

            uint8_t ST6: 1;
            uint8_t ST7: 1;
        };
    } sr;

public:
    class keyboard keyboard;

    class mouse mouse;


    // configuration byte structure
    struct ccb {
        // first device port interrupt (0=not allowed, 1=allowed)
        uint8_t KIE: 1;
        // second device port interrupt (0=not allowed, 1=allowed)
        uint8_t MIE: 1;

        // system flag
        uint8_t SYSF: 1;

        uint8_t : 1;

        // first device port clock (0=enabled, 1=disabled)
        uint8_t KE: 1;
        // second device port clock (0=enabled, 1=disabled)
        uint8_t ME: 1;

        uint8_t : 1;
    };
    // the first byte of the ram holds the configuration byte
    struct ccb *ccb = (struct ccb *) &controller_ram[0];

    union {
        uint8_t raw;
        struct {
            uint8_t SYS_RES: 1;

            // con
            uint8_t A20: 1;

            uint8_t SECOND_DEVICE_CLCK: 1;
            uint8_t SECOND_DEVICE_PORT_DATA: 1;

            // output buffer has adata from the first device
            uint8_t OB_FIRST: 1;
            // output buffer has adata from the second device
            uint8_t OB_SECOND: 1;

            uint8_t FIRST_DEVICE_CLCK: 1;
            uint8_t FIRST_DEVICE_PORT_DATA: 1;
        };
    } output_port;

public:
    ps2_controller(ram *m) : keyboard(this), mouse(this) {
        sr.raw = 0;
        mem = m;
    };


    // read data currently stored at the output buffer
    uint8_t read_out_buff(void);

    // write data to output buffer
    void write_out_buff(uint8_t v);


    // pmio interface
    uint8_t read_byte_from_port(uint16_t addr) override;

    void write_byte_to_port(uint16_t addr, uint8_t v) override;


    // send a command for the controller to execute
    void exec_command(uint8_t v);


    // read the write to the output port of the controller (not the output buffer)
    void write_to_controller_output_port(uint8_t v);

    uint8_t read_controller_output_port(void);

};


#endif
