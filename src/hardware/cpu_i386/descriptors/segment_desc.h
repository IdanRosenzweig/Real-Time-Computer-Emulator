#ifndef EMULATOR_SEGMENT_DESC_H
#define EMULATOR_SEGMENT_DESC_H

#include <stdint.h>

// segment descriptor, used in the GDT
struct segment_desc {
    uint16_t limit_l;
    uint16_t base_l;

    uint8_t base_m;

    union {
        union {
            struct {
                uint8_t : 1;
                uint8_t w: 1;        // 0:r, 1:rw
                uint8_t exd: 1;
                uint8_t : 5;
            } data;
            struct {
                uint8_t : 1;
                uint8_t r: 1;        // 0:x, 1;xr
                uint8_t cnf: 1;
                uint8_t : 5;
            } code;
            struct {
                uint8_t A: 1;
                uint8_t : 2;
                uint8_t segc: 1;
                uint8_t : 4;
            };
        } type;

        struct {
            uint8_t : 4;
            uint8_t S: 1;            // 1
            uint8_t DPL: 2;
            uint8_t P: 1;
        };
    } access_byte;

    uint8_t limit_h: 4;

    uint8_t AVL: 1;
    uint8_t : 1;
    uint8_t DB: 1;
    uint8_t G: 1;

    uint8_t base_h;
};

#endif
