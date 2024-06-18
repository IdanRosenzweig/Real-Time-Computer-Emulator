#include "gc.h"
#include "../vga.hpp"

#include "gc_pmio_codes.h"

#include <iostream>
using namespace std;;

bool graphics_controller::sync_offset(uint32_t *offset) {
    uint32_t base, size;
    get_mem_area(&base, &size);

    if (base <= *offset && *offset < base + size) { // in range
        *offset -= base;
        return true;
    } else return false;
}

graphics_controller::graphics_controller(class vga *v) {
    vga = v;
    for (uint8_t i = 0; i < sizeof(regs) / sizeof(uint8_t * ); i++)
        if (regs[i] != nullptr) *regs[i] = 0;
}


uint8_t graphics_controller::read(uint32_t addr) {
    if (!sync_offset(&addr)) return 0;

    switch (gmr.WM) {
        case 0:
            if (gmr.OE) {
                uint8_t nplane = (rmsr.MS & 2) + (addr & 1);
                return vga->read_plane(nplane, addr & (~1));
            } else
                return vga->read_plane(rmsr.MS, addr);
        case 1:
            break;
    }

    return 0;
}

void graphics_controller::write(uint8_t nplane, uint32_t addr, uint8_t v) {
    if (!sync_offset(&addr)) return;

    switch (gmr.WM) {
        case 0:
            if (gmr.OE) {
                addr &= ~1;
            }
            vga->write_plane(nplane, addr, v);
            break;
        case 1:
            break;
        case 2:
            break;
        case 3:
            break;
    }

}


gmode_t graphics_controller::curr_graphics_mode(void) {
    if (mr.GM) {
        if (gmr._256CM) return MODE_GRAPHIC256;
        else return MODE_GRAPHIC;
    } else  return MODE_TEXT;
}


uint8_t graphics_controller::read_byte_from_port(uint16_t addr) {
    switch (addr) {
        // get the index of the currently selected register
        case gc_pmio_input_codes::GET_SELECTED_REG:
            return gcar.raw;

            // read the register that is currently selected
        case gc_pmio_input_codes::ACCESS_REG:
            return *regs[gcar.INDX];
    }
    return -1;
}

void graphics_controller::write_byte_to_port(uint16_t addr, uint8_t v) {
    switch (addr) {
        // set the index of the currently selected register (for future access)
        case gc_pmio_output_codes::SELECT_REG:
            chk_regidx(v);
            gcar.raw = v;
            break;

            // write to the register that is currently selected
        case gc_pmio_output_codes::ACCESS_REG:
            *regs[gcar.INDX] = v;
            break;
    }
}