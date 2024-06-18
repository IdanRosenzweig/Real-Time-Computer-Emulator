#ifndef EMULATOR_IO_MAPPED_MEM_H
#define EMULATOR_IO_MAPPED_MEM_H

#include <stdint.h>
#include "../ram/ram.hpp"

// memory mapped io (MMIO)
// is a method of transferring data to external devices by mapping some region of the cpu RAM as dedicated to a certain device.
// in order to communicate with the device, you simply need to write and read from the mapped region in RAM
class mmio {
protected:
    uint8_t *memory;
    uint32_t paddr;
    size_t size;

public:
    virtual ~mmio() {};

    void set_mem(uint8_t *mem, uint32_t addr, size_t len) {
        memory = mem;
        paddr = addr;
        size = len;
    };

//    virtual uint32_t read32(uint32_t offset) {
//        uint32_t v = 0;
//        for (int i = 0; i < 4; i++)
//            v += read_byte_from_mem(offset + i) << (8 * i);
//        return v;
//    }
//
//    virtual uint16_t read16(uint32_t offset) {
//        uint16_t v = 0;
//        for (int i = 0; i < 2; i++)
//            v += read_byte_from_mem(offset + i) << (8 * i);
//        return v;
//    }

    template<int NO_BITS>
    data_t<NO_BITS> read_from_mem(uint32_t addr) {
        data_t<NO_BITS> v = 0;
        for (int i = 0; i < sizeof(v); i++)
            v += read_byte_from_mem(addr + i) << (8 * i);
        return v;
    };

    virtual uint8_t read_byte_from_mem(uint32_t addr) = 0;


//    virtual void write32(uint32_t offset, uint32_t v) {
//        for (int i = 0; i < 4; i++)
//            write_byte_to_mem(offset + i, (v >> (8 * i)) & 0xff);
//    }
//
//    virtual void write16(uint32_t offset, uint16_t v) {
//        for (int i = 0; i < 2; i++)
//            write_byte_to_mem(offset + i, (v >> (8 * i)) & 0xff);
//    }

    template<int NO_BITS>
    void write_to_mem(uint32_t addr, data_t<NO_BITS> v) {
        for (int i = 0; i < sizeof(v); i++) {
            write_byte_to_mem(addr + i, (v >> (8 * i)) & 0xff);
        }
    };

    virtual void write_byte_to_mem(uint32_t addr, uint8_t v) = 0;
};


#include <map>
#include <utility>

class mmio_manager {
private:
    ram *memory; // the cpu_i386's RAM
    std::map <uint32_t, std::pair<size_t, mmio *>> regions;

public:
    mmio_manager() = delete;

    mmio_manager(ram *mem) : memory(mem) {

    }

    void assign_mem_region(uint32_t addr, size_t len, mmio *handler) {
        handler->set_mem(memory->memory, addr, len);
        regions[addr] = {len, handler};
    }

private:
    std::pair <uint32_t, std::pair<size_t, mmio *>> find_region_it(uint32_t addr) {
        std::map < uint32_t, std::pair < size_t, mmio * >> ::iterator
        it;
        uint32_t reg_base;
        size_t reg_sz;

        if (regions.empty()) goto err;

        it = regions.upper_bound(addr);
        if (it == regions.begin()) goto err;

        it--;

        reg_base = it->first; // base addr of the region
        reg_sz = it->second.first; // size of the region

        // make sure the searched address is within the range [reg_base, reg_base + reg_sz)
        // (the searched address is already larger/equal than reg_base becuase of the binary search)
        if (!(addr < reg_base + reg_sz))
            goto err;

        return *it;

        err:
        return {0, {0, nullptr}};
    }

public:
    // find the mmio handler of the region that addr belongs to
    mmio *find_handler_of_region(uint32_t addr) {
        auto it = find_region_it(addr);
        if (it.second.second == nullptr) return nullptr;

        return it.second.second;
    }

    // find the base address of the region that addr belongs to
    uint32_t find_region_base(uint32_t addr) {
        auto it = find_region_it(addr);
        if (it.second.second == nullptr) return 0;

        return it.first;
    }

//    uint32_t read_memio32(uint32_t base, uint32_t offset) {
//        mmio *handler = find_handler_of_region(base);
//        return handler->read_from_mem<32>(offset);
//    }
//
//    uint16_t read_memio16(uint32_t base, uint32_t offset) {
//        mmio *handler = find_handler_of_region(base);
//        return handler->read_from_mem<16>(offset);
//    }
//
//    uint8_t read_memio8(uint32_t base, uint32_t offset) {
//        mmio *handler = find_handler_of_region(base);
//        return handler->read_byte_from_mem(offset);
//    }

    template<int NO_BITS>
    data_t<NO_BITS> read_memio(uint32_t base, uint32_t offset) {
        mmio *handler = find_handler_of_region(base);
        return handler->read_from_mem<NO_BITS>(base + offset);
    }


//    void write_memio32(uint32_t base, uint32_t offset, uint32_t value) {
//        mmio *handler = find_handler_of_region(base);
//        return handler->write_to_mem<32>(offset, value);
//    }
//
//    void write_memio16(uint32_t base, uint32_t offset, uint16_t value) {
//        mmio *handler = find_handler_of_region(base);
//        return handler->write_to_mem<16>(offset, value);
//    }
//
//    void write_memio8(uint32_t base, uint32_t offset, uint8_t value) {
//        mmio *handler = find_handler_of_region(base);
//        return handler->write_byte_to_mem(offset, value);
//    }

    template<int NO_BITS>
    void write_memio(uint32_t base, uint32_t offset, data_t<NO_BITS> value) {
        mmio *handler = find_handler_of_region(base);
        return handler->write_to_mem<NO_BITS>(base + offset, value);
    }
};


#endif
