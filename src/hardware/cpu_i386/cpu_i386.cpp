#include "cpu_i386.hpp"

#include "descriptors/segment_desc.h"
#include "tss/tss.h"

#include "descriptors/general_desc.h"
#include "descriptors/gate_desc.h"
#include "ivt/ivt_entry.h"

#include "instr/helper_funcs.h"

#include <iostream>

using namespace std;;

void cpu_i386::dump_regs() {
//    const char *gpreg_name[] = {"EAX", "ECX", "EDX", "EBX", "ESP", "EBP", "ESI", "EDI"};
//    const char *sgreg_name[] = {"ES", "CS", "SS", "DS", "FS", "GS"};
//    const char *dtreg_name[] = {"GDTR", "IDTR", "LDTR", " TR "};
//
//    DEBUG_MSG("EIP = 0x%08x\n", regs.ip_reg.eip);
//    for (int i = 0; i < GPREGS_COUNT; i++)
//        DEBUG_MSG("%s = 0x%08x : 0x%04x (0x%02x/0x%02x)\n", gpreg_name[i], regs.general_regs[i].reg32,
//            regs.general_regs[i].reg16, regs.general_regs[i].reg8_h, regs.general_regs[i].reg8_l);
////    DEBUG_MSG("EFLAGS = 0x%08x\n", flags_reg.get_eflags());
//    DEBUG_MSG("EFLAGS = 0x%08x\n", regs.flags_reg.eflags);
//
//    for (int i = 0; i < SGREGS_COUNT; i++) {
//        segment_reg_cache cache = regs.segment_regs[i].cache;
//        DEBUG_MSG("%s = 0x%04x {base = 0x%08x, limit = %08x, flags = %04x}\n", sgreg_name[i], regs.segment_regs[i].raw,
//            cache.base,
//            cache.limit, cache.flags.raw);
//    }
//
//    int i;
//    for (i = 0; i < LDTR; i++)
//        DEBUG_MSG("%s =        {base = 0x%08x, limit = %08x}\n", dtreg_name[i], regs.dt_regs[i].base, regs.dt_regs[i].limit);
//    for (; i < DTREGS_COUNT; i++)
//        DEBUG_MSG("%s = 0x%04x {base = 0x%08x, limit = %08x}\n", dtreg_name[i], regs.dt_regs[i].selector,
//            regs.dt_regs[i].base,
//            regs.dt_regs[i].limit);
//
//    for (i = 0; i < 5; i++)
//        DEBUG_MSG("CR%d=0x%08x ", i, regs.control_reg.get_crn(i));
//    DEBUG_MSG("\n");
}

void cpu_i386::set_segment(sgreg_t reg, uint16_t sel) {
    segment_reg sg;
    segment_reg_cache *cache = &sg.cache;

    regs.get_sgreg(reg, &sg);
    sg.raw = sel;

    if (regs.is_protected()) {

        const char *sgreg_name[] = {"ES", "CS", "SS", "DS", "FS", "GS"};

        uint32_t dt_base = regs.get_dtreg_base(sg.TI ? LDTR : GDTR);
        uint16_t dt_limit = regs.get_dtreg_limit(sg.TI ? LDTR : GDTR);
        uint16_t dt_index = sg.index << 3;

        CPU_EXCEPTION(EXP_GP, (reg == CS || reg == SS) && !dt_index);
        CPU_EXCEPTION(EXP_GP, dt_index > dt_limit);

        segment_desc gdt;
        memory.read_data(&gdt, dt_base + dt_index, sizeof(segment_desc));

        cache->base = (gdt.base_h << 24) + (gdt.base_m << 16) + gdt.base_l;
        cache->limit = (gdt.limit_h << 16) + gdt.limit_l;
        *(uint8_t * ) & cache->flags.type = *(uint8_t * ) & gdt.access_byte.type;
        cache->flags.AVL = gdt.AVL;
        cache->flags.DB = gdt.DB;
        cache->flags.G = gdt.G;



    } else
        cache->base = (uint32_t) sel << 4;

    regs.set_sgreg(reg, &sg);
}


