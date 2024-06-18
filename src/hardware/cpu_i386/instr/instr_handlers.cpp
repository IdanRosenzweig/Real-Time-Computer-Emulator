#include "instr_handlers.h"

#include "../exceptions/exceptions.h"
#include "helper_funcs.h"
#include "../tss/tss.h"

#include <iostream>

using namespace std;;

void retf(cpu_i386 *proc, x86_instr_t *instr) {
    segment_reg cs;

    uint8_t CPL = proc->regs.get_cpl();

    proc->regs.ip_reg.eip = pop32(proc);
    cs.raw = pop32(proc);

    if (cs.RPL ^ CPL) {
        uint32_t esp;
        uint16_t ss;

        esp = pop32(proc);
        ss = pop32(proc);
        proc->regs.set_gpreg(ESP, esp);
        proc->set_segment(SS, ss);
    }

    proc->set_segment(CS, cs.raw);
}

void iret(cpu_i386 *proc, x86_instr_t *instruction) {
    if (get_is_mode32(proc, instruction)) {
        uint8_t CPL = proc->regs.get_cpl();

        proc->regs.ip_reg.eip = pop32(proc);

        segment_reg cs;
        cs.raw = pop32(proc);

        flags_reg flags_reg;
        flags_reg.eflags = pop32(proc);
        proc->regs.flags_reg.eflags = (flags_reg.eflags);
//
        if (flags_reg.NT) {
            uint32_t base;
            TSS tss;

            base = proc->regs.get_dtreg_base(TR);
            proc->memory.read_data(&tss, base, sizeof(TSS));
            switch_task(proc, tss.prev_sel);
        } else {
            if (cs.RPL > CPL) {
                uint32_t esp;
                uint16_t ss;

                esp = pop32(proc);
                ss = pop32(proc);
                proc->regs.set_gpreg(ESP, esp);
                proc->set_segment(SS, ss);
            }
        }

        proc->set_segment(CS, cs.raw);

    } else {
        proc->regs.ip_reg.ip = pop16(proc);

        uint16_t cs = pop16(proc);

        proc->regs.flags_reg.flags = pop16(proc);

        proc->set_segment(CS, cs);

    }
}

void add_rm8_r8(cpu_i386 *proc, x86_instr_t *instr) {
    uint8_t rm8, r8;

    rm8 = get_rm8(proc, instr);
    r8 = get_r8(proc, instr);
    set_rm8(proc, instr, rm8 + r8);
    proc->regs.flags_reg.update_reg_add(rm8, r8);
}

void add_r8_rm8(cpu_i386 *proc, x86_instr_t *instr) {
    uint8_t r8, rm8;

    r8 = get_r8(proc, instr);
    rm8 = get_rm8(proc, instr);
    set_r8(proc, instr, r8 + rm8);
    proc->regs.flags_reg.update_reg_add(r8, rm8);
}

void add_al_imm8(cpu_i386 *proc, x86_instr_t *instr) {
    uint8_t al;

    al = proc->regs.get_gpreg(AL);
    proc->regs.set_gpreg(AL, al + instr->imm.imm8);
    proc->regs.flags_reg.update_reg_add(al, instr->imm.imm8);
}

void or_rm8_r8(cpu_i386 *proc, x86_instr_t *instr) {
    uint8_t rm8, r8;

    rm8 = get_rm8(proc, instr);
    r8 = get_r8(proc, instr);
    set_rm8(proc, instr, rm8 | r8);
    proc->regs.flags_reg.update_reg_or(rm8, r8);
}

void or_al_imm8(cpu_i386 *proc, x86_instr_t *instr) {
    uint8_t al;

    al = proc->regs.get_gpreg(AL);
    proc->regs.set_gpreg(AL, al | instr->imm.imm8);
    proc->regs.flags_reg.update_reg_or(al, instr->imm.imm8);
}

void or_r8_rm8(cpu_i386 *proc, x86_instr_t *instr) {
    uint8_t r8, rm8;

    r8 = get_r8(proc, instr);
    rm8 = get_rm8(proc, instr);
    set_r8(proc, instr, r8 | rm8);
    proc->regs.flags_reg.update_reg_or(r8, rm8);
}

void and_rm8_r8(cpu_i386 *proc, x86_instr_t *instr) {
    uint8_t rm8, r8;

    rm8 = get_rm8(proc, instr);
    r8 = get_r8(proc, instr);
    set_rm8(proc, instr, rm8 & r8);
    proc->regs.flags_reg.update_reg_and(rm8, r8);
}

void and_r8_rm8(cpu_i386 *proc, x86_instr_t *instr) {
    uint8_t r8, rm8;

    r8 = get_r8(proc, instr);
    rm8 = get_rm8(proc, instr);
    set_r8(proc, instr, r8 & rm8);
    proc->regs.flags_reg.update_reg_and(r8, rm8);
}

void and_al_imm8(cpu_i386 *proc, x86_instr_t *instr) {
    uint8_t al;

    al = proc->regs.get_gpreg(AL);
    proc->regs.set_gpreg(AL, al & instr->imm.imm8);
    proc->regs.flags_reg.update_reg_and(al, instr->imm.imm8);
}

void sub_rm8_r8(cpu_i386 *proc, x86_instr_t *instr) {
    uint8_t rm8, r8;

    rm8 = get_rm8(proc, instr);
    r8 = get_r8(proc, instr);
    set_rm8(proc, instr, rm8 - r8);
    proc->regs.flags_reg.update_reg_sub(rm8, r8);
}

void sub_r8_rm8(cpu_i386 *proc, x86_instr_t *instr) {
    uint8_t r8, rm8;

    r8 = get_r8(proc, instr);
    rm8 = get_rm8(proc, instr);
    set_r8(proc, instr, r8 - rm8);
    proc->regs.flags_reg.update_reg_sub(r8, rm8);
}

void sub_al_imm8(cpu_i386 *proc, x86_instr_t *instr) {
    uint8_t al;

    al = proc->regs.get_gpreg(AL);
    proc->regs.set_gpreg(AL, al - instr->imm.imm8);
    proc->regs.flags_reg.update_reg_sub(al, instr->imm.imm8);
}

void xor_rm8_r8(cpu_i386 *proc, x86_instr_t *instr) {
    uint8_t rm8, r8;

    rm8 = get_rm8(proc, instr);
    r8 = get_r8(proc, instr);
    set_rm8(proc, instr, rm8 ^ r8);
}

void xor_r8_rm8(cpu_i386 *proc, x86_instr_t *instr) {
    uint8_t r8, rm8;

    r8 = get_r8(proc, instr);
    rm8 = get_rm8(proc, instr);
    set_r8(proc, instr, r8 ^ rm8);
}

void xor_al_imm8(cpu_i386 *proc, x86_instr_t *instr) {
    uint8_t al;

    al = proc->regs.get_gpreg(AL);
    proc->regs.set_gpreg(AL, al ^ instr->imm.imm8);
}

void cmp_rm8_r8(cpu_i386 *proc, x86_instr_t *instr) {
    uint8_t rm8, r8;

    rm8 = get_rm8(proc, instr);
    r8 = get_r8(proc, instr);
    proc->regs.flags_reg.update_reg_sub(rm8, r8);
}

void cmp_r8_rm8(cpu_i386 *proc, x86_instr_t *instr) {
    uint8_t r8, rm8;

    r8 = get_r8(proc, instr);
    rm8 = get_rm8(proc, instr);
    proc->regs.flags_reg.update_reg_sub(r8, rm8);
}

void cmp_al_imm8(cpu_i386 *proc, x86_instr_t *instr) {
    uint8_t al;

    al = proc->regs.get_gpreg(AL);
    proc->regs.flags_reg.update_reg_sub(al, instr->imm.imm8);
}

#define JCC_REL8(cc, is_flag) \
void j ## cc ## _rel8(cpu_i386 *proc, x86_instr_t *instr){ \
    if(is_flag) \
        proc->regs.ip_reg.ip += instr->imm.imm8; \
}

JCC_REL8(o, proc->regs.flags_reg.overflow)

JCC_REL8(no, !proc->regs.flags_reg.overflow)

JCC_REL8(b, proc->regs.flags_reg.carry)

JCC_REL8(nb, !proc->regs.flags_reg.carry)

JCC_REL8(z, proc->regs.flags_reg.zero)

JCC_REL8(nz, !proc->regs.flags_reg.zero)

JCC_REL8(be, proc->regs.flags_reg.carry || proc->regs.flags_reg.zero)

JCC_REL8(a, !(proc->regs.flags_reg.carry || proc->regs.flags_reg.zero))

JCC_REL8(s, proc->regs.flags_reg.sign)

JCC_REL8(ns, !proc->regs.flags_reg.sign)

JCC_REL8(p, proc->regs.flags_reg.parity)

JCC_REL8(np, !proc->regs.flags_reg.parity)

JCC_REL8(l, proc->regs.flags_reg.sign != proc->regs.flags_reg.overflow)

JCC_REL8(nl, proc->regs.flags_reg.sign == proc->regs.flags_reg.overflow)

JCC_REL8(le, proc->regs.flags_reg.zero || (proc->regs.flags_reg.sign != proc->regs.flags_reg.overflow))

JCC_REL8(nle, !proc->regs.flags_reg.zero && (proc->regs.flags_reg.sign == proc->regs.flags_reg.overflow))

void test_rm8_r8(cpu_i386 *proc, x86_instr_t *instr) {
    uint8_t rm8, r8;

    rm8 = get_rm8(proc, instr);
    r8 = get_r8(proc, instr);
    proc->regs.flags_reg.update_reg_and(rm8, r8);
}

void xchg_r8_rm8(cpu_i386 *proc, x86_instr_t *instr) {
    uint8_t r8, rm8;

    r8 = get_r8(proc, instr);
    rm8 = get_rm8(proc, instr);
    set_r8(proc, instr, rm8);
    set_rm8(proc, instr, r8);
}

void mov_rm8_r8(cpu_i386 *proc, x86_instr_t *instr) {
    uint8_t r8;

    r8 = get_r8(proc, instr);
    set_rm8(proc, instr, r8);
}

void mov_r8_rm8(cpu_i386 *proc, x86_instr_t *instr) {
    uint8_t rm8;

    rm8 = get_rm8(proc, instr);
    set_r8(proc, instr, rm8);
}

void mov_sreg_rm16(cpu_i386 *proc, x86_instr_t *instr) {
    uint16_t rm16;

    rm16 = get_rm16(proc, instr);
    set_sreg(proc, instr, rm16);
}

void nop(cpu_i386 *proc, x86_instr_t *instr) {}        // xchg eax, eax

void mov_al_moffs8(cpu_i386 *proc, x86_instr_t *instr) {
    proc->regs.set_gpreg(AL, get_moffs8(proc, instr));
}

void mov_moffs8_al(cpu_i386 *proc, x86_instr_t *instr) {
    set_moffs8(proc, instr, proc->regs.get_gpreg(AL));
}

void test_al_imm8(cpu_i386 *proc, x86_instr_t *instr) {
    uint8_t al;

    al = proc->regs.get_gpreg(AL);
    proc->regs.flags_reg.update_reg_and(al, instr->imm.imm8);
}

void mov_r8_imm8(cpu_i386 *proc, x86_instr_t *instr) {
    uint8_t reg;

    reg = instr->opcode & ((1 << 3) - 1);
    proc->regs.set_gpreg(static_cast<general_regs_t>(AL + reg), instr->imm.imm8);
}

void mov_rm8_imm8(cpu_i386 *proc, x86_instr_t *instr) {
    set_rm8(proc, instr, instr->imm.imm8);
}

//void retf(cpu_i386 *proc, x86_instr_t *instr) {
//    EmuInstr::retf();
//}

void int3(cpu_i386 *proc, x86_instr_t *instr) {
    proc->dump_regs();
    proc->memory.dump_mem((proc->regs.get_sgreg(SS) << 4) + proc->regs.get_gpreg(ESP) - 0x40, 0x80);
}

void int_imm8(cpu_i386 *proc, x86_instr_t *instr) {
    proc->interrupts_system.queue_interrupt(interrupt{instr->imm.imm8, false});
}

//void iret(cpu_i386 *proc, x86_instr_t *instr) {
//    EmuInstr::iret();
//}

void in_al_imm8(cpu_i386 *proc, x86_instr_t *instr) {
    proc->regs.set_gpreg(AL, proc->io.pmio_manager.in_io<8>((uint8_t) instr->imm.imm8));
}

void out_imm8_al(cpu_i386 *proc, x86_instr_t *instr) {
    uint8_t al;

    al = proc->regs.get_gpreg(AL);
    proc->io.pmio_manager.out_io<8>((uint8_t) instr->imm.imm8, al);
}

void jmp(cpu_i386 *proc, x86_instr_t *instr) {
    proc->regs.ip_reg.ip += (instr->imm.imm8);
}

void in_al_dx(cpu_i386 *proc, x86_instr_t *instr) {
    uint16_t dx;

    dx = proc->regs.get_gpreg(DX);
    proc->regs.set_gpreg(AL, proc->io.pmio_manager.in_io<8>(dx));
}

void out_dx_al(cpu_i386 *proc, x86_instr_t *instr) {
    uint16_t dx;
    uint8_t al;

    dx = proc->regs.get_gpreg(DX);
    al = proc->regs.get_gpreg(AL);
    proc->io.pmio_manager.out_io<8>(dx, al);
}

