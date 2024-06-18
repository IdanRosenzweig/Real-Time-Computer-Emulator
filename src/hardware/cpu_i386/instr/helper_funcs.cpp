#include "helper_funcs.h"

//bool get_is_mode32(cpu_i386 *proc, x86_instr_t *instr) {
//    return proc->regs.is_mode32() ^ instr->operand_override;
//}
//
//bool get_is_addr_override(cpu_i386 *proc, x86_instr_t *instr) {
//    return instr->operand_override ^ instr->addr_override;
//}

//sgreg_t select_segment(x86_instr_t *instruction) {
//    if (instruction->segment_override.used)
//        return instruction->segment_override.reg;
//    else
//        return instruction->curr_seg;
//}

void set_rm32(class cpu_i386 *proc, x86_instr_t *instruction, uint32_t value) {
    if (instruction->modrm.mod == 3)
        proc->regs.set_gpreg(static_cast<general_regs_t>(EAX + instruction->modrm.rm), value);
    else
        proc->write_mem<32>(select_segment(instruction), calc_modrm(proc, instruction), value);
}

uint32_t get_rm32(class cpu_i386 *proc, x86_instr_t *instruction) {
    if (instruction->modrm.mod == 3)
        return proc->regs.get_gpreg(static_cast<general_regs_t>(EAX + instruction->modrm.rm));
    else
        return proc->read_mem<32>(select_segment(instruction), calc_modrm(proc, instruction));
}

void set_r32(class cpu_i386 *proc, x86_instr_t *instruction, uint32_t value) {
    proc->regs.set_gpreg(static_cast<general_regs_t>(EAX + instruction->modrm.reg), value);
}

uint32_t get_r32(class cpu_i386 *proc, x86_instr_t *instruction) {
    return proc->regs.get_gpreg(static_cast<general_regs_t>(EAX + instruction->modrm.reg));
}

void set_moffs32(class cpu_i386 *proc, x86_instr_t *instruction, uint32_t value) {
    instruction->curr_seg = DS;
    return proc->write_mem<32>(select_segment(instruction), instruction->moffs, value);
}

uint32_t get_moffs32(class cpu_i386 *proc, x86_instr_t *instruction) {
    instruction->curr_seg = DS;
    return proc->read_mem<32>(select_segment(instruction), instruction->moffs);
}

void set_rm16(class cpu_i386 *proc, x86_instr_t *instruction, uint16_t value) {
    if (instruction->modrm.mod == 3)
        proc->regs.set_gpreg(static_cast<general_regs_t>(AX + instruction->modrm.rm), value);
    else
        proc->write_mem<16>(select_segment(instruction), calc_modrm(proc, instruction), value);
}

uint16_t get_rm16(class cpu_i386 *proc, x86_instr_t *instruction) {
    if (instruction->modrm.mod == 3)
        return proc->regs.get_gpreg(static_cast<general_regs_t>(AX + instruction->modrm.rm));
    else
        return proc->read_mem<16>(select_segment(instruction), calc_modrm(proc, instruction));
}

void set_r16(class cpu_i386 *proc, x86_instr_t *instruction, uint16_t value) {
    proc->regs.set_gpreg(static_cast<general_regs_t>(AX + instruction->modrm.reg), value);
}

uint16_t get_r16(class cpu_i386 *proc, x86_instr_t *instruction) {
    return proc->regs.get_gpreg(static_cast<general_regs_t>(AX + instruction->modrm.reg));
}

void set_moffs16(class cpu_i386 *proc, x86_instr_t *instruction, uint16_t value) {
    instruction->curr_seg = DS;
    return proc->write_mem<16>(select_segment(instruction), instruction->moffs, value);
}

uint16_t get_moffs16(class cpu_i386 *proc, x86_instr_t *instruction) {
    instruction->curr_seg = DS;
    return proc->read_mem<16>(select_segment(instruction), instruction->moffs);
}

void set_rm8(class cpu_i386 *proc, x86_instr_t *instruction, uint8_t value) {
    if (instruction->modrm.mod == 3)
        proc->regs.set_gpreg(static_cast<general_regs_t>(AL + instruction->modrm.rm), value);
    else
        proc->write_mem<8>(select_segment(instruction), calc_modrm(proc, instruction), value);
}

uint8_t get_rm8(class cpu_i386 *proc, x86_instr_t *instruction) {
    if (instruction->modrm.mod == 3)
        return proc->regs.get_gpreg(static_cast<general_regs_t>(AL + instruction->modrm.rm));
    else
        return proc->read_mem<8>(select_segment(instruction), calc_modrm(proc, instruction));
}

