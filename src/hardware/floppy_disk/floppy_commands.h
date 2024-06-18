#ifndef EMULATOR_FLOPPY_DRIVER_COMMANDS_H
#define EMULATOR_FLOPPY_DRIVER_COMMANDS_H

#include <stdint.h>

namespace floppy_commands {
    constexpr uint8_t
    FDD_READ_TRACK = 0x02;
    constexpr uint8_t
    FDD_SENSE_DRIVE_STATUS = 0x04;
    constexpr uint8_t
    FDD_WRITE_DATA = 0x05;
    constexpr uint8_t
    FDD_READ_DATA = 0x06;
    constexpr uint8_t
    FDD_RECALIBRATE = 0x07;
    constexpr uint8_t
    FDD_READ_ID = 0x0a;
    constexpr uint8_t
    FDD_FORMAT_TRACK = 0x0d;
    constexpr uint8_t
    FDD_SEEK = 0x0f;
    constexpr uint8_t
    FDD_CONFIGURE = 0x13;
};
#define MAX_COMMAND_NUM 32

#endif