uint32_t cpu_i386::trans_v2p(acsmode_t mode, sgreg_t seg, uint32_t vaddr) {
    uint32_t addr;

    segment_reg sg;
    regs.get_sgreg(seg, &sg);

    if (regs.is_protected()) {
        uint8_t CPL = regs.get_cpl();

        segment_reg_cache cache = sg.cache;

        uint32_t base = cache.base;
        uint32_t limit = cache.limit;
        if (cache.flags.G) limit <<= 12;

        if (cache.flags.type.segc) {
            CPU_EXCEPTION(EXP_GP, mode == MODE_WRITE);
            CPU_EXCEPTION(EXP_GP, mode == MODE_READ && !cache.flags.type.code.r);
            CPU_EXCEPTION(EXP_GP, CPL > cache.flags.DPL && !(mode == MODE_EXEC && cache.flags.type.code.cnf));
        } else {
            CPU_EXCEPTION(EXP_GP, mode == MODE_EXEC);
            CPU_EXCEPTION(EXP_GP, mode == MODE_WRITE && !cache.flags.type.data.w);
            CPU_EXCEPTION(EXP_GP, CPL > cache.flags.DPL);

            if (cache.flags.type.data.exd)
                base -= limit;
        }
        CPU_EXCEPTION(EXP_GP, vaddr > limit);

        addr = base + vaddr;
    } else {
        addr = (sg.raw << 4) + vaddr;
    }

    if (regs.control_reg.is_paging_enabled()) // paging is enabled, need to translate
        addr = paging_system.translate_addr(mode, addr);

    if (!memory.a20_gate.is_enabled)
        addr &= (1 << 20) - 1;

    return addr;
}

//uint32_t cpu_i386::trans_v2l(acsmode_t mode, sgreg_t seg, uint32_t vaddr) {
//    uint8_t CPL = regs.get_sgreg(CS) & 3;
//
//    segment_reg sg;
//    regs.get_sgreg(seg, &sg);
//
//    uint32_t laddr;
//    if (regs.is_protected()) {
//        segment_reg_cache cache = sg.cache;
//
//        uint32_t base = cache.base;
//        uint32_t limit = cache.limit;
//        if (cache.flags.G) limit <<= 12;
//
//        if (cache.flags.type.segc) {
//            CPU_EXCEPTION(EXP_GP, mode == MODE_WRITE);
//            CPU_EXCEPTION(EXP_GP, mode == MODE_READ && !cache.flags.type.code.r);
//            CPU_EXCEPTION(EXP_GP, CPL > cache.flags.DPL && !(mode == MODE_EXEC && cache.flags.type.code.cnf));
//        } else {
//            CPU_EXCEPTION(EXP_GP, mode == MODE_EXEC);
//            CPU_EXCEPTION(EXP_GP, mode == MODE_WRITE && !cache.flags.type.data.w);
//            CPU_EXCEPTION(EXP_GP, CPL > cache.flags.DPL);
//
//            if (cache.flags.type.data.exd)
//                base -= limit;
//        }
//        CPU_EXCEPTION(EXP_GP, vaddr > limit);
//
//        laddr = base + vaddr;
//        DEBUG_INFO(6, "base=0x%04x, limit=0x%02x, laddr=0x%02x", base, limit, laddr);
//    } else {
//        laddr = (sg.raw << 4) + vaddr;
//        DEBUG_INFO(6, "base=0x%04x, laddr=0x%02x", sg.raw << 4, laddr);
//    }
//
//    return laddr;
//}

//bool cpu_i386::search_tlb(uint32_t vpn, page_table_entry *res) {
//    if (!(vpn < tlb.size()) || tlb[vpn] == nullptr)
//        return false;
//
//    DEBUG_ASSERT(res);
//    *res = *tlb[vpn];
//
//    return true;
//}
//
//void cpu_i386::cache_tlb(uint32_t vpn, page_table_entry pte) {
//    if (!(vpn < tlb.size()))
//        tlb.resize(vpn + 1, nullptr);
//
//    tlb[vpn] = new page_table_entry;
//    *tlb[vpn] = pte;
//}


//void cpu_i386::push32(uint32_t value) {
//    uint32_t esp;
//
//    regs.update_gpreg(ESP, -4);
//    esp = regs.get_gpreg(ESP);
//    write_mem32(SS, esp, value);
//}
//
//uint32_t cpu_i386::pop32(void) {
//    uint32_t esp, value;
//
//    esp = regs.get_gpreg(ESP);
//    value = read_mem32(SS, esp);
//    regs.update_gpreg(ESP, 4);
//
//    return value;
//}
//
//void cpu_i386::push16(this, uint16_t value) {
//    uint16_t sp;
//
//    regs.update_gpreg(SP, -2);
//    sp = regs.get_gpreg(SP);
//    write_mem16(SS, sp, value);
//}
//
//uint16_t cpu_i386::pop16(void) {
//    uint16_t sp, value;
//
//    sp = regs.get_gpreg(SP);
//    value = read_mem16(SS, sp);
//    regs.update_gpreg(SP, 2);
//
//    return value;
//}