void set_r8(class cpu_i386 *proc, x86_instr_t *instruction, uint8_t value) {
    proc->regs.set_gpreg(static_cast<general_regs_t>(AL + instruction->modrm.reg), value);
}

void set_moffs8(class cpu_i386 *proc, x86_instr_t *instruction, uint8_t value) {
    instruction->curr_seg = DS;
    return proc->write_mem<8>(select_segment(instruction), instruction->moffs, value);
}

uint8_t get_moffs8(class cpu_i386 *proc, x86_instr_t *instruction) {
    instruction->curr_seg = DS;
    return proc->read_mem<8>(select_segment(instruction), instruction->moffs);
}

uint8_t get_r8(class cpu_i386 *proc, x86_instr_t *instruction) {
    return proc->regs.get_gpreg(static_cast<general_regs_t>(AL + instruction->modrm.reg));
}

uint32_t get_m(class cpu_i386 *proc, x86_instr_t *instruction) {
    return calc_modrm(proc, instruction);
}

void set_sreg(class cpu_i386 *proc, x86_instr_t *instruction, uint16_t value) {
    proc->set_segment(static_cast<sgreg_t>(instruction->modrm.reg), value);
}

uint16_t get_sreg(class cpu_i386 *proc, x86_instr_t *instruction) {
    return proc->regs.get_sgreg(static_cast<sgreg_t>(instruction->modrm.reg));
}

void set_crn(class cpu_i386 *proc, x86_instr_t *instruction, uint32_t value) {

    proc->regs.control_reg.set_crn(instruction->modrm.reg, value);
}

uint32_t get_crn(class cpu_i386 *proc, x86_instr_t *instruction) {
    return proc->regs.control_reg.get_crn(instruction->modrm.reg);
}

uint32_t calc_modrm(class cpu_i386 *proc, x86_instr_t *instruction) {
    DEBUG_ASSERT(instruction->modrm.mod != 3);

    instruction->curr_seg = DS;
    if (get_is_mode32(proc, instruction) ^ get_is_addr_override(proc, instruction))
        return calc_modrm32(proc, instruction);
    else
        return calc_modrm16(proc, instruction);
}

uint32_t calc_modrm16(class cpu_i386 *proc, x86_instr_t *instruction) {
    uint32_t addr = 0;

    switch (instruction->modrm.mod) {
        case 1:
            addr += instruction->disp.disp8;
            break;
        case 2:
            addr += instruction->disp.disp16;
            break;
    }

    switch (instruction->modrm.rm) {
        case 0:
        case 1:
        case 7:
            addr += proc->regs.get_gpreg(BX);
            break;
        case 2:
        case 3:
        case 6:
            if (instruction->modrm.mod == 0 && instruction->modrm.rm == 6)
                addr += instruction->disp.disp16;
            else {
                addr += proc->regs.get_gpreg(BP);
                instruction->curr_seg = SS;
            }
            break;
    }

    if (instruction->modrm.rm < 6) {
        if (instruction->modrm.rm % 2)
            addr += proc->regs.get_gpreg(DI);
        else
            addr += proc->regs.get_gpreg(SI);
    }

    return addr;
}

uint32_t calc_modrm32(class cpu_i386 *proc, x86_instr_t *instruction) {
    uint32_t addr = 0;

    switch (instruction->modrm.mod) {
        case 1:
            addr += instruction->disp.disp8;
            break;
        case 2:
            addr += instruction->disp.disp32;
            break;
    }

    switch (instruction->modrm.rm) {
        case 4:
            addr += calc_sib(proc, instruction);
            break;
        case 5:
            if (instruction->modrm.mod == 0) {
                addr += instruction->disp.disp32;
                break;
            }
        default:
            instruction->curr_seg = (instruction->modrm.rm == 5) ? SS : DS;
            addr += proc->regs.get_gpreg(static_cast<general_regs_t>(EAX + instruction->modrm.rm));
    }

    return addr;
}

