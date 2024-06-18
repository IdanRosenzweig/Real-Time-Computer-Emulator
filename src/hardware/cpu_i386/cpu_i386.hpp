#ifndef EMULATOR_CPU_I386_H
#define EMULATOR_CPU_I386_H

#include "../../common.hpp"
#include "exceptions/exceptions.h"

#include "registers/registers.hpp"
#include "ram/ram.hpp"
#include "io/io.hpp"
#include "pic/pic.hpp"
#include "pit/pit.hpp"
#include "a20_gate/a20_gate_controller.hpp"

#include "paging/access_mode.h"
#include "paging/paging_system.h"

#include "interrupts/interrupts_system.h"

#include <vector>
#include <queue>
#include <iostream>
using namespace std;;

struct processor_settings {
    size_t mem_size;
    uint32_t init_ip_reg;
    uint16_t init_cs_reg;
};

class cpu_i386 {
public:
    // registers
    registers regs;

    // RAM
    class ram memory;

    // IO
    class io io;

    // programmable interval timer
    class pit pit;

    // paging system
    class paging_system paging_system;

    // interrupts system
    struct interrupts_system interrupts_system;

    bool halt;

    cpu_i386(processor_settings set) :
            regs(set.init_ip_reg, set.init_cs_reg), memory(set.mem_size), io(&memory),
            pit(),
            paging_system(&regs, &memory),
            interrupts_system(&regs) {
        halt = false;
    };

private:

    // translate virtual/logical addresses to physical RAM addresses
    uint32_t trans_v2p(acsmode_t mode, sgreg_t seg, uint32_t vaddr);

    // save important registers on the stack
    void save_regs(bool chpl);

    void restore_regs(void);

public:

    void dump_regs();

    void set_segment(sgreg_t seg, uint16_t v);

public:

//    uint32_t read_mem32(sgreg_t seg, uint32_t addr);
//
//    uint16_t read_mem16(sgreg_t seg, uint32_t addr);
//
//    uint8_t read_mem8(sgreg_t seg, uint32_t addr);

    template<int NO_BITS>
    data_t<NO_BITS> read_mem(sgreg_t seg, uint32_t addr) {
        uint32_t paddr = trans_v2p(MODE_READ, seg, addr);

        // check if the address is assigned to MMIO
        uint32_t io_base = io.mmio_manager.find_region_base(paddr);
        if (io_base != 0) {
            // assigned to MMIO, read the data from device
//            cout<<"reading from mmio, base:" << std::hex << io_base << ", off: " << paddr - io_base<<"\n";
            data_t<NO_BITS> res = io.mmio_manager.read_memio<NO_BITS>(io_base, paddr - io_base);
//            cout << "res: " << res << "\n";
            return res;
        } else {
            // not assigned to MMIO, simply read data from RAM
            return memory.read_mem<NO_BITS>(paddr);
        }

//    uint32_t paddr, io_base;
//
//    paddr = trans_v2p(MODE_READ, seg, addr);
//    return (io_base = io.mmio_manager.find_region_base(paddr)) ? io.mmio_manager.read_memio<16>(io_base, paddr - io_base) : memory.read_mem<16>(paddr);
    }


//    void write_mem32(sgreg_t seg, uint32_t addr, uint32_t v);
//
//    void write_mem16(sgreg_t seg, uint32_t addr, uint16_t v);
//
//    void write_mem8(sgreg_t seg, uint32_t addr, uint8_t v);

    template<int NO_BITS>
    void write_mem(sgreg_t seg, uint32_t addr, data_t<NO_BITS> v)  {
        uint32_t paddr = trans_v2p(MODE_WRITE, seg, addr);

        // check if the address is assigned to MMIO
        uint32_t io_base = io.mmio_manager.find_region_base(paddr);
        if (io_base != 0) {
            // assigned to MMIO, write the data to the device
//            cout<<"writing to mmio, paddr: " << std::hex << addr << ", base:" << io_base << ", off: " << paddr - io_base<<", value: " << v << "\n";
            io.mmio_manager.write_memio<NO_BITS>(io_base, paddr - io_base, v);
        } else {
            // not assigned to MMIO, simply write data to RAM
            memory.write_mem<NO_BITS>(paddr, v);
        }
    }


//    uint8_t read_code8(int index) {
////        return exec_mem8_seg(CS, regs.ip_reg.eip + index);
//        return memory.read_mem<8>(trans_v2p(MODE_EXEC, CS, regs.ip_reg.eip + index));
//    };
//
//    uint16_t read_code16(int index) {
////        return exec_mem16_seg(CS, regs.ip_reg.eip + index);
//        return memory.read_mem<16>(trans_v2p(MODE_EXEC, CS, regs.ip_reg.eip + index));
//    };
//
//    uint32_t read_code32(int index) {
////        return exec_mem32_seg(CS, regs.ip_reg.eip + index);
//        return memory.read_mem<32>(trans_v2p(MODE_EXEC, CS, regs.ip_reg.eip + index));
//    };

    template<int NO_BITS>
    data_t<NO_BITS> read_code(int index = 0) {
        return memory.read_mem<NO_BITS>(trans_v2p(MODE_EXEC, CS, regs.ip_reg.eip + index));
    }


public:
    // handle the next interrupt
    void hundle_interrupt(void);

    void iret(void);


};

#endif
