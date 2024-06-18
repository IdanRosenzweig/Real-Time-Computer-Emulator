#ifndef EMULATOR_RAM_H
#define EMULATOR_RAM_H

#include "../../../common.hpp"

#include "../a20_gate/a20_gate.h"
#include "../a20_gate/a20_gate_controller.hpp"

#define DEFAULT_MEMORY_SIZE    (1*KB)
#define ASSERT_RANGE(addr, len)    DEBUG_ASSERT(addr+len-1 < mem_size)
#define IN_RANGE(addr, len)    (addr+len-1 < mem_size)

class ram {
public:
    uint32_t mem_size;
    uint8_t *memory;

    // a20 gate
    struct a20_gate a20_gate;

    // a20 gate controller device
    a20_gate_controller a20_controller;

public:
    ram(size_t size = DEFAULT_MEMORY_SIZE);

    ~ram();

    void dump_mem(uint32_t addr, size_t size);

    size_t read_data(void *dst, uint32_t src_addr, size_t size);

    size_t write_data(uint32_t dst_addr, void *src, size_t size);


//    uint32_t read_mem32(uint32_t addr) { return IN_RANGE(addr, 4) ? *((uint32_t *) &memory[addr]) : 0; };
//
//    uint16_t read_mem16(uint32_t addr) { return IN_RANGE(addr, 2) ? *((uint16_t *) &memory[addr]) : 0; };
//
//    uint8_t read_mem8(uint32_t addr) { return IN_RANGE(addr, 1) ? memory[addr] : 0; };

    template<int NO_BITS>
    data_t<NO_BITS> read_mem(uint32_t addr) { return IN_RANGE(addr, 4) ? *((data_t<NO_BITS> *) &memory[addr]) : 0; };


//    void write_mem32(uint32_t addr, uint32_t v) { if (IN_RANGE(addr, 4)) *((uint32_t *) &memory[addr]) = v; };
//
//    void write_mem16(uint32_t addr, uint16_t v) { if (IN_RANGE(addr, 2)) *((uint16_t *) &memory[addr]) = v; };
//
//    void write_mem8(uint32_t addr, uint8_t v) { if (IN_RANGE(addr, 1)) memory[addr] = v; };

    template<int NO_BITS>
    void write_mem(uint32_t addr, uint32_t v) { if (IN_RANGE(addr, 4)) *((data_t<NO_BITS> *) &memory[addr]) = v; };

};

#endif