void cli(cpu_i386 *proc, x86_instr_t *instr) {
    proc->regs.flags_reg.interrupt = (false);
}

void sti(cpu_i386 *proc, x86_instr_t *instr) {
    proc->regs.flags_reg.interrupt = (true);
}

void cld(cpu_i386 *proc, x86_instr_t *instr) {
    proc->regs.flags_reg.direction = (false);
}

void STD(cpu_i386 *proc, x86_instr_t *instr) {
    proc->regs.flags_reg.direction = (true);
}

void hlt(cpu_i386 *proc, x86_instr_t *instr) {
    CPU_EXCEPTION(EXP_GP, !chk_ring(proc, 0));
    proc->halt = true;
    //proc->dump_regs();
}

void ltr_rm16(cpu_i386 *proc, x86_instr_t *instr) {
    uint16_t rm16;

    CPU_EXCEPTION(EXP_GP, !chk_ring(proc, 0));
    rm16 = get_rm16(proc, instr);
    set_tr(proc, rm16);
}

void mov_r32_crn(cpu_i386 *proc, x86_instr_t *instr) {
    uint32_t crn;

    crn = get_crn(proc, instr);
    proc->regs.set_gpreg(static_cast<general_regs_t>(EAX + instr->modrm.rm), crn);    // set_r32
}

void mov_crn_r32(cpu_i386 *proc, x86_instr_t *instr) {
    uint32_t r32;

    CPU_EXCEPTION(EXP_GP, !chk_ring(proc, 0));
    r32 = proc->regs.get_gpreg(static_cast<general_regs_t>(EAX + instr->modrm.rm));    // get_r32
    set_crn(proc, instr, r32);
}

#define SETCC_RM8(cc, is_flag) \
void set ## cc ## _rm8(cpu_i386 *proc, x86_instr_t *instr){ \
    proc->regs.set_gpreg(static_cast<general_regs_t>(EAX+instr->modrm.rm), is_flag); \
}

SETCC_RM8(o, proc->regs.flags_reg.overflow)

SETCC_RM8(no, !proc->regs.flags_reg.overflow)

SETCC_RM8(b, proc->regs.flags_reg.carry)

SETCC_RM8(nb, !proc->regs.flags_reg.carry)

SETCC_RM8(z, proc->regs.flags_reg.zero)

SETCC_RM8(nz, !proc->regs.flags_reg.zero)

SETCC_RM8(be, proc->regs.flags_reg.carry || proc->regs.flags_reg.zero)

SETCC_RM8(a, !(proc->regs.flags_reg.carry || proc->regs.flags_reg.zero))

SETCC_RM8(s, proc->regs.flags_reg.sign)

SETCC_RM8(ns, !proc->regs.flags_reg.sign)

SETCC_RM8(p, proc->regs.flags_reg.parity)

SETCC_RM8(np, !proc->regs.flags_reg.parity)

SETCC_RM8(l, proc->regs.flags_reg.sign != proc->regs.flags_reg.overflow)

SETCC_RM8(nl, proc->regs.flags_reg.sign == proc->regs.flags_reg.overflow)

SETCC_RM8(le, proc->regs.flags_reg.zero || (proc->regs.flags_reg.sign != proc->regs.flags_reg.overflow))

SETCC_RM8(nle,
          !proc->regs.flags_reg.zero && (proc->regs.flags_reg.sign == proc->regs.flags_reg.overflow))


void code_80(cpu_i386 *proc, x86_instr_t *instr) {
    switch (instr->modrm.reg) {
        case 0:
            add_rm8_imm8(proc, instr);
            break;
        case 1:
            or_rm8_imm8(proc, instr);
            break;
        case 2:
            adc_rm8_imm8(proc, instr);
            break;
        case 3:
            sbb_rm8_imm8(proc, instr);
            break;
        case 4:
            and_rm8_imm8(proc, instr);
            break;
        case 5:
            sub_rm8_imm8(proc, instr);
            break;
        case 6:
            xor_rm8_imm8(proc, instr);
            break;
        case 7:
            cmp_rm8_imm8(proc, instr);
            break;
        default:
            DEBUG_ERROR("not implemented: 0x80 /%d\n", instr->modrm.reg);
    }
}

void code_82(cpu_i386 *proc, x86_instr_t *instr) {
    code_80(proc, instr);
}

void code_c0(cpu_i386 *proc, x86_instr_t *instr) {
    switch (instr->modrm.reg) {
        //	case 0:	rol_rm8_imm8();		break;
        //	case 1:	ror_rm8_imm8();		break;
        //	case 2:	rcl_rm8_imm8();		break;
        //	case 3:	rcr_rm8_imm8();		break;
        case 4:
            shl_rm8_imm8(proc, instr);
            break;
        case 5:
            shr_rm8_imm8(proc, instr);
            break;
        case 6:
            sal_rm8_imm8(proc, instr);
            break;
        case 7:
            sar_rm8_imm8(proc, instr);
            break;
        default:
            DEBUG_ERROR("not implemented: 0xc0 /%d\n", instr->modrm.reg);
    }
}

void code_f6(cpu_i386 *proc, x86_instr_t *instr) {
    switch (instr->modrm.reg) {
        case 0:
            test_rm8_imm8(proc, instr);
            break;
        case 2:
            not_rm8(proc, instr);
            break;
        case 3:
            neg_rm8(proc, instr);
            break;
        case 4:
            mul_ax_al_rm8(proc, instr);
            break;
        case 5:
            imul_ax_al_rm8(proc, instr);
            break;
        case 6:
            div_al_ah_rm8(proc, instr);
            break;
        case 7:
            idiv_al_ah_rm8(proc, instr);
            break;
        default:
            DEBUG_ERROR("not implemented: 0xf6 /%d\n", instr->modrm.reg);
    }
}


void add_rm8_imm8(cpu_i386 *proc, x86_instr_t *instr) {
    uint8_t rm8;

    rm8 = get_rm8(proc, instr);
    set_rm8(proc, instr, rm8 + instr->imm.imm8);
    proc->regs.flags_reg.update_reg_add(rm8, instr->imm.imm8);
}

void or_rm8_imm8(cpu_i386 *proc, x86_instr_t *instr) {
    uint8_t rm8;

    rm8 = get_rm8(proc, instr);
    set_rm8(proc, instr, rm8 | instr->imm.imm8);
    proc->regs.flags_reg.update_reg_or(rm8, instr->imm.imm8);
}

void adc_rm8_imm8(cpu_i386 *proc, x86_instr_t *instr) {
    uint8_t rm8, cf;

    rm8 = get_rm8(proc, instr);
    cf = proc->regs.flags_reg.carry;
    set_rm8(proc, instr, rm8 + instr->imm.imm8 + cf);
    proc->regs.flags_reg.update_reg_add(rm8, instr->imm.imm8 + cf);
}

void sbb_rm8_imm8(cpu_i386 *proc, x86_instr_t *instr) {
    uint8_t rm8, cf;

    rm8 = get_rm8(proc, instr);
    cf = proc->regs.flags_reg.carry;
    set_rm8(proc, instr, rm8 - instr->imm.imm8 - cf);
    proc->regs.flags_reg.update_reg_sub(rm8, instr->imm.imm8 + cf);
}

void and_rm8_imm8(cpu_i386 *proc, x86_instr_t *instr) {
    uint8_t rm8;

    rm8 = get_rm8(proc, instr);
    set_rm8(proc, instr, rm8 & instr->imm.imm8);
    proc->regs.flags_reg.update_reg_and(rm8, instr->imm.imm8);
}

void sub_rm8_imm8(cpu_i386 *proc, x86_instr_t *instr) {
    uint8_t rm8;

    rm8 = get_rm8(proc, instr);
    set_rm8(proc, instr, rm8 - instr->imm.imm8);
    proc->regs.flags_reg.update_reg_sub(rm8, instr->imm.imm8);
}

void xor_rm8_imm8(cpu_i386 *proc, x86_instr_t *instr) {
    uint8_t rm8;

    rm8 = get_rm8(proc, instr);
    set_rm8(proc, instr, rm8 ^ instr->imm.imm8);
}

void cmp_rm8_imm8(cpu_i386 *proc, x86_instr_t *instr) {
    uint8_t rm8;

    rm8 = get_rm8(proc, instr);
    proc->regs.flags_reg.update_reg_sub(rm8, instr->imm.imm8);
}


void shl_rm8_imm8(cpu_i386 *proc, x86_instr_t *instr) {
    uint8_t rm8;

    rm8 = get_rm8(proc, instr);
    set_rm8(proc, instr, rm8 << instr->imm.imm8);
    proc->regs.flags_reg.update_reg_shl(rm8, instr->imm.imm8);
}

void shr_rm8_imm8(cpu_i386 *proc, x86_instr_t *instr) {
    uint8_t rm8;

    rm8 = get_rm8(proc, instr);
    set_rm8(proc, instr, rm8 >> instr->imm.imm8);
    proc->regs.flags_reg.update_reg_shr(rm8, instr->imm.imm8);
}

void sal_rm8_imm8(cpu_i386 *proc, x86_instr_t *instr) {
    int8_t rm8_s;

    rm8_s = get_rm8(proc, instr);
    set_rm8(proc, instr, rm8_s << instr->imm.imm8);
//	EFLAGS_UPDATE_SAL(rm8_s, instr->imm.imm8);
}

void sar_rm8_imm8(cpu_i386 *proc, x86_instr_t *instr) {
    int8_t rm8_s;

    rm8_s = get_rm8(proc, instr);
    set_rm8(proc, instr, rm8_s >> instr->imm.imm8);
//	EFLAGS_UPDATE_SAR(rm8_s, instr->imm.imm8);
}


void test_rm8_imm8(cpu_i386 *proc, x86_instr_t *instr) {
    uint8_t rm8, imm8;

    rm8 = get_rm8(proc, instr);
    imm8 = proc->read_code<8>();
    proc->regs.ip_reg.eip += 1;
    proc->regs.flags_reg.update_reg_and(rm8, imm8);
}

void not_rm8(cpu_i386 *proc, x86_instr_t *instr) {
    uint8_t rm8;

    rm8 = get_rm8(proc, instr);
    set_rm8(proc, instr, ~rm8);
}

void neg_rm8(cpu_i386 *proc, x86_instr_t *instr) {
    int8_t rm8_s;

    rm8_s = get_rm8(proc, instr);
    set_rm8(proc, instr, -rm8_s);
    proc->regs.flags_reg.update_reg_sub((uint8_t) 0, rm8_s);
}

void mul_ax_al_rm8(cpu_i386 *proc, x86_instr_t *instr) {
    uint8_t rm8, al;
    uint16_t val;

    rm8 = get_rm8(proc, instr);
    al = proc->regs.get_gpreg(AL);
    val = al * rm8;

    proc->regs.set_gpreg(AX, val);

    proc->regs.flags_reg.update_reg_mul(al, rm8);
}

void imul_ax_al_rm8(cpu_i386 *proc, x86_instr_t *instr) {
    int8_t rm8_s, al_s;
    int16_t val_s;

    rm8_s = get_rm8(proc, instr);
    al_s = proc->regs.get_gpreg(AL);
    val_s = al_s * rm8_s;

    proc->regs.set_gpreg(AX, val_s);

    proc->regs.flags_reg.update_reg_imul(al_s, rm8_s);
}

void div_al_ah_rm8(cpu_i386 *proc, x86_instr_t *instr) {
    uint8_t rm8;
    uint16_t ax;

    rm8 = get_rm8(proc, instr);
    ax = proc->regs.get_gpreg(AX);

    proc->regs.set_gpreg(AL, ax / rm8);
    proc->regs.set_gpreg(AH, ax % rm8);
}

void idiv_al_ah_rm8(cpu_i386 *proc, x86_instr_t *instr) {
    int8_t rm8_s;
    int16_t ax_s;

    rm8_s = get_rm8(proc, instr);
    ax_s = proc->regs.get_gpreg(AX);

    proc->regs.set_gpreg(AL, ax_s / rm8_s);
    proc->regs.set_gpreg(AH, ax_s % rm8_s);
}

/******************************************************************/




void add_rm16_r16(class cpu_i386 *proc, x86_instr_t *instruction) {
    uint16_t rm16, r16;

    rm16 = get_rm16(proc, instruction);
    r16 = get_r16(proc, instruction);
    set_rm16(proc, instruction, rm16 + r16);
    proc->regs.flags_reg.update_reg_add(rm16, r16);
}

void add_r16_rm16(class cpu_i386 *proc, x86_instr_t *instruction) {
    uint16_t r16, rm16;

    r16 = get_r16(proc, instruction);
    rm16 = get_rm16(proc, instruction);
    set_r16(proc, instruction, r16 + rm16);
    proc->regs.flags_reg.update_reg_add(r16, rm16);
}

void add_ax_imm16(class cpu_i386 *proc, x86_instr_t *instruction) {
    uint16_t ax;

    ax = proc->regs.get_gpreg(AX);
    proc->regs.set_gpreg(AX, ax + instruction->imm.imm16);
    proc->regs.flags_reg.update_reg_add(ax, instruction->imm.imm16);
}

void push_es16(class cpu_i386 *proc, x86_instr_t *instruction) {
    push16(proc, proc->regs.get_sgreg(ES));
}

void pop_es16(class cpu_i386 *proc, x86_instr_t *instruction) {
    proc->set_segment(ES, pop16(proc));
}

