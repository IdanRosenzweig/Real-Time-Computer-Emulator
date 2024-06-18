#include <stdint.h>
#include <string.h>
#include "ram.hpp"

ram::ram(size_t size) : a20_controller(this) {
    mem_size = size;
    memory = new uint8_t[size];
    a20_gate.is_enabled = false;
}

ram::~ram() {
    delete[] memory;
    mem_size = 0;
}

void ram::dump_mem(uint32_t addr, size_t size) {
    addr &= ~(0x10 - 1);

//    for (uint32_t idx = 0; idx * 0x10 < size; idx++) {
//        DEBUG_MSG("0x%08x : ", addr + idx * 0x10);
//        for (uint8_t i = 0; i < 4; i++)
//            DEBUG_MSG("%08x ", ((uint32_t *) memory)[(addr + idx * 0x10) / 4 + i]);
//        DEBUG_MSG("\n");
//    }
}

size_t ram::read_data(void *dst, uint32_t src_addr, size_t size) {
    ASSERT_RANGE(src_addr, size);

    memcpy(dst, &memory[src_addr], size);
    return size;
}

size_t ram::write_data(uint32_t dst_addr, void *src, size_t size) {
    ASSERT_RANGE(dst_addr, size);

    memcpy(&memory[dst_addr], src, size);
    return size;
}
