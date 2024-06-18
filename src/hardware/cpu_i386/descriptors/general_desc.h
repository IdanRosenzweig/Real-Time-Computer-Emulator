#ifndef EMULATOR_GENERAL_DESC_H
#define EMULATOR_GENERAL_DESC_H

#include <stdint.h>

#define TYPE_TSS    0x01
#define TYPE_LDT    0x02
#define TYPE_CALL    0x04
#define TYPE_TASK    0x05
#define TYPE_INTERRUPT    0x06
#define TYPE_TRAP    0x07

#define TYPE_DATA    0x10
#define TYPE_CODE    0x18

union type_attr {
    uint8_t raw;
    struct {
        uint8_t type: 3; // combine "type" with "D"?
        uint8_t D: 1;

        uint8_t S: 1;
        uint8_t DPL: 2;
        uint8_t P: 1;
    };
};

// general representation of descriptor
struct general_desc {
    uint8_t buff[8];

    type_attr get_type_attr() {
        return type_attr{.raw=buff[5]}; // todo problem here?
    }
};

#endif