void or_rm16_r16(class cpu_i386 *proc, x86_instr_t *instruction) {
    uint16_t rm16, r16;

    rm16 = get_rm16(proc, instruction);
    r16 = get_r16(proc, instruction);
    set_rm16(proc, instruction, rm16 | r16);
    proc->regs.flags_reg.update_reg_or(rm16, r16);
}

void or_r16_rm16(class cpu_i386 *proc, x86_instr_t *instruction) {
    uint16_t r16, rm16;

    r16 = get_r16(proc, instruction);
    rm16 = get_rm16(proc, instruction);
    set_r16(proc, instruction, r16 | rm16);
    proc->regs.flags_reg.update_reg_or(r16, rm16);
}

void or_ax_imm16(class cpu_i386 *proc, x86_instr_t *instruction) {
    uint16_t ax;

    ax = proc->regs.get_gpreg(AX);
    proc->regs.set_gpreg(AX, ax | instruction->imm.imm16);
    proc->regs.flags_reg.update_reg_or(ax, instruction->imm.imm16);
}

void adc_rm16_r16(class cpu_i386 *proc, x86_instr_t *instruction) {
    uint16_t rm16 = get_rm16(proc, instruction);
    uint16_t r16 = get_r16(proc, instruction);
    set_rm16(proc, instruction, rm16 + r16 + proc->regs.flags_reg.carry);
    proc->regs.flags_reg.update_reg_add(rm16, r16, proc->regs.flags_reg.carry);
}

void push_ss16(class cpu_i386 *proc, x86_instr_t *instruction) {
    push16(proc, proc->regs.get_sgreg(SS));
}

void pop_ss16(class cpu_i386 *proc, x86_instr_t *instruction) {
    proc->set_segment(SS, pop16(proc));
}

void push_ds16(class cpu_i386 *proc, x86_instr_t *instruction) {
    push16(proc, proc->regs.get_sgreg(DS));
}

void pop_ds16(class cpu_i386 *proc, x86_instr_t *instruction) {
    proc->set_segment(DS, pop16(proc));
}

void and_rm16_r16(class cpu_i386 *proc, x86_instr_t *instruction) {
    uint16_t rm16, r16;

    rm16 = get_rm16(proc, instruction);
    r16 = get_r16(proc, instruction);
    set_rm16(proc, instruction, rm16 & r16);
    proc->regs.flags_reg.update_reg_add(rm16, r16);
}

void and_r16_rm16(class cpu_i386 *proc, x86_instr_t *instruction) {
    uint16_t r16, rm16;

    r16 = get_r16(proc, instruction);
    rm16 = get_rm16(proc, instruction);
    set_r16(proc, instruction, r16 & rm16);
    proc->regs.flags_reg.update_reg_add(r16, rm16);
}

void and_ax_imm16(class cpu_i386 *proc, x86_instr_t *instruction) {
    uint16_t ax;

    ax = proc->regs.get_gpreg(AX);
    proc->regs.set_gpreg(AX, ax & instruction->imm.imm16);
    proc->regs.flags_reg.update_reg_add(ax, instruction->imm.imm16);
}

void sub_rm16_r16(class cpu_i386 *proc, x86_instr_t *instruction) {
    uint16_t rm16, r16;

    rm16 = get_rm16(proc, instruction);
    r16 = get_r16(proc, instruction);
    set_rm16(proc, instruction, rm16 - r16);
    proc->regs.flags_reg.update_reg_sub(rm16, r16);
}

void sub_r16_rm16(class cpu_i386 *proc, x86_instr_t *instruction) {
    uint16_t r16, rm16;

    r16 = get_r16(proc, instruction);
    rm16 = get_rm16(proc, instruction);
    set_r16(proc, instruction, r16 - rm16);
    proc->regs.flags_reg.update_reg_sub(r16, rm16);
}

void sub_ax_imm16(class cpu_i386 *proc, x86_instr_t *instruction) {
    uint16_t ax;

    ax = proc->regs.get_gpreg(AX);
    proc->regs.set_gpreg(AX, ax - instruction->imm.imm16);
    proc->regs.flags_reg.update_reg_sub(ax, instruction->imm.imm16);
}

void xor_rm16_r16(class cpu_i386 *proc, x86_instr_t *instruction) {
    uint16_t rm16, r16;

    rm16 = get_rm16(proc, instruction);
    r16 = get_r16(proc, instruction);
    set_rm16(proc, instruction, rm16 ^ r16);
}

void xor_r16_rm16(class cpu_i386 *proc, x86_instr_t *instruction) {
    uint16_t r16, rm16;

    r16 = get_r16(proc, instruction);
    rm16 = get_rm16(proc, instruction);
    set_r16(proc, instruction, r16 ^ rm16);
}

void xor_ax_imm16(class cpu_i386 *proc, x86_instr_t *instruction) {
    uint16_t ax;

    ax = proc->regs.get_gpreg(AX);
    proc->regs.set_gpreg(AX, ax ^ instruction->imm.imm16);
}

void cmp_rm16_r16(class cpu_i386 *proc, x86_instr_t *instruction) {
    uint16_t rm16, r16;

    rm16 = get_rm16(proc, instruction);
    r16 = get_r16(proc, instruction);
    proc->regs.flags_reg.update_reg_sub(rm16, r16);
}

void cmp_r16_rm16(class cpu_i386 *proc, x86_instr_t *instruction) {
    uint16_t r16, rm16;

    r16 = get_r16(proc, instruction);
    rm16 = get_rm16(proc, instruction);
    proc->regs.flags_reg.update_reg_sub(r16, rm16);
}

void cmp_ax_imm16(class cpu_i386 *proc, x86_instr_t *instruction) {
    uint16_t ax;

    ax = proc->regs.get_gpreg(AX);
    proc->regs.flags_reg.update_reg_sub(ax, instruction->imm.imm16);
}

void inc_r16(class cpu_i386 *proc, x86_instr_t *instruction) {
    uint8_t reg;
    uint16_t r16;

    reg = instruction->opcode & ((1 << 3) - 1);
    r16 = proc->regs.get_gpreg(static_cast<general_regs_t>(AX + reg));
    proc->regs.set_gpreg(static_cast<general_regs_t>(AX + reg), r16 + 1);
    proc->regs.flags_reg.update_reg_add(r16, 1);
}

void dec_r16(class cpu_i386 *proc, x86_instr_t *instruction) {
    uint8_t reg;
    uint16_t r16;

    reg = instruction->opcode & ((1 << 3) - 1);
    r16 = proc->regs.get_gpreg(static_cast<general_regs_t>(AX + reg));
    proc->regs.set_gpreg(static_cast<general_regs_t>(AX + reg), r16 - 1);
    proc->regs.flags_reg.update_reg_sub(r16, 1);
}

void push_r16(class cpu_i386 *proc, x86_instr_t *instruction) {
    uint8_t reg;

    reg = instruction->opcode & ((1 << 3) - 1);
    push16(proc, proc->regs.get_gpreg(static_cast<general_regs_t>(AX + reg)));
}

void pop_r16(class cpu_i386 *proc, x86_instr_t *instruction) {
    uint8_t reg;

    reg = instruction->opcode & ((1 << 3) - 1);
    proc->regs.set_gpreg(static_cast<general_regs_t>(AX + reg), pop16(proc));
}

void pusha(class cpu_i386 *proc, x86_instr_t *instruction) {
    uint16_t sp;

    sp = proc->regs.get_gpreg(SP);

    push16(proc, proc->regs.get_gpreg(AX));
    push16(proc, proc->regs.get_gpreg(CX));
    push16(proc, proc->regs.get_gpreg(DX));
    push16(proc, proc->regs.get_gpreg(BX));
    push16(proc, sp);
    push16(proc, proc->regs.get_gpreg(BP));
    push16(proc, proc->regs.get_gpreg(SI));
    push16(proc, proc->regs.get_gpreg(DI));
}

void popa(class cpu_i386 *proc, x86_instr_t *instruction) {
    uint16_t sp;

    proc->regs.set_gpreg(DI, pop16(proc));
    proc->regs.set_gpreg(SI, pop16(proc));
    proc->regs.set_gpreg(BP, pop16(proc));
    sp = pop16(proc);
    proc->regs.set_gpreg(BX, pop16(proc));
    proc->regs.set_gpreg(DX, pop16(proc));
    proc->regs.set_gpreg(CX, pop16(proc));
    proc->regs.set_gpreg(AX, pop16(proc));

    proc->regs.set_gpreg(SP, sp);
}

void push_imm16_16(class cpu_i386 *proc, x86_instr_t *instruction) {
    push16(proc, instruction->imm.imm16);
}

void imul_r16_rm16_imm16(class cpu_i386 *proc, x86_instr_t *instruction) {
    int16_t rm16_s;

    rm16_s = get_rm16(proc, instruction);
    set_r16(proc, instruction, rm16_s * instruction->imm.imm16);
    proc->regs.flags_reg.update_reg_imul(rm16_s, instruction->imm.imm16);
}

void push_imm8_16(class cpu_i386 *proc, x86_instr_t *instruction) {
    push16(proc, instruction->imm.imm8);
}

void imul_r16_rm16_imm8(class cpu_i386 *proc, x86_instr_t *instruction) {
    int16_t rm16_s;

    rm16_s = get_rm16(proc, instruction);
    set_r16(proc, instruction, rm16_s * instruction->imm.imm8);
    proc->regs.flags_reg.update_reg_imul(rm16_s, instruction->imm.imm8);
}

void test_rm16_r16(class cpu_i386 *proc, x86_instr_t *instruction) {
    uint16_t rm16, r16;

    rm16 = get_rm16(proc, instruction);
    r16 = get_r16(proc, instruction);
    proc->regs.flags_reg.update_reg_add(rm16, r16);
}

void xchg_r16_rm16(class cpu_i386 *proc, x86_instr_t *instruction) {
    uint16_t r16, rm16;

    r16 = get_r16(proc, instruction);
    rm16 = get_rm16(proc, instruction);
    set_r16(proc, instruction, rm16);
    set_rm16(proc, instruction, r16);
}

void mov_rm16_r16(class cpu_i386 *proc, x86_instr_t *instruction) {
    uint16_t r16;

    r16 = get_r16(proc, instruction);
    set_rm16(proc, instruction, r16);
}

void mov_r16_rm16(class cpu_i386 *proc, x86_instr_t *instruction) {
    uint16_t rm16;

    rm16 = get_rm16(proc, instruction);
    set_r16(proc, instruction, rm16);
}

void mov_rm16_sreg(class cpu_i386 *proc, x86_instr_t *instruction) {
    uint16_t sreg;

    sreg = get_sreg(proc, instruction);
    set_rm16(proc, instruction, sreg);
}

void lea_r16_m16(class cpu_i386 *proc, x86_instr_t *instruction) {
    uint16_t m16;

    m16 = get_m(proc, instruction);
    set_r16(proc, instruction, m16);
}

void xchg_r16_ax(class cpu_i386 *proc, x86_instr_t *instruction) {
    uint16_t r16, ax;

    r16 = get_r16(proc, instruction);
    ax = proc->regs.get_gpreg(AX);
    set_r16(proc, instruction, ax);
    proc->regs.set_gpreg(AX, r16);
}

void cbw(class cpu_i386 *proc, x86_instr_t *instruction) {
    int8_t al_s;

    al_s = proc->regs.get_gpreg(AL);
    proc->regs.set_gpreg(AX, al_s);
}

void cwd(class cpu_i386 *proc, x86_instr_t *instruction) {
    uint16_t ax;

    ax = proc->regs.get_gpreg(AX);
    proc->regs.set_gpreg(DX, ax & (1 << 15) ? -1 : 0);
}

void callf_ptr16_16(class cpu_i386 *proc, x86_instr_t *instruction) {
    callf(proc, instruction->ptr16, instruction->imm.imm16);
}

void pushf_16(class cpu_i386 *proc, x86_instr_t *instruction) {
    push16(proc, proc->regs.flags_reg.flags);
}

void popf_16(class cpu_i386 *proc, x86_instr_t *instruction) {
    proc->regs.flags_reg.flags = pop16(proc);
}

void mov_ax_moffs16(class cpu_i386 *proc, x86_instr_t *instruction) {
    proc->regs.set_gpreg(AX, get_moffs16(proc, instruction));
}

void mov_moffs16_ax(class cpu_i386 *proc, x86_instr_t *instruction) {
    set_moffs16(proc, instruction, proc->regs.get_gpreg(AX));
}

void movsw_m16_m16(class cpu_i386 *proc, x86_instr_t *instruction) {

    repeat:
    uint16_t word = proc->read_mem<16>(DS, proc->regs.get_gpreg(SI));
    proc->write_mem<16>(ES, proc->regs.get_gpreg(DI), word);

    proc->regs.update_gpreg(SI, proc->regs.flags_reg.direction ? -2 : 2);
    proc->regs.update_gpreg(DI, proc->regs.flags_reg.direction ? -2 : 2);


    if (instruction->repetition_pre) {
        proc->regs.update_gpreg(CX, -1);
        switch (instruction->repetition_pre) {
            case REPZ:
                if (!proc->regs.get_gpreg(CX) || proc->regs.flags_reg.zero)
                    break;
                goto repeat;
            case REPNZ:
                if (!proc->regs.get_gpreg(CX) || proc->regs.flags_reg.zero)
                    break;
                goto repeat;
            default:
                break;
        }
    }
}

