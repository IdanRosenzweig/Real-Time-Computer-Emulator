#ifndef EMULATOR_SYS_SEGMENT_DESC_H
#define EMULATOR_SYS_SEGMENT_DESC_H

#include <stdint.h>

// system segment descriptor, used in the GDT
struct sys_segment_desc {
    uint16_t limit_l;
    uint16_t base_l;

    uint8_t base_m;

    union {
        struct {
            uint8_t : 1;
            uint8_t B: 1;
        };

        struct {
            uint8_t type: 3;    // 1 or 3
            uint8_t D: 1;        // 0:16bit, 1:32bit
            uint8_t S: 1;        // 0
            uint8_t DPL: 2;
            uint8_t P: 1;
        };
    } access_byte;

    uint8_t limit_h: 4;

    uint8_t AVL: 1;
    uint8_t : 2;
    uint8_t G: 1;

    uint8_t base_h;
};

#endif
