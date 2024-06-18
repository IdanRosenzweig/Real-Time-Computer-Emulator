#ifndef EMULATOR_SEGMENT_REGISTER_H
#define EMULATOR_SEGMENT_REGISTER_H

#include <stdint.h>

struct segment_reg_cache {
    uint32_t base;
    uint32_t limit: 20;

    union {
        uint16_t raw: 12;

        union {
            struct {
                uint8_t : 1;
                uint8_t w: 1;          // 0:r, 1:rw
                uint8_t exd: 1;
                uint8_t : 1;
            } data;
            struct {
                uint8_t : 1;
                uint8_t r: 1;          // 0:x, 1;xr
                uint8_t cnf: 1;
                uint8_t : 1;
            } code;
            struct {
                uint8_t A: 1;
                uint8_t : 2;
                uint8_t segc: 1;
            };
        } type;

        struct {
            uint8_t : 4;
            uint8_t S: 1;
            uint8_t DPL: 2;
            uint8_t P: 1;

            uint8_t AVL: 1;
            uint8_t : 1;
            uint8_t DB: 1;
            uint8_t G: 1;
        };
    } flags;
};

// segment register
struct segment_reg {
    union {
        uint16_t raw;
        struct {
            uint16_t RPL: 2;
            uint16_t TI: 1;
            uint16_t index: 13;
        };
    };

    segment_reg_cache cache;
};

enum sgreg_t {
    ES, CS, SS, DS, FS, GS, SGREGS_COUNT
};

#endif