void cmps_m8_m8_16(class cpu_i386 *proc, x86_instr_t *instruction) {
    uint8_t m8_s, m8_d;

    repeat:
    m8_s = proc->read_mem<8>(select_segment(instruction), proc->regs.get_gpreg(SI));
    m8_d = proc->read_mem<8>(ES, proc->regs.get_gpreg(DI));
    proc->regs.flags_reg.update_reg_sub(m8_s, m8_d);

    proc->regs.update_gpreg(SI, proc->regs.flags_reg.direction ? -1 : 1);
    proc->regs.update_gpreg(DI, proc->regs.flags_reg.direction ? -1 : 1);


    if (instruction->repetition_pre) {
        proc->regs.update_gpreg(CX, -1);
        switch (instruction->repetition_pre) {
            case REPZ:
                if (!proc->regs.get_gpreg(CX) || !proc->regs.flags_reg.zero)
                    break;
                goto repeat;
            case REPNZ:
                if (!proc->regs.get_gpreg(CX) || proc->regs.flags_reg.zero)
                    break;
                goto repeat;
            default:
                break;
        }
    }
}

void cmps_m16_m16(class cpu_i386 *proc, x86_instr_t *instruction) {
    uint16_t m16_s, m16_d;

    repeat:
    m16_s = proc->read_mem<16>(select_segment(instruction), proc->regs.get_gpreg(SI));
    m16_d = proc->read_mem<16>(ES, proc->regs.get_gpreg(DI));
    proc->regs.flags_reg.update_reg_sub(m16_s, m16_d);

    proc->regs.update_gpreg(SI, proc->regs.flags_reg.direction ? -1 : 1);
    proc->regs.update_gpreg(DI, proc->regs.flags_reg.direction ? -1 : 1);

    if (instruction->repetition_pre) {
        proc->regs.update_gpreg(CX, -1);
        switch (instruction->repetition_pre) {
            case REPZ:
                if (!proc->regs.get_gpreg(CX) || !proc->regs.flags_reg.zero)
                    break;
                goto repeat;
            case REPNZ:
                if (!proc->regs.get_gpreg(CX) || proc->regs.flags_reg.zero)
                    break;
                goto repeat;
            default:
                break;
        }
    }
}

void test_ax_imm16(class cpu_i386 *proc, x86_instr_t *instruction) {
    uint16_t ax = proc->regs.get_gpreg(AX);
    proc->regs.flags_reg.update_reg_add(ax, instruction->imm.imm16);
}

void lodsb(class cpu_i386 *proc, x86_instr_t *instruction) {

    repeat:
    uint8_t byte = proc->read_mem<8>(DS, proc->regs.get_gpreg(SI));
    proc->regs.set_gpreg(AL, byte);

    proc->regs.update_gpreg(SI, proc->regs.flags_reg.direction ? -1 : 1);

    if (instruction->repetition_pre) {
        cout << "cx: " << proc->regs.get_gpreg(CX) << "\n";
        proc->regs.update_gpreg(CX, -1);
        switch (instruction->repetition_pre) {
            case REPZ:
                if (!proc->regs.get_gpreg(CX) || !proc->regs.flags_reg.zero)
                    break;
                goto repeat;
            case REPNZ:
                if (!proc->regs.get_gpreg(CX) || proc->regs.flags_reg.zero)
                    break;
                goto repeat;
            default:
                break;
        }
    }
}

void mov_r16_imm16(class cpu_i386 *proc, x86_instr_t *instruction) {
    uint8_t reg;

    reg = instruction->opcode & ((1 << 3) - 1);
    proc->regs.set_gpreg(static_cast<general_regs_t>(AX + reg), instruction->imm.imm16);
}

void ret_16(class cpu_i386 *proc, x86_instr_t *instruction) {
    proc->regs.ip_reg.ip = pop16(proc);
}

void mov_rm16_imm16(class cpu_i386 *proc, x86_instr_t *instruction) {
    set_rm16(proc, instruction, instruction->imm.imm16);
}

void leave_16(class cpu_i386 *proc, x86_instr_t *instruction) {
    uint16_t ebp;

    ebp = proc->regs.get_gpreg(EBP);
    proc->regs.set_gpreg(ESP, ebp);
    proc->regs.set_gpreg(EBP, pop16(proc));
}

void in_ax_imm8(class cpu_i386 *proc, x86_instr_t *instruction) {
    proc->regs.set_gpreg(AX, proc->io.pmio_manager.in_io<16>(instruction->imm.imm8));
}

void out_imm8_ax(class cpu_i386 *proc, x86_instr_t *instruction) {
    uint16_t ax;

    ax = proc->regs.get_gpreg(AX);
    proc->io.pmio_manager.out_io<16>(instruction->imm.imm8, ax);
}

void call_rel16(class cpu_i386 *proc, x86_instr_t *instruction) {
    push16(proc, proc->regs.ip_reg.ip);
    proc->regs.ip_reg.ip += (instruction->imm.imm16);
}

void jmp_rel16(class cpu_i386 *proc, x86_instr_t *instruction) {
    proc->regs.ip_reg.ip += (instruction->imm.imm16);
}

void jmpf_ptr16_16(class cpu_i386 *proc, x86_instr_t *instruction) {
    jmpf(proc, instruction->ptr16, instruction->imm.imm16);
}

void in_ax_dx(class cpu_i386 *proc, x86_instr_t *instruction) {
    uint16_t dx;

    dx = proc->regs.get_gpreg(DX);
    proc->regs.set_gpreg(AX, proc->io.pmio_manager.in_io<16>(dx));
}

void out_dx_ax(class cpu_i386 *proc, x86_instr_t *instruction) {
    uint16_t dx, ax;

    dx = proc->regs.get_gpreg(DX);
    ax = proc->regs.get_gpreg(AX);
    proc->io.pmio_manager.out_io<16>(dx, ax);
}

#define JCC_REL16(cc, is_flag) \
void j ## cc ## _rel16(class cpu_i386 *proc, x86_instr_t *instruction){ \
    if(is_flag) \
        proc->regs.ip_reg.eip += (instruction->imm.imm16); \
}

JCC_REL16(o, proc->regs.flags_reg.overflow)

JCC_REL16(no, !proc->regs.flags_reg.overflow)

JCC_REL16(b, proc->regs.flags_reg.carry)

JCC_REL16(nb, !proc->regs.flags_reg.carry)

JCC_REL16(z, proc->regs.flags_reg.zero)

JCC_REL16(nz, !proc->regs.flags_reg.zero)

JCC_REL16(be, proc->regs.flags_reg.carry || proc->regs.flags_reg.zero)

JCC_REL16(a, !(proc->regs.flags_reg.carry || proc->regs.flags_reg.zero))

JCC_REL16(s, proc->regs.flags_reg.sign)

JCC_REL16(ns, !proc->regs.flags_reg.sign)

JCC_REL16(p, proc->regs.flags_reg.parity)

JCC_REL16(np, !proc->regs.flags_reg.parity)

JCC_REL16(l, proc->regs.flags_reg.sign != proc->regs.flags_reg.overflow)

JCC_REL16(nl, proc->regs.flags_reg.sign == proc->regs.flags_reg.overflow)

JCC_REL16(le, proc->regs.flags_reg.zero ||
              (proc->regs.flags_reg.sign != proc->regs.flags_reg.overflow))

JCC_REL16(nle, !proc->regs.flags_reg.zero &&
               (proc->regs.flags_reg.sign == proc->regs.flags_reg.overflow))

void imul_r16_rm16(class cpu_i386 *proc, x86_instr_t *instruction) {
    int16_t r16_s, rm16_s;

    r16_s = get_r16(proc, instruction);
    rm16_s = get_rm16(proc, instruction);
    set_r16(proc, instruction, r16_s * rm16_s);
    proc->regs.flags_reg.update_reg_imul(r16_s, rm16_s);
}

void movzx_r16_rm8(class cpu_i386 *proc, x86_instr_t *instruction) {
    uint8_t rm8;

    rm8 = get_rm8(proc, instruction);
    set_r16(proc, instruction, rm8);
}

void movzx_r16_rm16(class cpu_i386 *proc, x86_instr_t *instruction) {
    uint16_t rm16;

    rm16 = get_rm16(proc, instruction);
    set_r16(proc, instruction, rm16);
}

void movsx_r16_rm8(class cpu_i386 *proc, x86_instr_t *instruction) {
    int8_t rm8_s;

    rm8_s = get_rm8(proc, instruction);
    set_r16(proc, instruction, rm8_s);
}

void movsx_r16_rm16(class cpu_i386 *proc, x86_instr_t *instruction) {
    int16_t rm16_s;

    rm16_s = get_rm16(proc, instruction);
    set_r16(proc, instruction, rm16_s);
}


void code_81_16(class cpu_i386 *proc, x86_instr_t *instruction) {
    switch (instruction->modrm.reg) {
        case 0:
            add_rm16_imm16(proc, instruction);
            break;
        case 1:
            or_rm16_imm16(proc, instruction);
            break;
        case 2:
            adc_rm16_imm16(proc, instruction);
            break;
        case 3:
            sbb_rm16_imm16(proc, instruction);
            break;
        case 4:
            and_rm16_imm16(proc, instruction);
            break;
        case 5:
            sub_rm16_imm16(proc, instruction);
            break;
        case 6:
            xor_rm16_imm16(proc, instruction);
            break;
        case 7:
            cmp_rm16_imm16(proc, instruction);
            break;
        default:
            DEBUG_ERROR("not implemented: 0x81 /%d\n", instruction->modrm.reg);
    }
}

void code_83_16(class cpu_i386 *proc, x86_instr_t *instruction) {
    switch (instruction->modrm.reg) {
        case 0:
            add_rm16_imm8(proc, instruction);
            break;
        case 1:
            or_rm16_imm8(proc, instruction);
            break;
        case 2:
            adc_rm16_imm8(proc, instruction);
            break;
        case 3:
            sbb_rm16_imm8(proc, instruction);
            break;
        case 4:
            and_rm16_imm8(proc, instruction);
            break;
        case 5:
            sub_rm16_imm8(proc, instruction);
            break;
        case 6:
            xor_rm16_imm8(proc, instruction);
            break;
        case 7:
            cmp_rm16_imm8(proc, instruction);
            break;
        default:
            DEBUG_ERROR("not implemented: 0x83 /%d\n", instruction->modrm.reg);
    }
}

void code_c1_16(class cpu_i386 *proc, x86_instr_t *instruction) {
    switch (instruction->modrm.reg) {
        case 4:
            shl_rm16_imm8(proc, instruction);
            break;
        case 5:
            shr_rm16_imm8(proc, instruction);
            break;
        case 6:
            sal_rm16_imm8(proc, instruction);
            break;    // ?
        case 7:
            sar_rm16_imm8(proc, instruction);
            break;
        default:
            DEBUG_ERROR("not implemented: 0xc1 /%d\n", instruction->modrm.reg);
    }
}

void code_d3_16(class cpu_i386 *proc, x86_instr_t *instruction) {
    switch (instruction->modrm.reg) {
        case 4:
            shl_rm16_cl(proc, instruction);
            break;
        case 5:
            shr_rm16_cl(proc, instruction);
            break;
        case 6:
            sal_rm16_cl(proc, instruction);
            break;    // ?
        case 7:
            sar_rm16_cl(proc, instruction);
            break;
        default:
            DEBUG_ERROR("not implemented: 0xd3 /%d\n", instruction->modrm.reg);
    }
}

void code_f7_16(class cpu_i386 *proc, x86_instr_t *instruction) {
    switch (instruction->modrm.reg) {
        case 0:
            test_rm16_imm16(proc, instruction);
            break;
        case 2:
            not_rm16(proc, instruction);
            break;
        case 3:
            neg_rm16(proc, instruction);
            break;
        case 4:
            mul_dx_ax_rm16(proc, instruction);
            break;
        case 5:
            imul_dx_ax_rm16(proc, instruction);
            break;
        case 6:
            div_dx_ax_rm16(proc, instruction);
            break;
        case 7:
            idiv_dx_ax_rm16(proc, instruction);
            break;
        default:
            DEBUG_ERROR("not implemented: 0xf7 /%d\n", instruction->modrm.reg);
    }
}

void code_ff_16(class cpu_i386 *proc, x86_instr_t *instruction) {
    switch (instruction->modrm.reg) {
        case 0:
            inc_rm16(proc, instruction);
            break;
        case 1:
            dec_rm16(proc, instruction);
            break;
        case 2:
            call_rm16(proc, instruction);
            break;
        case 3:
            callf_m16_16(proc, instruction);
            break;
        case 4:
            jmp_rm16(proc, instruction);
            break;
        case 5:
            jmpf_m16_16(proc, instruction);
            break;
        case 6:
            push_rm16(proc, instruction);
            break;
        default:
            DEBUG_ERROR("not implemented: 0xff /%d\n", instruction->modrm.reg);
    }
}

void code_0f00_16(class cpu_i386 *proc, x86_instr_t *instruction) {
    switch (instruction->modrm.reg) {
        case 3:
            ltr_rm16(proc, instruction);
            break;
        default:
            DEBUG_ERROR("not implemented: 0x0f00 /%d\n", instruction->modrm.reg);
    }
}

void code_0f01_16(class cpu_i386 *proc, x86_instr_t *instruction) {
    switch (instruction->modrm.reg) {
        case 2:
            lgdt_m24(proc, instruction);
            break;
        case 3:
            lidt_m24(proc, instruction);
            break;
        default:
            DEBUG_ERROR("not implemented: 0x0f01 /%d\n", instruction->modrm.reg);
    }
}