uint32_t calc_sib(class cpu_i386 *proc, x86_instr_t *instruction) {
    uint32_t base;

    if (instruction->sib.base == 5 && instruction->modrm.mod == 0)
        base = instruction->disp.disp32;
    else if (instruction->sib.base == 4) {
        if (instruction->sib.scale ==
            0) {        // instr->sib.base == 4, instr->sib.index ==4, instr->sib.scale == 0 : [esp]
            instruction->curr_seg = SS;
            base = 0;
        } else
            DEBUG_ERROR("not implemented SIB (base = %d, index = %d, scale = %d)\n", instruction->sib.base,
                  instruction->sib.index, instruction->sib.scale);
    } else {
        instruction->curr_seg = (instruction->modrm.rm == 5) ? SS : DS;
        base = proc->regs.get_gpreg(static_cast<general_regs_t>(EAX + instruction->sib.base));
    }

    return base + proc->regs.get_gpreg(static_cast<general_regs_t>(EAX + instruction->sib.index)) *
                  (1 << instruction->sib.scale);
/*
	if(instr->sib.base!=5 && instr->sib.index!=4)
		return proc->regs.get_gpreg(static_cast<general_regs_t>(EAX+instr->sib.base)) + proc->regs.get_gpreg(static_cast<general_regs_t>(EAX+instr->sib.index)) * (1<<instr->sib.scale);
	else if(instr->sib.base==4 && instr->sib.index==4 && instr->sib.scale==0)
		return proc->regs.get_gpreg(static_cast<general_regs_t>(EAX+ESP));
//	else if(instr->sib.base==5 && instr->modrm.mod==0)
//		return instr->disp.disp32 + proc->regs.get_gpreg(static_cast<general_regs_t>(EAX+instr->sib.index)) * (1<<instr->sib.scale);
*/
}


#include "../exceptions/exceptions.h"
#include "../descriptors/general_desc.h"
#include "../descriptors/segment_desc.h"
#include "../descriptors/sys_segment_desc.h"
#include "../tss/tss.h"

uint8_t type_descriptor(class cpu_i386 *proc, uint16_t sel) {
    uint16_t gdt_limit = proc->regs.get_dtreg_limit(GDTR);
    CPU_EXCEPTION(EXP_GP, sel > gdt_limit);

    uint32_t gdt_base = proc->regs.get_dtreg_base(GDTR);

    general_desc desc;
    proc->memory.read_data(&desc, gdt_base + sel, sizeof(general_desc));
    if (desc.get_type_attr().S) {
        if (((segment_desc *) &desc)->access_byte.type.segc)
            return TYPE_CODE;
        else
            return TYPE_DATA;
    } else if (desc.get_type_attr().type == 3)
        return TYPE_TSS;

    return desc.get_type_attr().type;
}

void set_ldtr(class cpu_i386 *proc, uint16_t sel) {
    uint16_t gdt_limit = proc->regs.get_dtreg_limit(GDTR);
    CPU_EXCEPTION(EXP_GP, sel > gdt_limit);

    uint32_t gdt_base = proc->regs.get_dtreg_base(GDTR);
    segment_desc ldt;
    proc->memory.read_data(&ldt, gdt_base + sel, sizeof(segment_desc));

    uint32_t base = (ldt.base_h << 24) + (ldt.base_m << 16) + ldt.base_l;
    uint16_t limit = (ldt.limit_h << 16) + ldt.limit_l;
    proc->regs.set_dtreg(LDTR, sel, base, limit);
}

void set_tr(class cpu_i386 *proc, uint16_t sel) {
    uint32_t gdt_base, base;
    uint16_t gdt_limit, limit;
    sys_segment_desc tssdesc;

    gdt_base = proc->regs.get_dtreg_base(GDTR);
    gdt_limit = proc->regs.get_dtreg_limit(GDTR);
    CPU_EXCEPTION(EXP_GP, sel > gdt_limit);

    proc->memory.read_data(&tssdesc, gdt_base + sel, sizeof(sys_segment_desc));
    CPU_EXCEPTION(EXP_GP, tssdesc.access_byte.type != TYPE_TSS);

    base = (tssdesc.base_h << 24) + (tssdesc.base_m << 16) + tssdesc.base_l;
    limit = (tssdesc.limit_h << 16) + tssdesc.limit_l;

    proc->regs.set_dtreg(TR, sel, base, limit);
}

