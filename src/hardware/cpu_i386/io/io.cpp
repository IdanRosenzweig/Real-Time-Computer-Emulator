#include <stdint.h>
#include <unordered_map>
#include <utility>
#include "io.hpp"

io::~io() {
//    std::unordered_map<uint16_t, pmio *>::iterator it_p;
//    std::unordered_map<uint32_t, mmio *>::iterator it_m;

//	for(it_p = port_io.begin(); it_p != port_io.end(); it_p++)
//		delete it_p->second;
//    port_io.clear();

//	for(it_m = mem_io.begin(); it_m != mem_io.end(); it_m++)
//		delete it_m->second;
//    mem_io.clear();
//    mem_io_map.clear();
}


//uint32_t io::in_io32(uint16_t addr) {
//    uint32_t v = 0;
//
//    for (int i = 0; i < 4; i++)
//        v += in_io8(addr + i) << (8 * i);
//    return v;
//}
//
//uint16_t io::in_io16(uint16_t addr) {
//    uint16_t v = 0;
//
//    for (int i = 0; i < 2; i++)
//        v += in_io8(addr + i) << (8 * i);
//    return v;
//}
//
//uint8_t io::in_io8(uint16_t addr) {
//    uint8_t v = 0;
//    pmio* handler = pmio_manager.find_handler_at_port(addr);
//    if (handler == nullptr)
//        DEBUG_ERROR("no device connected at port : 0x%04x", addr);
//    else
//        v = handler->input_byte(addr);
//
//    DEBUG_INFO( "in [0x%04x] (0x%04x)", addr, v);
//    return v;
//}

//void io::out_io32(uint16_t addr, uint32_t value) {
//    DEBUG_MSG("out_io32, addr: %x, value: %x\n", addr, value);
//    for (int i = 0; i < 4; i++)
//        out_io8(addr + i, (value >> (8 * i)) & 0xff);
//}
//
//void io::out_io16(uint16_t addr, uint16_t value) {
//    DEBUG_MSG("out_io16, addr: %x, value: %x\n", addr, value);
//    for (int i = 0; i < 2; i++)
//        out_io8(addr + i, (value >> (8 * i)) & 0xff);
//}
//
//void io::out_io8(uint16_t addr, uint8_t value) {
////    DEBUG_MSG("out_io8, addr: %x, value: %x\n", addr, value);
//    pmio* handler = pmio_manager.find_handler_at_port(addr);
//    if (handler == nullptr)
//        DEBUG_ERROR("no device connected at port : 0x%04x", addr);
//    else
//        handler->output_byte(addr, value);
//    DEBUG_INFO( "out [0x%04x] (0x%04x)", addr, value);
//}


//uint32_t io::read_memio32(uint32_t base, uint32_t offset) {
////    DEBUG_ASSERT(mem_io.count(base));
////    return mem_io[base]->read32(offset);
//    mmio *handler = mmio_manager.find_handler_of_region(base);
//    return handler->read32(offset);
//}
//
//uint16_t io::read_memio16(uint32_t base, uint32_t offset) {
////    DEBUG_ASSERT(mem_io.count(base));
////    return mem_io[base]->read16(offset);
//    mmio *handler = mmio_manager.find_handler_of_region(base);
//    return handler->read16(offset);
//}
//
//uint8_t io::read_memio8(uint32_t base, uint32_t offset) {
////    DEBUG_ASSERT(mem_io.count(base));
////    return mem_io[base]->read_byte_from_mem(offset);
//    mmio *handler = mmio_manager.find_handler_of_region(base);
//    return handler->read_byte_from_mem(offset);
//}
//
//void io::write_memio32(uint32_t base, uint32_t offset, uint32_t value) {
////    DEBUG_ASSERT(mem_io.count(base));
////    mem_io[base]->write32(offset, value);
//    mmio *handler = mmio_manager.find_handler_of_region(base);
//    return handler->write32(offset, value);
//}
//
//void io::write_memio16(uint32_t base, uint32_t offset, uint16_t value) {
////    DEBUG_ASSERT(mem_io.count(base));
////    mem_io[base]->write16(offset, value);
//    mmio *handler = mmio_manager.find_handler_of_region(base);
//    return handler->write16(offset, value);
//}
//
//void io::write_memio8(uint32_t base, uint32_t offset, uint8_t value) {
////    DEBUG_ASSERT(mem_io.count(base));
////    mem_io[base]->write_byte_to_mem(offset, value);
//    mmio *handler = mmio_manager.find_handler_of_region(base);
//    return handler->write_byte_to_mem(offset, value);
//}