void add_rm16_imm16(class cpu_i386 *proc, x86_instr_t *instruction) {
    uint16_t rm16;

    rm16 = get_rm16(proc, instruction);
    set_rm16(proc, instruction, rm16 + instruction->imm.imm16);
    proc->regs.flags_reg.update_reg_add(rm16, instruction->imm.imm16);
}

void or_rm16_imm16(class cpu_i386 *proc, x86_instr_t *instruction) {
    uint16_t rm16 = get_rm16(proc, instruction);
    set_rm16(proc, instruction, rm16 | instruction->imm.imm16);
    proc->regs.flags_reg.update_reg_or(rm16, instruction->imm.imm16);
}

void adc_rm16_imm16(class cpu_i386 *proc, x86_instr_t *instruction) {
    uint16_t rm16 = get_rm16(proc, instruction);
    uint8_t cf = proc->regs.flags_reg.carry;
    set_rm16(proc, instruction, rm16 + instruction->imm.imm16 + cf);
    proc->regs.flags_reg.update_reg_add(rm16, instruction->imm.imm16 + cf);
}

void sbb_rm16_imm16(class cpu_i386 *proc, x86_instr_t *instruction) {
    uint16_t rm16 = get_rm16(proc, instruction);
    uint8_t cf = proc->regs.flags_reg.carry;
    set_rm16(proc, instruction, rm16 - instruction->imm.imm16 - cf);
    proc->regs.flags_reg.update_reg_sub(rm16, instruction->imm.imm16 + cf);
}

void and_rm16_imm16(class cpu_i386 *proc, x86_instr_t *instruction) {
    uint16_t rm16 = get_rm16(proc, instruction);
    set_rm16(proc, instruction, rm16 & instruction->imm.imm16);

    proc->regs.flags_reg.update_reg_add(rm16, instruction->imm.imm16);
}

void sub_rm16_imm16(class cpu_i386 *proc, x86_instr_t *instruction) {
    uint16_t rm16;

    rm16 = get_rm16(proc, instruction);
    set_rm16(proc, instruction, rm16 - instruction->imm.imm16);
    proc->regs.flags_reg.update_reg_sub(rm16, instruction->imm.imm16);
}

void xor_rm16_imm16(class cpu_i386 *proc, x86_instr_t *instruction) {
    uint16_t rm16;

    rm16 = get_rm16(proc, instruction);
    set_rm16(proc, instruction, rm16 ^ instruction->imm.imm16);
}

void cmp_rm16_imm16(class cpu_i386 *proc, x86_instr_t *instruction) {
    uint16_t rm16;

    rm16 = get_rm16(proc, instruction);
    proc->regs.flags_reg.update_reg_sub(rm16, instruction->imm.imm16);
}


void add_rm16_imm8(class cpu_i386 *proc, x86_instr_t *instruction) {
    uint16_t rm16 = get_rm16(proc, instruction);
    set_rm16(proc, instruction, rm16 + instruction->imm.imm8);
    proc->regs.flags_reg.update_reg_add(rm16, instruction->imm.imm8);
}

void or_rm16_imm8(class cpu_i386 *proc, x86_instr_t *instruction) {
    uint16_t rm16 = get_rm16(proc, instruction);
    set_rm16(proc, instruction, rm16 | instruction->imm.imm8);
    proc->regs.flags_reg.update_reg_or(rm16, instruction->imm.imm8);
}

void adc_rm16_imm8(class cpu_i386 *proc, x86_instr_t *instruction) {
    uint16_t rm16;
    uint8_t cf;

    rm16 = get_rm16(proc, instruction);
    cf = proc->regs.flags_reg.carry;
    set_rm16(proc, instruction, rm16 + instruction->imm.imm8 + cf);
    proc->regs.flags_reg.update_reg_add(rm16, instruction->imm.imm8 + cf);
}

void sbb_rm16_imm8(class cpu_i386 *proc, x86_instr_t *instruction) {
    uint16_t rm16;
    uint8_t cf;

    rm16 = get_rm16(proc, instruction);
    cf = proc->regs.flags_reg.carry;
    set_rm16(proc, instruction, rm16 - instruction->imm.imm8 - cf);
    proc->regs.flags_reg.update_reg_sub(rm16, instruction->imm.imm8 + cf);
}

void and_rm16_imm8(class cpu_i386 *proc, x86_instr_t *instruction) {
    uint16_t rm16;

    rm16 = get_rm16(proc, instruction);
    set_rm16(proc, instruction, rm16 & instruction->imm.imm8);
    proc->regs.flags_reg.update_reg_add(rm16, instruction->imm.imm8);
}

void sub_rm16_imm8(class cpu_i386 *proc, x86_instr_t *instruction) {
    uint16_t rm16;

    rm16 = get_rm16(proc, instruction);
    set_rm16(proc, instruction, rm16 - instruction->imm.imm8);
    proc->regs.flags_reg.update_reg_sub(rm16, instruction->imm.imm8);
}

void xor_rm16_imm8(class cpu_i386 *proc, x86_instr_t *instruction) {
    uint16_t rm16;

    rm16 = get_rm16(proc, instruction);
    set_rm16(proc, instruction, rm16 ^ instruction->imm.imm8);
}

void cmp_rm16_imm8(class cpu_i386 *proc, x86_instr_t *instruction) {
    uint16_t rm16;

    rm16 = get_rm16(proc, instruction);
    proc->regs.flags_reg.update_reg_sub(rm16, instruction->imm.imm8);
}


void shl_rm16_imm8(class cpu_i386 *proc, x86_instr_t *instruction) {
    uint16_t rm16;

    rm16 = get_rm16(proc, instruction);
    set_rm16(proc, instruction, rm16 << instruction->imm.imm8);
    proc->regs.flags_reg.update_reg_shl(rm16, instruction->imm.imm8);
}

void shr_rm16_imm8(class cpu_i386 *proc, x86_instr_t *instruction) {
    uint16_t rm16;

    rm16 = get_rm16(proc, instruction);
    set_rm16(proc, instruction, rm16 >> instruction->imm.imm8);
    proc->regs.flags_reg.update_reg_shr(rm16, instruction->imm.imm8);
}

void sal_rm16_imm8(class cpu_i386 *proc, x86_instr_t *instruction) {
    int16_t rm16_s;

    rm16_s = get_rm16(proc, instruction);
    set_rm16(proc, instruction, rm16_s << instruction->imm.imm8);
//	EFLAGS_UPDATE_SAL(rm16_s, instr->imm.imm8);
}

void sar_rm16_imm8(class cpu_i386 *proc, x86_instr_t *instruction) {
    int16_t rm16_s;

    rm16_s = get_rm16(proc, instruction);
    set_rm16(proc, instruction, rm16_s >> instruction->imm.imm8);
//	EFLAGS_UPDATE_SAR(rm16_s, instr->imm.imm8);
}


void shl_rm16_cl(class cpu_i386 *proc, x86_instr_t *instruction) {
    uint16_t rm16;
    uint8_t cl;

    rm16 = get_rm16(proc, instruction);
    cl = proc->regs.get_gpreg(CL);
    set_rm16(proc, instruction, rm16 << cl);
    proc->regs.flags_reg.update_reg_shl(rm16, cl);
}

void shr_rm16_cl(class cpu_i386 *proc, x86_instr_t *instruction) {
    uint16_t rm16;
    uint8_t cl;

    rm16 = get_rm16(proc, instruction);
    cl = proc->regs.get_gpreg(CL);
    set_rm16(proc, instruction, rm16 >> cl);
    proc->regs.flags_reg.update_reg_shr(rm16, cl);
}

void sal_rm16_cl(class cpu_i386 *proc, x86_instr_t *instruction) {
    int16_t rm16_s;
    uint8_t cl;

    rm16_s = get_rm16(proc, instruction);
    cl = proc->regs.get_gpreg(CL);
    set_rm16(proc, instruction, rm16_s << cl);
//	EFLAGS_UPDATE_SAL(rm16_s, cl);
}

void sar_rm16_cl(class cpu_i386 *proc, x86_instr_t *instruction) {
    int16_t rm16_s;
    uint8_t cl;

    rm16_s = get_rm16(proc, instruction);
    cl = proc->regs.get_gpreg(CL);
    set_rm16(proc, instruction, rm16_s >> cl);
//	EFLAGS_UPDATE_SAR(rm16_s, cl);
}


void test_rm16_imm16(class cpu_i386 *proc, x86_instr_t *instruction) {
    uint16_t rm16, imm16;

    rm16 = get_rm16(proc, instruction);
    imm16 = proc->read_code<16>();
    proc->regs.ip_reg.eip += (2);
    proc->regs.flags_reg.update_reg_add(rm16, imm16);
}

void not_rm16(class cpu_i386 *proc, x86_instr_t *instruction) {
    uint16_t rm16;

    rm16 = get_rm16(proc, instruction);
    set_rm16(proc, instruction, ~rm16);
}

void neg_rm16(class cpu_i386 *proc, x86_instr_t *instruction) {
    int16_t rm16_s;

    rm16_s = get_rm16(proc, instruction);
    set_rm16(proc, instruction, -rm16_s);
    proc->regs.flags_reg.update_reg_sub((uint16_t) 0, rm16_s);
}

void mul_dx_ax_rm16(class cpu_i386 *proc, x86_instr_t *instruction) {
    uint16_t rm16, ax;
    uint32_t val;

    rm16 = get_rm16(proc, instruction);
    ax = proc->regs.get_gpreg(AX);
    val = ax * rm16;

    proc->regs.set_gpreg(AX, val & ((1 << 16) - 1));
    proc->regs.set_gpreg(DX, (val >> 16) & ((1 << 16) - 1));

    proc->regs.flags_reg.update_reg_mul(ax, rm16);
}

void imul_dx_ax_rm16(class cpu_i386 *proc, x86_instr_t *instruction) {
    int16_t rm16_s, ax_s;
    int32_t val_s;

    rm16_s = get_rm16(proc, instruction);
    ax_s = proc->regs.get_gpreg(AX);
    val_s = ax_s * rm16_s;

    proc->regs.set_gpreg(AX, val_s & ((1 << 16) - 1));
    proc->regs.set_gpreg(DX, (val_s >> 16) & ((1 << 16) - 1));

    proc->regs.flags_reg.update_reg_imul(ax_s, rm16_s);
}

void div_dx_ax_rm16(class cpu_i386 *proc, x86_instr_t *instruction) {
    uint16_t rm16;
    uint32_t val;

    rm16 = get_rm16(proc, instruction);
    CPU_EXCEPTION(EXP_DE, !rm16);
    val = (proc->regs.get_gpreg(DX) << 16) | proc->regs.get_gpreg(AX);

    proc->regs.set_gpreg(AX, val / rm16);
    proc->regs.set_gpreg(DX, val % rm16);
}

void idiv_dx_ax_rm16(class cpu_i386 *proc, x86_instr_t *instruction) {
    int16_t rm16_s;
    int32_t val_s;

    rm16_s = get_rm16(proc, instruction);
    CPU_EXCEPTION(EXP_DE, !rm16_s);
    val_s = (proc->regs.get_gpreg(DX) << 16) | proc->regs.get_gpreg(AX);

    proc->regs.set_gpreg(AX, val_s / rm16_s);
    proc->regs.set_gpreg(DX, val_s % rm16_s);
}


void inc_rm16(class cpu_i386 *proc, x86_instr_t *instruction) {
    uint16_t rm16;

    rm16 = get_rm16(proc, instruction);
    set_rm16(proc, instruction, rm16 + 1);
    proc->regs.flags_reg.update_reg_add(rm16, 1);
}

void dec_rm16(class cpu_i386 *proc, x86_instr_t *instruction) {
    uint16_t rm16;

    rm16 = get_rm16(proc, instruction);
    set_rm16(proc, instruction, rm16 - 1);
    proc->regs.flags_reg.update_reg_sub(rm16, 1);
}

void call_rm16(class cpu_i386 *proc, x86_instr_t *instruction) {
    uint16_t rm16 = get_rm16(proc, instruction);

    push16(proc, proc->regs.ip_reg.ip);
    proc->regs.ip_reg.ip = rm16;
}

void callf_m16_16(class cpu_i386 *proc, x86_instr_t *instruction) {
    uint16_t m32, cs, ip;

    m32 = get_m(proc, instruction);

    ip = proc->memory.read_mem<16>(m32);
    cs = proc->memory.read_mem<16>(m32 + 2);

    callf(proc, cs, ip);
}

void jmp_rm16(class cpu_i386 *proc, x86_instr_t *instruction) {
    uint16_t rm16;

    rm16 = get_rm16(proc, instruction);
    proc->regs.ip_reg.ip = rm16;
}

void jmpf_m16_16(class cpu_i386 *proc, x86_instr_t *instruction) {
    uint16_t m32, sel, ip;

    m32 = get_m(proc, instruction);
    ip = proc->memory.read_mem<16>(m32);
    sel = proc->memory.read_mem<16>(m32 + 2);

    jmpf(proc, sel, ip);
}

void push_rm16(class cpu_i386 *proc, x86_instr_t *instruction) {
    uint16_t rm16;

    rm16 = get_rm16(proc, instruction);
    push16(proc, rm16);
}