void switch_task(class cpu_i386 *proc, uint16_t sel) {
    uint32_t base;
    uint16_t prev, limit;
    TSS old_tss, new_tss;

    prev = proc->regs.get_dtreg_selector(TR);
    base = proc->regs.get_dtreg_base(TR);
    limit = proc->regs.get_dtreg_limit(TR);
    CPU_EXCEPTION(EXP_GP, limit < sizeof(TSS) - 1);

    proc->memory.read_data(&old_tss, base, sizeof(TSS));
    old_tss.cr3 = proc->regs.control_reg.get_crn(3);
    old_tss.eip = proc->regs.ip_reg.eip;
    old_tss.eflags = proc->regs.flags_reg.eflags;
    old_tss.eax = proc->regs.get_gpreg(EAX);
    old_tss.ecx = proc->regs.get_gpreg(ECX);
    old_tss.edx = proc->regs.get_gpreg(EDX);
    old_tss.ebx = proc->regs.get_gpreg(EBX);
    old_tss.esp = proc->regs.get_gpreg(ESP);
    old_tss.ebp = proc->regs.get_gpreg(EBP);
    old_tss.esi = proc->regs.get_gpreg(ESI);
    old_tss.edi = proc->regs.get_gpreg(EDI);
    old_tss.es = proc->regs.get_sgreg(ES);
    old_tss.cs = proc->regs.get_sgreg(CS);
    old_tss.ss = proc->regs.get_sgreg(SS);
    old_tss.ds = proc->regs.get_sgreg(DS);
    old_tss.fs = proc->regs.get_sgreg(FS);
    old_tss.gs = proc->regs.get_sgreg(GS);
    old_tss.ldtr = proc->regs.get_dtreg_selector(LDTR);
    proc->memory.write_data(base, &old_tss, sizeof(TSS));

    set_tr(proc, sel);

    base = proc->regs.get_dtreg_base(TR);
    limit = proc->regs.get_dtreg_limit(TR);
    CPU_EXCEPTION(EXP_GP, limit < sizeof(TSS) - 1);

    proc->memory.read_data(&new_tss, base, sizeof(TSS));
    new_tss.prev_sel = prev;
    proc->memory.write_data(base, &new_tss, sizeof(TSS));
    proc->regs.control_reg.cr3.raw = new_tss.cr3;
    proc->regs.ip_reg.eip = new_tss.eip;
    proc->regs.flags_reg.eflags = (new_tss.eflags);
    proc->regs.set_gpreg(EAX, new_tss.eax);
    proc->regs.set_gpreg(ECX, new_tss.ecx);
    proc->regs.set_gpreg(EDX, new_tss.edx);
    proc->regs.set_gpreg(EBX, new_tss.ebx);
    proc->regs.set_gpreg(ESP, new_tss.esp);
    proc->regs.set_gpreg(EBP, new_tss.ebp);
    proc->regs.set_gpreg(ESI, new_tss.esi);
    proc->regs.set_gpreg(EDI, new_tss.edi);
    proc->set_segment(ES, new_tss.es);
    proc->set_segment(CS, new_tss.cs);
    proc->set_segment(SS, new_tss.ss);
    proc->set_segment(DS, new_tss.ds);
    proc->set_segment(FS, new_tss.fs);
    proc->set_segment(GS, new_tss.gs);

    set_ldtr(proc, new_tss.ldtr);
}

void jmpf(class cpu_i386 *proc, uint16_t sel, uint32_t eip) {
    if (proc->regs.is_protected()) {
        switch (type_descriptor(proc, sel)) {
            case TYPE_CODE:
                goto jmp;
            case TYPE_TSS:
                switch_task(proc, sel);
                return;
        }
    }

    jmp:

    proc->set_segment(CS, sel);
    proc->regs.ip_reg.eip = eip;
}

void callf(class cpu_i386 *proc, uint16_t sel, uint32_t eip) {
/*
	if(proc->is_protected()){
		switch(type_descriptor(proc, sel)){
			case TYPE_CODE:
				goto jmp;
			case TYPE_TSS:
				switch_task(proc, sel);
				return;
		}
	}

jmp:
*/
    segment_reg cs;

    cs.raw = proc->regs.get_sgreg(CS);
    uint8_t RPL = sel & 3;

    if (cs.RPL ^ RPL) {
        CPU_EXCEPTION(EXP_GP, RPL < cs.RPL);
        push32(proc, proc->regs.get_sgreg(SS));
        push32(proc, proc->regs.get_gpreg(ESP));
    }

    push32(proc, cs.raw);
    push32(proc, proc->regs.ip_reg.eip);

    proc->set_segment(CS, sel);
    proc->regs.ip_reg.eip = eip;
}

// checks if the DPL is premiered regarding the current CPL
bool chk_ring(class cpu_i386 *proc, uint8_t DPL) {
    return proc->regs.get_cpl() <= DPL;
}


void push32(class cpu_i386 *proc, uint32_t value) {
    uint32_t esp;

    proc->regs.update_gpreg(ESP, -4);
    esp = proc->regs.get_gpreg(ESP);
    proc->write_mem<32>(SS, esp, value);
}

uint32_t pop32(class cpu_i386 *proc) {
    uint32_t esp, value;

    esp = proc->regs.get_gpreg(ESP);
    value = proc->read_mem<32>(SS, esp);
    proc->regs.update_gpreg(ESP, 4);

    return value;
}

void push16(class cpu_i386 *proc, uint16_t value) {
    uint16_t sp;

    proc->regs.update_gpreg(SP, -2);
    sp = proc->regs.get_gpreg(SP);
    proc->write_mem<16>(SS, sp, value);
}

uint16_t pop16(class cpu_i386 *proc) {
    uint16_t sp, value;

    sp = proc->regs.get_gpreg(SP);
    value = proc->read_mem<16>(SS, sp);
    proc->regs.update_gpreg(SP, 2);

    return value;
}