//uint32_t cpu_i386::read_mem32(sgreg_t seg, uint32_t addr) {
//    uint32_t paddr = trans_v2p(MODE_READ, seg, addr);
//
//    // check if the address is assigned to MMIO
//    uint32_t io_base = io.mmio_manager.find_region_base(paddr);
//    if (io_base != 0) {
//        // assigned to MMIO, read the data from device
//        return io.mmio_manager.read_memio<32>(io_base, paddr - io_base);
//    } else {
//        // not assigned to MMIO, simply read data from RAM
//        return memory.read_mem<32>(paddr);
//    }
//}
//
//uint16_t cpu_i386::read_mem16(sgreg_t seg, uint32_t addr) {
//    uint32_t paddr = trans_v2p(MODE_READ, seg, addr);
//
//    // check if the address is assigned to MMIO
//    uint32_t io_base = io.mmio_manager.find_region_base(paddr);
//    if (io_base != 0) {
//        // assigned to MMIO, read the data from device
//        return io.mmio_manager.read_memio<16>(io_base, paddr - io_base);
//    } else {
//        // not assigned to MMIO, simply read data from RAM
//        return memory.read_mem<16>(paddr);
//    }
//
////    uint32_t paddr, io_base;
////
////    paddr = trans_v2p(MODE_READ, seg, addr);
////    return (io_base = io.mmio_manager.find_region_base(paddr)) ? io.mmio_manager.read_memio<16>(io_base, paddr - io_base) : memory.read_mem<16>(paddr);
//}
//
//uint8_t cpu_i386::read_mem8(sgreg_t seg, uint32_t addr) {
//    uint32_t paddr = trans_v2p(MODE_READ, seg, addr);
//
//    // check if the address is assigned to MMIO
//    uint32_t io_base = io.mmio_manager.find_region_base(paddr);
//    if (io_base != 0) {
//        // assigned to MMIO, read the data from device
//        return io.mmio_manager.read_memio<8>(io_base, paddr - io_base);
//    } else {
//        // not assigned to MMIO, simply read data from RAM
//        return memory.read_mem<8>(paddr);
//    }
//
////    uint32_t paddr, io_base;
////
////    paddr = trans_v2p(MODE_READ, seg, addr);
////    return (io_base = io.mmio_manager.find_region_base(paddr)) ? io.mmio_manager.read_memio<8>(io_base, paddr - io_base) : memory.read_mem<8>(paddr);
//
//}


//void cpu_i386::write_mem32(sgreg_t seg, uint32_t addr, uint32_t v) {
//    uint32_t paddr = trans_v2p(MODE_WRITE, seg, addr);
//
//    // check if the address is assigned to MMIO
//    uint32_t io_base = io.mmio_manager.find_region_base(paddr);
//    if (io_base != 0) {
//        // assigned to MMIO, write the data to the device
//        io.mmio_manager.write_memio<32>(io_base, paddr - io_base, v);
//    } else {
//        // not assigned to MMIO, simply write data to RAM
//        memory.write_mem<32>(paddr, v);
//    }
//
//    //    uint32_t paddr, io_base;
////
////    paddr = trans_v2p(MODE_WRITE, seg, addr);
////    if ((io_base = io.mmio_manager.find_region_base(paddr)))
////        io.mmio_manager.write_memio<32>(io_base, paddr - io_base, v);
////    else
////        memory.write_mem<32>(paddr, v);
//}
//
//void cpu_i386::write_mem16(sgreg_t seg, uint32_t addr, uint16_t v) {
//    uint32_t paddr = trans_v2p(MODE_WRITE, seg, addr);
//
//    // check if the address is assigned to MMIO
//    uint32_t io_base = io.mmio_manager.find_region_base(paddr);
//    if (io_base != 0) {
//        // assigned to MMIO, write the data to the device
//        io.mmio_manager.write_memio<16>(io_base, paddr - io_base, v);
//    } else {
//        // not assigned to MMIO, simply write data to RAM
//        memory.write_mem<16>(paddr, v);
//    }
//
////    uint32_t paddr, io_base;
////
////    paddr = trans_v2p(MODE_WRITE, seg, addr);
////    if ((io_base = io.mmio_manager.find_region_base(paddr)))
////        io.mmio_manager.write_memio<16>(io_base, paddr - io_base, v);
////    else
////        memory.write_mem<16>(paddr, v);
//}
//
//void cpu_i386::write_mem8(sgreg_t seg, uint32_t addr, uint8_t v) {
//    uint32_t paddr = trans_v2p(MODE_WRITE, seg, addr);
//
//    // check if the address is assigned to MMIO
//    uint32_t io_base = io.mmio_manager.find_region_base(paddr);
//    if (io_base != 0) {
//        // assigned to MMIO, write the data to the device
//        io.mmio_manager.write_memio<8>(io_base, paddr - io_base, v);
//    } else {
//        // not assigned to MMIO, simply write data to RAM
//        memory.write_mem<8>(paddr, v);
//    }
//
////    uint32_t paddr, io_base;
////
////    paddr = trans_v2p(MODE_WRITE, seg, addr);
////    if ((io_base = io.mmio_manager.find_region_base(paddr)))
////        io.mmio_manager.write_memio<8>(io_base, paddr - io_base, v);
////    else
////        memory.write_mem<8>(paddr, v);
//}