void lgdt_m24(class cpu_i386 *proc, x86_instr_t *instruction) {
    uint16_t m48, base, limit;

    CPU_EXCEPTION(EXP_GP, !chk_ring(proc, 0));

    m48 = get_m(proc, instruction);
    limit = proc->memory.read_mem<16>(m48);
    base = proc->memory.read_mem<32>(m48 + 2) & ((1 << 24) - 1);

    proc->regs.set_dtreg(GDTR, 0, base, limit);
}

void lidt_m24(class cpu_i386 *proc, x86_instr_t *instruction) {
    uint16_t m48, base, limit;

    CPU_EXCEPTION(EXP_GP, !chk_ring(proc, 0));

    m48 = get_m(proc, instruction);
    limit = proc->memory.read_mem<16>(m48);
    base = proc->memory.read_mem<32>(m48 + 2) & ((1 << 24) - 1);

    proc->regs.set_dtreg(IDTR, 0, base, limit);
}

/******************************************************************/

void add_rm32_r32(cpu_i386 *proc, x86_instr_t *instr) {
    uint32_t rm32, r32;

    rm32 = get_rm32(proc, instr);
    r32 = get_r32(proc, instr);
    set_rm32(proc, instr, rm32 + r32);
    proc->regs.flags_reg.update_reg_add(rm32, r32);
}

void add_r32_rm32(cpu_i386 *proc, x86_instr_t *instr) {
    uint32_t r32, rm32;

    r32 = get_r32(proc, instr);
    rm32 = get_rm32(proc, instr);
    set_r32(proc, instr, r32 + rm32);
    proc->regs.flags_reg.update_reg_add(r32, rm32);
}

void add_eax_imm32(cpu_i386 *proc, x86_instr_t *instr) {
    uint32_t eax;

    eax = proc->regs.get_gpreg(EAX);
    proc->regs.set_gpreg(EAX, eax + instr->imm.imm32);
    proc->regs.flags_reg.update_reg_add(eax, instr->imm.imm32);
}

void push_es32(cpu_i386 *proc, x86_instr_t *instr) {
    push32(proc, proc->regs.get_sgreg(ES));
}

void pop_es32(cpu_i386 *proc, x86_instr_t *instr) {
    proc->set_segment(ES, pop32(proc));
}

void or_rm32_r32(cpu_i386 *proc, x86_instr_t *instr) {
    uint32_t rm32, r32;

    rm32 = get_rm32(proc, instr);
    r32 = get_r32(proc, instr);
    set_rm32(proc, instr, rm32 | r32);
    proc->regs.flags_reg.update_reg_or(rm32, r32);
}

void or_r32_rm32(cpu_i386 *proc, x86_instr_t *instr) {
    uint32_t r32, rm32;

    r32 = get_r32(proc, instr);
    rm32 = get_rm32(proc, instr);
    set_r32(proc, instr, r32 | rm32);
    proc->regs.flags_reg.update_reg_or(r32, rm32);
}

void or_eax_imm32(cpu_i386 *proc, x86_instr_t *instr) {
    uint32_t eax;

    eax = proc->regs.get_gpreg(EAX);
    proc->regs.set_gpreg(EAX, eax | instr->imm.imm32);
    proc->regs.flags_reg.update_reg_or(eax, instr->imm.imm32);
}

void push_ss32(cpu_i386 *proc, x86_instr_t *instr) {
    push32(proc, proc->regs.get_sgreg(SS));
}

void pop_ss32(cpu_i386 *proc, x86_instr_t *instr) {
    proc->set_segment(SS, pop32(proc));
}

void push_ds32(cpu_i386 *proc, x86_instr_t *instr) {
    push32(proc, proc->regs.get_sgreg(DS));
}

void pop_ds32(cpu_i386 *proc, x86_instr_t *instr) {
    proc->set_segment(DS, pop32(proc));
}

void and_rm32_r32(cpu_i386 *proc, x86_instr_t *instr) {
    uint32_t rm32, r32;

    rm32 = get_rm32(proc, instr);
    r32 = get_r32(proc, instr);
    set_rm32(proc, instr, rm32 & r32);
    proc->regs.flags_reg.update_reg_and(rm32, r32);
}

void and_r32_rm32(cpu_i386 *proc, x86_instr_t *instr) {
    uint32_t r32, rm32;

    r32 = get_r32(proc, instr);
    rm32 = get_rm32(proc, instr);
    set_r32(proc, instr, r32 & rm32);
    proc->regs.flags_reg.update_reg_and(r32, rm32);
}

void and_eax_imm32(cpu_i386 *proc, x86_instr_t *instr) {
    uint32_t eax;

    eax = proc->regs.get_gpreg(EAX);
    proc->regs.set_gpreg(EAX, eax & instr->imm.imm32);
    proc->regs.flags_reg.update_reg_and(eax, instr->imm.imm32);
}

void sub_rm32_r32(cpu_i386 *proc, x86_instr_t *instr) {
    uint32_t rm32, r32;

    rm32 = get_rm32(proc, instr);
    r32 = get_r32(proc, instr);
    set_rm32(proc, instr, rm32 - r32);
    proc->regs.flags_reg.update_reg_sub(rm32, r32);
}

void sub_r32_rm32(cpu_i386 *proc, x86_instr_t *instr) {
    uint32_t r32, rm32;

    r32 = get_r32(proc, instr);
    rm32 = get_rm32(proc, instr);
    set_r32(proc, instr, r32 - rm32);
    proc->regs.flags_reg.update_reg_sub(r32, rm32);
}

void sub_eax_imm32(cpu_i386 *proc, x86_instr_t *instr) {
    uint32_t eax;

    eax = proc->regs.get_gpreg(EAX);
    proc->regs.set_gpreg(EAX, eax - instr->imm.imm32);
    proc->regs.flags_reg.update_reg_sub(eax, instr->imm.imm32);
}

void xor_rm32_r32(cpu_i386 *proc, x86_instr_t *instr) {
    uint32_t rm32, r32;

    rm32 = get_rm32(proc, instr);
    r32 = get_r32(proc, instr);
    set_rm32(proc, instr, rm32 ^ r32);
}

void xor_r32_rm32(cpu_i386 *proc, x86_instr_t *instr) {
    uint32_t r32, rm32;

    r32 = get_r32(proc, instr);
    rm32 = get_rm32(proc, instr);
    set_r32(proc, instr, r32 ^ rm32);
}

void xor_eax_imm32(cpu_i386 *proc, x86_instr_t *instr) {
    uint32_t eax;

    eax = proc->regs.get_gpreg(EAX);
    proc->regs.set_gpreg(EAX, eax ^ instr->imm.imm32);
}

void cmp_rm32_r32(cpu_i386 *proc, x86_instr_t *instr) {
    uint32_t rm32, r32;

    rm32 = get_rm32(proc, instr);
    r32 = get_r32(proc, instr);
    proc->regs.flags_reg.update_reg_sub(rm32, r32);
}

void cmp_r32_rm32(cpu_i386 *proc, x86_instr_t *instr) {
    uint32_t r32, rm32;

    r32 = get_r32(proc, instr);
    rm32 = get_rm32(proc, instr);
    proc->regs.flags_reg.update_reg_sub(r32, rm32);
}

void cmp_eax_imm32(cpu_i386 *proc, x86_instr_t *instr) {
    uint32_t eax;

    eax = proc->regs.get_gpreg(EAX);
    proc->regs.flags_reg.update_reg_sub(eax, instr->imm.imm32);
}

void inc_r32(cpu_i386 *proc, x86_instr_t *instr) {
    uint8_t reg;
    uint32_t r32;

    reg = instr->opcode & ((1 << 3) - 1);
    r32 = proc->regs.get_gpreg(static_cast<general_regs_t>(EAX + reg));
    proc->regs.set_gpreg(static_cast<general_regs_t>(EAX + reg), r32 + 1);
    proc->regs.flags_reg.update_reg_add(r32, 1);
}

void dec_r32(cpu_i386 *proc, x86_instr_t *instr) {
    uint8_t reg;
    uint32_t r32;

    reg = instr->opcode & ((1 << 3) - 1);
    r32 = proc->regs.get_gpreg(static_cast<general_regs_t>(EAX + reg));
    proc->regs.set_gpreg(static_cast<general_regs_t>(EAX + reg), r32 - 1);
    proc->regs.flags_reg.update_reg_sub(r32, 1);
}

void push_r32(cpu_i386 *proc, x86_instr_t *instr) {
    uint8_t reg;

    reg = instr->opcode & ((1 << 3) - 1);
    push32(proc, proc->regs.get_gpreg(static_cast<general_regs_t>(EAX + reg)));
}

void pop_r32(cpu_i386 *proc, x86_instr_t *instr) {
    uint8_t reg;

    reg = instr->opcode & ((1 << 3) - 1);
    proc->regs.set_gpreg(static_cast<general_regs_t>(EAX + reg), pop32(proc));
}

void pushad(cpu_i386 *proc, x86_instr_t *instr) {
    uint32_t esp;

    esp = proc->regs.get_gpreg(ESP);

    push32(proc, proc->regs.get_gpreg(EAX));
    push32(proc, proc->regs.get_gpreg(ECX));
    push32(proc, proc->regs.get_gpreg(EDX));
    push32(proc, proc->regs.get_gpreg(EBX));
    push32(proc, esp);
    push32(proc, proc->regs.get_gpreg(EBP));
    push32(proc, proc->regs.get_gpreg(ESI));
    push32(proc, proc->regs.get_gpreg(EDI));
}

void popad(cpu_i386 *proc, x86_instr_t *instr) {
    uint32_t esp;

    proc->regs.set_gpreg(EDI, pop32(proc));
    proc->regs.set_gpreg(ESI, pop32(proc));
    proc->regs.set_gpreg(EBP, pop32(proc));
    esp = pop32(proc);
    proc->regs.set_gpreg(EBX, pop32(proc));
    proc->regs.set_gpreg(EDX, pop32(proc));
    proc->regs.set_gpreg(ECX, pop32(proc));
    proc->regs.set_gpreg(EAX, pop32(proc));

    proc->regs.set_gpreg(ESP, esp);
}

void push_imm32_32(cpu_i386 *proc, x86_instr_t *instr) {
    push32(proc, instr->imm.imm32);
}

void imul_r32_rm32_imm32(cpu_i386 *proc, x86_instr_t *instr) {
    int32_t rm32_s;

    rm32_s = get_rm32(proc, instr);
    set_r32(proc, instr, rm32_s * instr->imm.imm32);
    proc->regs.flags_reg.update_reg_imul(rm32_s, instr->imm.imm32);
}

void push_imm8_32(cpu_i386 *proc, x86_instr_t *instr) {
    push32(proc, instr->imm.imm8);
}

void imul_r32_rm32_imm8(cpu_i386 *proc, x86_instr_t *instr) {
    int32_t rm32_s;

    rm32_s = get_rm32(proc, instr);
    set_r32(proc, instr, rm32_s * instr->imm.imm8);
    proc->regs.flags_reg.update_reg_imul(rm32_s, instr->imm.imm8);
}

void test_rm32_r32(cpu_i386 *proc, x86_instr_t *instr) {
    uint32_t rm32, r32;

    rm32 = get_rm32(proc, instr);
    r32 = get_r32(proc, instr);
    proc->regs.flags_reg.update_reg_and(rm32, r32);
}

void xchg_r32_rm32(cpu_i386 *proc, x86_instr_t *instr) {
    uint32_t r32, rm32;

    r32 = get_r32(proc, instr);
    rm32 = get_rm32(proc, instr);
    set_r32(proc, instr, rm32);
    set_rm32(proc, instr, r32);
}

void mov_rm32_r32(cpu_i386 *proc, x86_instr_t *instr) {
    uint32_t r32;

    r32 = get_r32(proc, instr);
    set_rm32(proc, instr, r32);
}

void mov_r32_rm32(cpu_i386 *proc, x86_instr_t *instr) {
    uint32_t rm32;

    rm32 = get_rm32(proc, instr);
    set_r32(proc, instr, rm32);
}

void mov_rm32_sreg(cpu_i386 *proc, x86_instr_t *instr) {
    uint16_t sreg;

    sreg = get_sreg(proc, instr);
    set_rm32(proc, instr, sreg);
}

void lea_r32_m32(cpu_i386 *proc, x86_instr_t *instr) {
    uint32_t m32;

    m32 = get_m(proc, instr);
    set_r32(proc, instr, m32);
}

void xchg_r32_eax(cpu_i386 *proc, x86_instr_t *instr) {
    uint32_t r32, eax;

    r32 = get_r32(proc, instr);
    eax = proc->regs.get_gpreg(EAX);
    set_r32(proc, instr, eax);
    proc->regs.set_gpreg(EAX, r32);
}

void cwde(cpu_i386 *proc, x86_instr_t *instr) {
    int16_t ax_s;

    ax_s = proc->regs.get_gpreg(AX);
    proc->regs.set_gpreg(EAX, ax_s);
}

void cdq(cpu_i386 *proc, x86_instr_t *instr) {
    uint32_t eax;

    eax = proc->regs.get_gpreg(EAX);
    proc->regs.set_gpreg(EDX, eax & (1 << 31) ? -1 : 0);
}

void callf_ptr16_32(cpu_i386 *proc, x86_instr_t *instr) {
    callf(proc, instr->ptr16, instr->imm.imm32);
}

void pushf_32(cpu_i386 *proc, x86_instr_t *instr) {
    push32(proc, proc->regs.flags_reg.eflags);
}

