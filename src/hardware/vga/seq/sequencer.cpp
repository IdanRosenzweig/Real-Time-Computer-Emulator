#include "sequencer.h"
#include "../vga.hpp"
#include "seq_pmio_codes.h"

#include <iostream>
using namespace std;;

sequencer::sequencer(class vga *v) {
    vga = v;
    for (uint8_t i = 0; i < sizeof(regs) / sizeof(uint8_t * ); i++)
        if (regs[i] != nullptr) *regs[i] = 0;
}


uint8_t sequencer::read(uint32_t addr) {
    // todo change the absolute addr to offset before enabling only 64k?
    if (!mem_mr.EM) // enable only 64k
        addr &= (1 << 16) - 1;

    return vga->gc.read(addr);
}

#define SEQ_WRITE_PLANE(n, o, v)    if((map_mr.raw>>(n)) & 1) vga->gc.write(n, o ,v)

void sequencer::write(uint32_t addr, uint8_t v) {
    // todo change the absolute addr to offset before enabling only 64k?
    if (!mem_mr.EM) // enable only 64k
        addr &= (1 << 16) - 1;

    if (mem_mr.C4) {
//        SEQ_WRITE_PLANE(addr & 3, addr & (~3), v);
        if ((map_mr.raw >> (addr & 3)) & 1)
            vga->gc.write(addr & 3, addr & (~3), v);

    } else {
        if (mem_mr.OE) {
            for (int i = 0; i < 4; i++)
//                SEQ_WRITE_PLANE(i, addr, v);
                if ((map_mr.raw >> (i)) & 1) vga->gc.write(i, addr, v);

        } else {
            uint8_t nplane = addr & 1;
//            SEQ_WRITE_PLANE(nplane, addr, v);
            if ((map_mr.raw >> (nplane)) & 1)
                vga->gc.write(nplane, addr, v);
//            SEQ_WRITE_PLANE(nplane + 2, addr, v);
            if ((map_mr.raw >> (nplane + 2)) & 1)
                vga->gc.write(nplane + 2, addr, v);

        }
    }
}

uint8_t *sequencer::get_font(uint8_t att) {
    // number of font used
    uint8_t font_num =
            // if bit 3 is on, select cmsr.CMA plus cmsr.CMAM as the third bit
            (att & (1 << 3)) ? (cmsr.CMAM << 2) + cmsr.CMA
            // otherwise, select cmsr.CMB plus cmsr.CMBM as the third bit
                             : (cmsr.CMBM << 2) + cmsr.CMB;

    // calculating the font's offset
    uint16_t font_off = (font_num & 0b100) ? (font_num & (~0b100)) * 2 + 1
                                           : font_num * 2;

    if (!mem_mr.EM) // enable only 64k
        font_off &= (1 << 16) - 1;

    return vga->plane[2] + font_off;
}


uint8_t sequencer::read_byte_from_port(uint16_t addr) {
    switch (addr) {
        // get the index of the currently selected register
        case seq_pmio_input_codes::GET_SELECTED_REG:
            return sar.raw;

            // read the register that is currently selected
        case seq_pmio_input_codes::ACCESS_REG:
            return *regs[sar.INDX];
    }
    return -1;
}

void sequencer::write_byte_to_port(uint16_t addr, uint8_t v) {
    switch (addr) {
        // set the index of the currently selected register (for future access)
        case seq_pmio_output_codes::SELECT_REG:
            chk_regidx(v);
            sar.raw = v;
            break;

            // write to the register that is currently selected
        case seq_pmio_output_codes::ACCESS_REG:
            *regs[sar.INDX] = v;
            break;
    }
}