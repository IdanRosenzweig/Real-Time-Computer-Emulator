#include "common.h"

void write_word_to_addr(uint16_t seg, uint32_t off, uint16_t val) {
    __asm__("push ds\n");
    __asm__("push ax\n");

    __asm__("xor ax, ax\n");
    __asm__("mov ds, ax\n");

    uint32_t addr = (seg << 4) + off;
    *((uint16_t*) addr) = val;

    __asm__("pop ax\n");
    __asm__("pop ds");
}

uint16_t read_word_from_addr(uint16_t seg, uint32_t off) {
    __asm__("push ds\n");
    __asm__("push ax\n");

    __asm__("xor ax, ax\n");
    __asm__("mov ds, ax\n");

    uint32_t addr = (seg << 4) + off;
    uint16_t res =  *((uint16_t*) addr);

    __asm__("pop ax\n");
    __asm__("pop ds");

    return res;
}