void popf_32(cpu_i386 *proc, x86_instr_t *instr) {
    proc->regs.flags_reg.eflags = (pop32(proc));
}

void mov_eax_moffs32(cpu_i386 *proc, x86_instr_t *instr) {
    proc->regs.set_gpreg(EAX, get_moffs32(proc, instr));
}

void mov_moffs32_eax(cpu_i386 *proc, x86_instr_t *instr) {
    set_moffs32(proc, instr, proc->regs.get_gpreg(EAX));
}

void cmps_m8_m8_32(cpu_i386 *proc, x86_instr_t *instr) {
    uint8_t m8_s, m8_d;

    repeat:
    m8_s = proc->read_mem<8>(select_segment(instr), proc->regs.get_gpreg(ESI));
    m8_d = proc->read_mem<8>(ES, proc->regs.get_gpreg(EDI));
    proc->regs.flags_reg.update_reg_sub(m8_s, m8_d);


    proc->regs.update_gpreg(ESI, proc->regs.flags_reg.direction ? -1 : 1);
    proc->regs.update_gpreg(EDI, proc->regs.flags_reg.direction ? -1 : 1);

    if (instr->repetition_pre) {
        proc->regs.update_gpreg(ECX, -1);
        switch (instr->repetition_pre) {
            case REPZ:
                if (!proc->regs.get_gpreg(ECX) || !proc->regs.flags_reg.zero)
                    break;
                goto repeat;
            case REPNZ:
                if (!proc->regs.get_gpreg(ECX) || proc->regs.flags_reg.zero)
                    break;
                goto repeat;
            default:
                break;
        }
    }
}

void cmps_m32_m32(cpu_i386 *proc, x86_instr_t *instr) {
    uint32_t m32_s, m32_d;

    repeat:
    m32_s = proc->read_mem<32>(select_segment(instr), proc->regs.get_gpreg(ESI));
    m32_d = proc->read_mem<32>(ES, proc->regs.get_gpreg(EDI));
    proc->regs.flags_reg.update_reg_sub(m32_s, m32_d);

    proc->regs.update_gpreg(ESI, proc->regs.flags_reg.direction ? -1 : 1);
    proc->regs.update_gpreg(EDI, proc->regs.flags_reg.direction ? -1 : 1);

    if (instr->repetition_pre) {
        proc->regs.update_gpreg(ECX, -1);
        switch (instr->repetition_pre) {
            case REPZ:
                if (!proc->regs.get_gpreg(ECX) || !proc->regs.flags_reg.zero)
                    break;
                goto repeat;
            case REPNZ:
                if (!proc->regs.get_gpreg(ECX) || proc->regs.flags_reg.zero)
                    break;
                goto repeat;
            default:
                break;
        }
    }
}

void test_eax_imm32(cpu_i386 *proc, x86_instr_t *instr) {
    uint32_t eax;

    eax = proc->regs.get_gpreg(EAX);
    proc->regs.flags_reg.update_reg_and(eax, instr->imm.imm32);
}

void mov_r32_imm32(cpu_i386 *proc, x86_instr_t *instr) {
    uint8_t reg;

    reg = instr->opcode & ((1 << 3) - 1);
    proc->regs.set_gpreg(static_cast<general_regs_t>(EAX + reg), instr->imm.imm32);
}

void ret_32(cpu_i386 *proc, x86_instr_t *instr) {
    proc->regs.ip_reg.eip = (pop32(proc));
}

void mov_rm32_imm32(cpu_i386 *proc, x86_instr_t *instr) {
    set_rm32(proc, instr, instr->imm.imm32);
}

void leave_32(cpu_i386 *proc, x86_instr_t *instr) {
    uint32_t ebp;

    ebp = proc->regs.get_gpreg(EBP);
    proc->regs.set_gpreg(ESP, ebp);
    proc->regs.set_gpreg(EBP, pop32(proc));
}

void in_eax_imm8(cpu_i386 *proc, x86_instr_t *instr) {
    proc->regs.set_gpreg(EAX, proc->io.pmio_manager.in_io<32>(instr->imm.imm8));
}

void out_imm8_eax(cpu_i386 *proc, x86_instr_t *instr) {
    uint32_t eax;

    eax = proc->regs.get_gpreg(EAX);
    proc->io.pmio_manager.out_io<32>(instr->imm.imm8, eax);
}

void call_rel32(cpu_i386 *proc, x86_instr_t *instr) {
    push32(proc, proc->regs.ip_reg.eip);
    proc->regs.ip_reg.eip += (instr->imm.imm32);
}

void jmp_rel32(cpu_i386 *proc, x86_instr_t *instr) {
    proc->regs.ip_reg.eip += (instr->imm.imm32);
}

void jmpf_ptr16_32(cpu_i386 *proc, x86_instr_t *instr) {
    jmpf(proc, instr->ptr16, instr->imm.imm32);
}

void in_eax_dx(cpu_i386 *proc, x86_instr_t *instr) {
    uint16_t dx;

    dx = proc->regs.get_gpreg(DX);
    proc->regs.set_gpreg(EAX, proc->io.pmio_manager.in_io<32>(dx));
}

void out_dx_eax(cpu_i386 *proc, x86_instr_t *instr) {
    uint16_t dx;
    uint32_t eax;

    dx = proc->regs.get_gpreg(DX);
    eax = proc->regs.get_gpreg(EAX);
    proc->io.pmio_manager.out_io<32>(dx, eax);
}

#define JCC_REL32(cc, is_flag) \
void j ## cc ## _rel32(cpu_i386 *proc, x86_instr_t *instr){ \
    if(is_flag) \
        proc->regs.ip_reg.eip += (instr->imm.imm32); \
}

JCC_REL32(o, proc->regs.flags_reg.overflow)

JCC_REL32(no, !proc->regs.flags_reg.overflow)

JCC_REL32(b, proc->regs.flags_reg.carry)

JCC_REL32(nb, !proc->regs.flags_reg.carry)

JCC_REL32(z, proc->regs.flags_reg.zero)

JCC_REL32(nz, !proc->regs.flags_reg.zero)

JCC_REL32(be, proc->regs.flags_reg.carry || proc->regs.flags_reg.zero)

JCC_REL32(a, !(proc->regs.flags_reg.carry || proc->regs.flags_reg.zero))

JCC_REL32(s, proc->regs.flags_reg.sign)

JCC_REL32(ns, !proc->regs.flags_reg.sign)

JCC_REL32(p, proc->regs.flags_reg.parity)

JCC_REL32(np, !proc->regs.flags_reg.parity)

JCC_REL32(l, proc->regs.flags_reg.sign != proc->regs.flags_reg.overflow)

JCC_REL32(nl, proc->regs.flags_reg.sign == proc->regs.flags_reg.overflow)

JCC_REL32(le, proc->regs.flags_reg.zero ||
              (proc->regs.flags_reg.sign != proc->regs.flags_reg.overflow))

JCC_REL32(nle, !proc->regs.flags_reg.zero &&
               (proc->regs.flags_reg.sign == proc->regs.flags_reg.overflow))

void imul_r32_rm32(cpu_i386 *proc, x86_instr_t *instr) {
    int16_t r32_s, rm32_s;

    r32_s = get_r32(proc, instr);
    rm32_s = get_rm32(proc, instr);
    set_r32(proc, instr, r32_s * rm32_s);
    proc->regs.flags_reg.update_reg_imul(r32_s, rm32_s);
}

void movzx_r32_rm8(cpu_i386 *proc, x86_instr_t *instr) {
    uint8_t rm8;

    rm8 = get_rm8(proc, instr);
    set_r32(proc, instr, rm8);
}

void movzx_r32_rm16(cpu_i386 *proc, x86_instr_t *instr) {
    uint16_t rm16;

    rm16 = get_rm16(proc, instr);
    set_r32(proc, instr, rm16);
}

void movsx_r32_rm8(cpu_i386 *proc, x86_instr_t *instr) {
    int8_t rm8_s;

    rm8_s = get_rm8(proc, instr);
    set_r32(proc, instr, rm8_s);
}

void movsx_r32_rm16(cpu_i386 *proc, x86_instr_t *instr) {
    int16_t rm16_s;

    rm16_s = get_rm16(proc, instr);
    set_r32(proc, instr, rm16_s);
}


void code_81_32(cpu_i386 *proc, x86_instr_t *instr) {
    switch (instr->modrm.reg) {
        case 0:
            add_rm32_imm32(proc, instr);
            break;
        case 1:
            or_rm32_imm32(proc, instr);
            break;
        case 2:
            adc_rm32_imm32(proc, instr);
            break;
        case 3:
            sbb_rm32_imm32(proc, instr);
            break;
        case 4:
            and_rm32_imm32(proc, instr);
            break;
        case 5:
            sub_rm32_imm32(proc, instr);
            break;
        case 6:
            xor_rm32_imm32(proc, instr);
            break;
        case 7:
            cmp_rm32_imm32(proc, instr);
            break;
        default:
            DEBUG_ERROR("not implemented: 0x81 /%d\n", instr->modrm.reg);
    }
}

void code_83_32(cpu_i386 *proc, x86_instr_t *instr) {
    switch (instr->modrm.reg) {
        case 0:
            add_rm32_imm8(proc, instr);
            break;
        case 1:
            or_rm32_imm8(proc, instr);
            break;
        case 2:
            adc_rm32_imm8(proc, instr);
            break;
        case 3:
            sbb_rm32_imm8(proc, instr);
            break;
        case 4:
            and_rm32_imm8(proc, instr);
            break;
        case 5:
            sub_rm32_imm8(proc, instr);
            break;
        case 6:
            xor_rm32_imm8(proc, instr);
            break;
        case 7:
            cmp_rm32_imm8(proc, instr);
            break;
        default:
            DEBUG_ERROR("not implemented: 0x83 /%d\n", instr->modrm.reg);
    }
}

void code_c1_32(cpu_i386 *proc, x86_instr_t *instr) {
    switch (instr->modrm.reg) {
        case 4:
            shl_rm32_imm8(proc, instr);
            break;
        case 5:
            shr_rm32_imm8(proc, instr);
            break;
        case 6:
            sal_rm32_imm8(proc, instr);
            break;    // ?
        case 7:
            sar_rm32_imm8(proc, instr);
            break;
        default:
            DEBUG_ERROR("not implemented: 0xc1 /%d\n", instr->modrm.reg);
    }
}

void code_d3_32(cpu_i386 *proc, x86_instr_t *instr) {
    switch (instr->modrm.reg) {
        case 4:
            shl_rm32_cl(proc, instr);
            break;
        case 5:
            shr_rm32_cl(proc, instr);
            break;
        case 6:
            sal_rm32_cl(proc, instr);
            break;    // ?
        case 7:
            sar_rm32_cl(proc, instr);
            break;
        default:
            DEBUG_ERROR("not implemented: 0xd3 /%d\n", instr->modrm.reg);
    }
}

void code_f7_32(cpu_i386 *proc, x86_instr_t *instr) {
    switch (instr->modrm.reg) {
        case 0:
            test_rm32_imm32(proc, instr);
            break;
        case 2:
            not_rm32(proc, instr);
            break;
        case 3:
            neg_rm32(proc, instr);
            break;
        case 4:
            mul_edx_eax_rm32(proc, instr);
            break;
        case 5:
            imul_edx_eax_rm32(proc, instr);
            break;
        case 6:
            div_edx_eax_rm32(proc, instr);
            break;
        case 7:
            idiv_edx_eax_rm32(proc, instr);
            break;
        default:
            DEBUG_ERROR("not implemented: 0xf7 /%d\n", instr->modrm.reg);
    }
}

void code_ff_32(cpu_i386 *proc, x86_instr_t *instr) {
    switch (instr->modrm.reg) {
        case 0:
            inc_rm32(proc, instr);
            break;
        case 1:
            dec_rm32(proc, instr);
            break;
        case 2:
            call_rm32(proc, instr);
            break;
        case 3:
            callf_m16_32(proc, instr);
            break;
        case 4:
            jmp_rm32(proc, instr);
            break;
        case 5:
            jmpf_m16_32(proc, instr);
            break;
        case 6:
            push_rm32(proc, instr);
            break;
        default:
            DEBUG_ERROR("not implemented: 0xff /%d\n", instr->modrm.reg);
    }
}

void code_0f00_32(cpu_i386 *proc, x86_instr_t *instr) {
    switch (instr->modrm.reg) {
        case 3:
            ltr_rm16(proc, instr);
            break;
        default:
            DEBUG_ERROR("not implemented: 0x0f00 /%d\n", instr->modrm.reg);
    }
}

void code_0f01_32(cpu_i386 *proc, x86_instr_t *instr) {
    switch (instr->modrm.reg) {
        case 2:
            lgdt_m32(proc, instr);
            break;
        case 3:
            lidt_m32(proc, instr);
            break;
        default:
            DEBUG_ERROR("not implemented: 0x0f01 /%d\n", instr->modrm.reg);
    }
}


void add_rm32_imm32(cpu_i386 *proc, x86_instr_t *instr) {
    uint32_t rm32;

    rm32 = get_rm32(proc, instr);
    set_rm32(proc, instr, rm32 + instr->imm.imm32);
    proc->regs.flags_reg.update_reg_add(rm32, instr->imm.imm32);
}

