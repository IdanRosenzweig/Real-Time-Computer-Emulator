#include "instr_decoder.h"
#include "helper_funcs.h"

#include <cstring>
#include <unordered_map>

std::unordered_map <opcode_t, x86_instr_flags_t> opcode_flags_common;
std::unordered_map <opcode_t, x86_instr_flags_t> opcode_flags_mode16;
std::unordered_map <opcode_t, x86_instr_flags_t> opcode_flags_mode32;

void decode_prefixes(cpu_i386 *processor, x86_instr_t *instruction) {
    while (true) {

        uint8_t code = processor->read_code<8>(); // get the next byte
        switch (code) { // check if the byte has a valid prefix value

            // segment override: use the specified segment instead of the default one
            case 0x26:
                instruction->segment_override = {true, ES};
                goto next;
            case 0x2e:
                instruction->segment_override = {true, CS};
            case 0x36:
                instruction->segment_override = {true, SS};
                goto next;
            case 0x3e:
                instruction->segment_override = {true, DS};
                goto next;
            case 0x64:
                instruction->segment_override = {true, FS};
                goto next;
            case 0x65:
                instruction->segment_override = {true, GS};
                goto next;

                // operand override
            case 0x66:
                instruction->operand_override = true;
                goto next;

                // address override: use 16 bit instead of 32 and vise versa
            case 0x67:
                instruction->addr_override = true;
                goto next;

                // repetition prefixes
            case 0xf2:
                instruction->repetition_pre = REPNZ;
                goto next;
            case 0xf3:
                instruction->repetition_pre = REPZ;
                goto next;

            case 0xf0:
                instruction->mutual_exc_pre = true;
                goto next;

                // the byte is not a valid prefix
            default:
                goto stop;
        }

        next:
        {
            processor->regs.ip_reg.eip += 1;
            continue;
        }

        stop:
        break;
    }
}

void decode_opcode(cpu_i386 *processor, x86_instr_t *instruction) {
    uint8_t opcode = processor->read_code<8>();
    processor->regs.ip_reg.eip += 1;

    if (opcode != 0x0f) { // 1-byte opcode
        instruction->opcode = opcode;

    } else { // 2-byte opcode
        instruction->opcode = (opcode << 8) + processor->read_code<8>();
        processor->regs.ip_reg.eip += 1;
    }

//    if (processor->regs.is_mode32())
//        DEBUG_MSG( "CS:%04x EIP:0x%04x opcode:%02x ", processor->regs.get_sgreg(CS), processor->regs.ip_reg.eip - 1,
//                  instruction->opcode);
//    else
//        DEBUG_MSG( "CS:%04x  IP:0x%04x opcode:%02x ", processor->regs.get_sgreg(CS), processor->regs.ip_reg.ip - 1,
//                  instruction->opcode);
}

void decode_modrm_sib_disp(cpu_i386 *proc, x86_instr_t *instruction) {
    uint8_t modrm = proc->read_code<8>();
    proc->regs.ip_reg.eip += 1;

    instruction->modrm.raw = modrm;

    if (get_is_mode32(proc, instruction) ^ get_is_addr_override(proc, instruction)) {
        // 32 bit mode

        if (instruction->modrm.mod != 3 && instruction->modrm.rm == 4) {
            instruction->sib.raw = proc->read_code<8>();
            proc->regs.ip_reg.eip += 1;

        }

        if (instruction->modrm.mod == 2
            || (instruction->modrm.mod == 0 && instruction->modrm.rm == 5)
            || (instruction->modrm.mod == 0 && instruction->sib.base == 5)) {

            instruction->disp.disp32 = proc->read_code<32>();
            proc->regs.ip_reg.eip += 4;

        } else if (instruction->modrm.mod == 1) {
            instruction->disp.disp8 = (int8_t) proc->read_code<8>();
            proc->regs.ip_reg.eip += 1;

        }

    } else {
        // 16 bit mode

        if ((instruction->modrm.mod == 0 && instruction->modrm.rm == 6) || instruction->modrm.mod == 2) {
            instruction->disp.disp16 = proc->read_code<32>(); // todo get 32 or 16 ?
            proc->regs.ip_reg.eip += 2;

        } else if (instruction->modrm.mod == 1) {
            instruction->disp.disp8 = (int8_t) proc->read_code<8>();
            proc->regs.ip_reg.eip += 1;

        }

    }
}

void decode_moffs(cpu_i386 *proc, x86_instr_t *instruction) {
    if (get_is_mode32(proc, instruction) ^ get_is_addr_override(proc, instruction)) {
        instruction->moffs = proc->read_code<32>();
        proc->regs.ip_reg.eip += 4;

    } else {
        instruction->moffs = proc->read_code<16>();
        proc->regs.ip_reg.eip += 2;

    }
}

void decode(cpu_i386 *proc, x86_instr_t *instruction) {
    // parse opcode prefixes
    decode_prefixes(proc, instruction);

    // parse opcode number
    decode_opcode(proc, instruction);
    opcode_t opcode = instruction->opcode;

    // find opcode flags
    x86_instr_flags_t flags;
    memset(&flags, '\x00', sizeof(flags));
    if (opcode_flags_common.count(opcode)) {
        flags = opcode_flags_common[opcode];

    } else {
        if (get_is_mode32(proc, instruction)) {
            if (!opcode_flags_mode32.count(opcode)) {

                DEBUG_ERROR("no opecode for mode32: %x\n", opcode);
                return;
            }

            flags = opcode_flags_mode32[opcode];
        } else {
            if (!opcode_flags_mode16.count(opcode)) {

                DEBUG_ERROR("no opecode for mode16: %x\n", opcode);
                return;
            }

            flags = opcode_flags_mode16[opcode];
        }
    }

    // parse modrm if needed
    if (flags.flags.has_modrm) {
        decode_modrm_sib_disp(proc, instruction);
    }

    // parse imm if needed
    if (flags.flags.has_imm32) {
        instruction->imm.imm32 = proc->read_code<32>();
        proc->regs.ip_reg.eip += 4;

    } else if (flags.flags.has_imm16) {
        instruction->imm.imm16 = proc->read_code<16>();
        proc->regs.ip_reg.eip += 2;

    } else if (flags.flags.has_imm8) {
        instruction->imm.imm8 = (int8_t) proc->read_code<8>();
        proc->regs.ip_reg.eip += 1;

    }

    // parse ptr16 if needed
    if (flags.flags.has_ptr16) {
        instruction->ptr16 = proc->read_code<16>();
        proc->regs.ip_reg.eip += 2;

    }

    // parse moffs if needed
    if (flags.flags.has_moffs) {

        decode_moffs(proc, instruction);
    }


}