#include "../common/common.h"

EXTERNC
void read_sectors_from_drive(uint16_t seg, uint16_t off) {
    int curr = 0;

    int end = curr + 0x200;
    for (; curr < end; curr++) {
        uint8_t val = in_port(0x3f5);
//        write_word_to_addr(0,   (((uint32_t) seg) << 4) + (uint32_t) off + (uint32_t) curr, val);
    }

}