void or_rm32_imm32(cpu_i386 *proc, x86_instr_t *instr) {
    uint32_t rm32;

    rm32 = get_rm32(proc, instr);
    set_rm32(proc, instr, rm32 | instr->imm.imm32);
    proc->regs.flags_reg.update_reg_or(rm32, instr->imm.imm32);
}

void adc_rm32_imm32(cpu_i386 *proc, x86_instr_t *instr) {
    uint32_t rm32;
    uint8_t cf;

    rm32 = get_rm32(proc, instr);
    cf = proc->regs.flags_reg.carry;
    set_rm32(proc, instr, rm32 + instr->imm.imm32 + cf);
    proc->regs.flags_reg.update_reg_add(rm32, instr->imm.imm32 + cf);
}

void sbb_rm32_imm32(cpu_i386 *proc, x86_instr_t *instr) {
    uint32_t rm32;
    uint8_t cf;

    rm32 = get_rm32(proc, instr);
    cf = proc->regs.flags_reg.carry;
    set_rm32(proc, instr, rm32 - instr->imm.imm32 - cf);
    proc->regs.flags_reg.update_reg_sub(rm32, instr->imm.imm32 + cf);
}

void and_rm32_imm32(cpu_i386 *proc, x86_instr_t *instr) {
    uint32_t rm32;

    rm32 = get_rm32(proc, instr);
    set_rm32(proc, instr, rm32 & instr->imm.imm32);
    proc->regs.flags_reg.update_reg_and(rm32, instr->imm.imm32);
}

void sub_rm32_imm32(cpu_i386 *proc, x86_instr_t *instr) {
    uint32_t rm32;

    rm32 = get_rm32(proc, instr);
    set_rm32(proc, instr, rm32 - instr->imm.imm32);
    proc->regs.flags_reg.update_reg_sub(rm32, instr->imm.imm32);
}

void xor_rm32_imm32(cpu_i386 *proc, x86_instr_t *instr) {
    uint32_t rm32;

    rm32 = get_rm32(proc, instr);
    set_rm32(proc, instr, rm32 ^ instr->imm.imm32);
}

void cmp_rm32_imm32(cpu_i386 *proc, x86_instr_t *instr) {
    uint32_t rm32;

    rm32 = get_rm32(proc, instr);
    proc->regs.flags_reg.update_reg_sub(rm32, instr->imm.imm32);
}


void add_rm32_imm8(cpu_i386 *proc, x86_instr_t *instr) {
    uint32_t rm32;

    rm32 = get_rm32(proc, instr);
    set_rm32(proc, instr, rm32 + instr->imm.imm8);
    proc->regs.flags_reg.update_reg_add(rm32, instr->imm.imm8);
}

void or_rm32_imm8(cpu_i386 *proc, x86_instr_t *instr) {
    uint32_t rm32;

    rm32 = get_rm32(proc, instr);
    set_rm32(proc, instr, rm32 | instr->imm.imm8);
    proc->regs.flags_reg.update_reg_or(rm32, instr->imm.imm8);
}

void adc_rm32_imm8(cpu_i386 *proc, x86_instr_t *instr) {
    uint32_t rm32;
    uint8_t cf;

    rm32 = get_rm32(proc, instr);
    cf = proc->regs.flags_reg.carry;
    set_rm32(proc, instr, rm32 + instr->imm.imm8 + cf);
    proc->regs.flags_reg.update_reg_add(rm32, instr->imm.imm8 + cf);
}

void sbb_rm32_imm8(cpu_i386 *proc, x86_instr_t *instr) {
    uint32_t rm32;
    uint8_t cf;

    rm32 = get_rm32(proc, instr);
    cf = proc->regs.flags_reg.carry;
    set_rm32(proc, instr, rm32 - instr->imm.imm8 - cf);
    proc->regs.flags_reg.update_reg_sub(rm32, instr->imm.imm8 + cf);
}

void and_rm32_imm8(cpu_i386 *proc, x86_instr_t *instr) {
    uint32_t rm32;

    rm32 = get_rm32(proc, instr);
    set_rm32(proc, instr, rm32 & instr->imm.imm8);
    proc->regs.flags_reg.update_reg_and(rm32, instr->imm.imm8);
}

void sub_rm32_imm8(cpu_i386 *proc, x86_instr_t *instr) {
    uint32_t rm32;

    rm32 = get_rm32(proc, instr);
    set_rm32(proc, instr, rm32 - instr->imm.imm8);
    proc->regs.flags_reg.update_reg_sub(rm32, instr->imm.imm8);
}

void xor_rm32_imm8(cpu_i386 *proc, x86_instr_t *instr) {
    uint32_t rm32;

    rm32 = get_rm32(proc, instr);
    set_rm32(proc, instr, rm32 ^ instr->imm.imm8);
}

void cmp_rm32_imm8(cpu_i386 *proc, x86_instr_t *instr) {
    uint32_t rm32;

    rm32 = get_rm32(proc, instr);
    proc->regs.flags_reg.update_reg_sub(rm32, instr->imm.imm8);
}


void shl_rm32_imm8(cpu_i386 *proc, x86_instr_t *instr) {
    uint32_t rm32;

    rm32 = get_rm32(proc, instr);
    set_rm32(proc, instr, rm32 << instr->imm.imm8);
    proc->regs.flags_reg.update_reg_shl(rm32, instr->imm.imm8);
}

void shr_rm32_imm8(cpu_i386 *proc, x86_instr_t *instr) {
    uint32_t rm32;

    rm32 = get_rm32(proc, instr);
    set_rm32(proc, instr, rm32 >> instr->imm.imm8);
    proc->regs.flags_reg.update_reg_shr(rm32, instr->imm.imm8);
}

void sal_rm32_imm8(cpu_i386 *proc, x86_instr_t *instr) {
    int32_t rm32_s;

    rm32_s = get_rm32(proc, instr);
    set_rm32(proc, instr, rm32_s << instr->imm.imm8);
//	EFLAGS_UPDATE_SAL(rm32_s, instr->imm.imm8);
}

void sar_rm32_imm8(cpu_i386 *proc, x86_instr_t *instr) {
    int32_t rm32_s;

    rm32_s = get_rm32(proc, instr);
    set_rm32(proc, instr, rm32_s >> instr->imm.imm8);
//	EFLAGS_UPDATE_SAR(rm32_s, instr->imm.imm8);
}


void shl_rm32_cl(cpu_i386 *proc, x86_instr_t *instr) {
    uint32_t rm32;
    uint8_t cl;

    rm32 = get_rm32(proc, instr);
    cl = proc->regs.get_gpreg(CL);
    set_rm32(proc, instr, rm32 << cl);
    proc->regs.flags_reg.update_reg_shl(rm32, cl);
}

void shr_rm32_cl(cpu_i386 *proc, x86_instr_t *instr) {
    uint32_t rm32;
    uint8_t cl;

    rm32 = get_rm32(proc, instr);
    cl = proc->regs.get_gpreg(CL);
    set_rm32(proc, instr, rm32 >> cl);
    proc->regs.flags_reg.update_reg_shr(rm32, cl);
}

void sal_rm32_cl(cpu_i386 *proc, x86_instr_t *instr) {
    int32_t rm32_s;
    uint8_t cl;

    rm32_s = get_rm32(proc, instr);
    cl = proc->regs.get_gpreg(CL);
    set_rm32(proc, instr, rm32_s << cl);
//	EFLAGS_UPDATE_SAL(rm32_s, cl);
}

void sar_rm32_cl(cpu_i386 *proc, x86_instr_t *instr) {
    int32_t rm32_s;
    uint8_t cl;

    rm32_s = get_rm32(proc, instr);
    cl = proc->regs.get_gpreg(CL);
    set_rm32(proc, instr, rm32_s >> cl);
//	EFLAGS_UPDATE_SAR(rm32_s, cl);
}


void test_rm32_imm32(cpu_i386 *proc, x86_instr_t *instr) {
    uint32_t rm32, imm32;

    rm32 = get_rm32(proc, instr);
    imm32 = proc->read_code<32>();
    proc->regs.ip_reg.eip += (4);
    proc->regs.flags_reg.update_reg_and(rm32, imm32);
}

void not_rm32(cpu_i386 *proc, x86_instr_t *instr) {
    uint32_t rm32;

    rm32 = get_rm32(proc, instr);
    set_rm32(proc, instr, ~rm32);
}

void neg_rm32(cpu_i386 *proc, x86_instr_t *instr) {
    int32_t rm32_s;

    rm32_s = get_rm32(proc, instr);
    set_rm32(proc, instr, -rm32_s);
    proc->regs.flags_reg.update_reg_sub((uint32_t) 0, rm32_s);
}

void mul_edx_eax_rm32(cpu_i386 *proc, x86_instr_t *instr) {
    uint32_t rm32, eax;
    uint64_t val;

    rm32 = get_rm32(proc, instr);
    eax = proc->regs.get_gpreg(EAX);
    val = eax * rm32;

    proc->regs.set_gpreg(EAX, val);
    proc->regs.set_gpreg(EDX, val >> 32);

    proc->regs.flags_reg.update_reg_mul(eax, rm32);
}

void imul_edx_eax_rm32(cpu_i386 *proc, x86_instr_t *instr) {
    int32_t rm32_s, eax_s;
    int64_t val_s;

    rm32_s = get_rm32(proc, instr);
    eax_s = proc->regs.get_gpreg(EAX);
    val_s = eax_s * rm32_s;

    proc->regs.set_gpreg(EAX, val_s);
    proc->regs.set_gpreg(EDX, val_s >> 32);

    proc->regs.flags_reg.update_reg_imul(eax_s, rm32_s);
}

void div_edx_eax_rm32(cpu_i386 *proc, x86_instr_t *instr) {
    uint32_t rm32;
    uint64_t val;

    rm32 = get_rm32(proc, instr);
    CPU_EXCEPTION(EXP_DE, !rm32);
    val = proc->regs.get_gpreg(EDX);
    val <<= 32;
    val |= proc->regs.get_gpreg(EAX);

    proc->regs.set_gpreg(EAX, val / rm32);
    proc->regs.set_gpreg(EDX, val % rm32);
}

void idiv_edx_eax_rm32(cpu_i386 *proc, x86_instr_t *instr) {
    int32_t rm32_s;
    int64_t val_s;

    rm32_s = get_rm32(proc, instr);
    CPU_EXCEPTION(EXP_DE, !rm32_s);
    val_s = proc->regs.get_gpreg(EDX);
    val_s <<= 32;
    val_s |= proc->regs.get_gpreg(EAX);

    proc->regs.set_gpreg(EAX, val_s / rm32_s);
    proc->regs.set_gpreg(EDX, val_s % rm32_s);
}


void inc_rm32(cpu_i386 *proc, x86_instr_t *instr) {
    uint32_t rm32;

    rm32 = get_rm32(proc, instr);
    set_rm32(proc, instr, rm32 + 1);
    proc->regs.flags_reg.update_reg_add(rm32, 1);
}

void dec_rm32(cpu_i386 *proc, x86_instr_t *instr) {
    uint32_t rm32;

    rm32 = get_rm32(proc, instr);
    set_rm32(proc, instr, rm32 - 1);
    proc->regs.flags_reg.update_reg_sub(rm32, 1);
}

void call_rm32(cpu_i386 *proc, x86_instr_t *instr) {
    uint32_t rm32;

    rm32 = get_rm32(proc, instr);

    push32(proc, proc->regs.ip_reg.eip);
    proc->regs.ip_reg.eip = (rm32);
}

void callf_m16_32(cpu_i386 *proc, x86_instr_t *instr) {
    uint32_t m48, eip;
    uint16_t cs;

    m48 = get_m(proc, instr);
    eip = proc->read_mem<32>(select_segment(instr), m48);
    cs = proc->read_mem<16>(select_segment(instr), m48 + 4);


    callf(proc, cs, eip);
}

void jmp_rm32(cpu_i386 *proc, x86_instr_t *instr) {
    uint32_t rm32;

    rm32 = get_rm32(proc, instr);
    proc->regs.ip_reg.eip = (rm32);
}

void jmpf_m16_32(cpu_i386 *proc, x86_instr_t *instr) {
    uint32_t m48, eip;
    uint16_t sel;

    m48 = get_m(proc, instr);
    eip = proc->read_mem<32>(select_segment(instr), m48);
    sel = proc->read_mem<16>(select_segment(instr), m48 + 4);

    jmpf(proc, sel, eip);
}

void push_rm32(cpu_i386 *proc, x86_instr_t *instr) {
    uint32_t rm32;

    rm32 = get_rm32(proc, instr);
    push32(proc, rm32);
}


void lgdt_m32(cpu_i386 *proc, x86_instr_t *instr) {
    uint32_t m48, base;
    uint16_t limit;

    CPU_EXCEPTION(EXP_GP, !chk_ring(proc, 0));

    m48 = get_m(proc, instr);
    limit = proc->read_mem<16>(select_segment(instr), m48);
    base = proc->read_mem<32>(select_segment(instr), m48 + 2);

    proc->regs.set_dtreg(GDTR, 0, base, limit);
}

void lidt_m32(cpu_i386 *proc, x86_instr_t *instr) {
    uint32_t m48, base;
    uint16_t limit;

    CPU_EXCEPTION(EXP_GP, !chk_ring(proc, 0));

    m48 = get_m(proc, instr);
    limit = proc->read_mem<16>(select_segment(instr), m48);
    base = proc->read_mem<32>(select_segment(instr), m48 + 2);

    proc->regs.set_dtreg(IDTR, 0, base, limit);
}