//void cpu_i386::queue_interrupt(struct interrupt intr) {
//    interrupts_system.que.push(intr);
//};
//
//bool cpu_i386::chk_irq(void) {
//    if (!regs.flags_reg.interrupt)
//        return false;
//
//    if (!interrupts_system.pic_m.chk_intr_req())
//        return false;
//
//    int8_t n_intr = interrupts_system.pic_m.get_nintr();
//    if (n_intr < 0)
//        n_intr = interrupts_system.pic_s.get_nintr();
//
////    cout << "new interrupt chk and queued\n";
//    queue_interrupt(interrupt{n_intr, true});
//    return true;
//}

void cpu_i386::hundle_interrupt(void) {
    if (interrupts_system.que.empty())
        return;

//    cout << "handeling interrupt\n";
    struct interrupt intr = interrupts_system.que.front();
    interrupts_system.que.pop();

    uint8_t n = intr.id;
    bool hard = intr.hard;

    if (regs.is_protected()) {
        uint8_t CPL = regs.get_cpl();

        uint32_t idt_base = regs.get_dtreg_base(IDTR);
        uint16_t idt_offset = n << 3;

        uint16_t idt_limit = regs.get_dtreg_limit(IDTR);
        CPU_EXCEPTION(EXP_GP, idt_offset > idt_limit);

        gate_desc idt;
        memory.read_data(&idt, idt_base + idt_offset, sizeof(gate_desc));
        uint8_t RPL = ((segment_reg *) &(idt.sel))->RPL;

//        DEBUG_INFO( "int 0x%02x [CPL : %d, DPL : %d RPL : %d] (EIP : 0x%04x, CS : 0x%04x)", n, CPL, idt.DPL, RPL,
//             (idt.offset_h << 16) + idt.offset_l, idt.sel);

        CPU_EXCEPTION(EXP_NP, !idt.P);
        CPU_EXCEPTION(EXP_GP, CPL < RPL);
        CPU_EXCEPTION(EXP_GP, !hard && CPL > idt.DPL);

//        uint16_t cs = regs.get_sgreg(CS);
//        set_segment(CS, idt.sel);
//        save_regs(CPL ^ RPL, cs);
//        regs.ip_reg.eip = (idt.offset_h << 16) + idt.offset_l;

        // saving important registers
        save_regs(CPL ^ RPL);
        // jumping to the addr that the descriptor holds
        set_segment(CS, idt.sel);
        regs.ip_reg.eip = (idt.offset_h << 16) + idt.offset_l;

        if (idt.type == TYPE_INTERRUPT)
            regs.flags_reg.interrupt = (false);

    } else {
        // find the addr of the IDT
        uint32_t idt_base = regs.get_dtreg_base(IDTR);
        uint16_t idt_offset = n << 2;

        uint16_t idt_limit = regs.get_dtreg_limit(IDTR);
        CPU_EXCEPTION(EXP_GP, idt_offset > idt_limit);

        // getting the ivt entry
        ivt_entry ivt;
        ivt.raw = memory.read_mem<32>(idt_base + idt_offset);

//        uint16_t cs = regs.get_sgreg(CS);
//        set_segment(CS, ivt.segment);
//        save_regs(false, cs);
//        regs.ip_reg.ip = ivt.offset;

        // saving important registers
        save_regs(false);
        // jumping to the addr that the ivt entry holds
        set_segment(CS, ivt.segment);
        regs.ip_reg.ip = ivt.offset;

        // regs.flags_reg.interrupt = (false);
//        DEBUG_INFO( "int 0x%02x (IP : 0x%04x, CS : 0x%04x)", n, ivt.offset, ivt.segment);
    }
}

void cpu_i386::save_regs(bool chpl) {
    if (regs.control_reg.is_protected()) {
        if (chpl) {
            uint32_t base, limit, esp;
            uint16_t ss;
            TSS tss;

            base = regs.get_dtreg_base(TR);
            limit = regs.get_dtreg_limit(TR);
            CPU_EXCEPTION(EXP_TS, limit < sizeof(TSS) - 1);

            memory.read_data(&tss, base, sizeof(TSS));

            ss = regs.get_sgreg(SS);
            esp = regs.get_gpreg(ESP);
            set_segment(SS, tss.ss0);
            regs.set_gpreg(ESP, tss.esp0);

            push32(this, ss);
            push32(this, esp);
        }
        push32(this, regs.flags_reg.eflags);
//        push32(this, cs);
        push32(this, regs.get_sgreg(CS));
        push32(this, regs.ip_reg.eip);
    } else {
//        push16(this, regs.flags_reg.get_flags());
        push16(this, regs.flags_reg.flags);
//        push16(this, cs);
        push16(this, regs.get_sgreg(CS));
        push16(this, regs.ip_reg.ip);
    }
}

