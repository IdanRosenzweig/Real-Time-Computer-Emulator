#ifndef EMULATOR_IO_H
#define EMULATOR_IO_H


#include "../../../common.hpp"

#include "../ram/ram.hpp"
#include "port_mapped_io.h"
#include "mem_mapped_io.h"

class io {
public:
    // port mapped io manager
    class pmio_manager pmio_manager;

    // memory mapped io manager
    class mmio_manager mmio_manager;

    io() = delete;

    io(ram *mem) : mmio_manager(mem) {
    };

    ~io();


//    uint32_t in_io32(uint16_t addr);
//
//    uint16_t in_io16(uint16_t addr);
//
//    uint8_t in_io8(uint16_t addr);
//
//    void out_io32(uint16_t addr, uint32_t v);
//
//    void out_io16(uint16_t addr, uint16_t v);
//
//    void out_io8(uint16_t addr, uint8_t v);


//    void set_memio(uint32_t base, size_t len, mmio *dev) {
//        DEBUG_ASSERT(!(base & ((1 << 12) - 1)));
//
//        dev->set_mem(memory->memory, base, len);
//
////        mem_io[base] = dev;
////        for (uint32_t addr = base; addr < base + len; addr += (1 << 12))
////            mem_io_map[addr] = base;
//        mmio_manager.assign_mem_region(base, len, dev);
//    }
//
//    uint32_t get_memio_base(uint32_t addr) {
////        addr &= (~((1 << 12) - 1));
////        return mem_io_map.count(addr) ? mem_io_map[addr] : 0;
//        return mmio_manager.find_region_base(addr);
//    }
//
//    uint32_t chk_memio(uint32_t addr) {
//        return get_memio_base(addr);
//    };


//    uint32_t read_memio32(uint32_t base, uint32_t offset);
//
//    uint16_t read_memio16(uint32_t base, uint32_t offset);
//
//    uint8_t read_memio8(uint32_t base, uint32_t offset);
//
//    void write_memio32(uint32_t base, uint32_t offset, uint32_t v);
//
//    void write_memio16(uint32_t base, uint32_t offset, uint16_t v);
//
//    void write_memio8(uint32_t base, uint32_t offset, uint8_t v);



};

#endif
