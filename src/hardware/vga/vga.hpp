#ifndef EMULATOR_VGA_H
#define EMULATOR_VGA_H

#include "../../common.hpp"
#include "../cpu_i386/io/mem_mapped_io.h"
#include "../cpu_i386/io/port_mapped_io.h"

#include "crt/crt.h"
#include "seq/sequencer.h"
#include "gc/gc.h"
#include "dac/dac.h"
#include "attr/attr.h"


class vga : public pmio, public mmio {
public:
    // miscellaneous output register
    union {
        uint8_t raw;
        struct {
            uint8_t IO: 1;

            // enables/disables access to the internal display buffer
            uint8_t ER: 1;

            uint8_t CLK0: 1;
            uint8_t CLK1: 1;
            uint8_t : 1;
            uint8_t PS: 1;
            uint8_t HSP: 1;
            uint8_t VSA: 1;
        };
    } mor;


#define NO_PLANES 4
    uint8_t *plane[NO_PLANES];

    inline uint8_t read_plane(uint8_t nplane, uint32_t offset) {
        if (nplane >= NO_PLANES || offset > (1 << 16) - 1) DEBUG_ERROR("Out of Plane range");
        return plane[nplane][offset];
    }

    inline void write_plane(uint8_t nplane, uint32_t offset, uint8_t v) {
        if (nplane >= NO_PLANES || offset > (1 << 16) - 1) DEBUG_ERROR("Out of Plane range");
        plane[nplane][offset] = v;
    }


    // internal variable that determines whether a display refresh is needed
    // (depending on the amount of data written, if any)
    bool refresh;

    bool need_refresh(void) {
        if (!refresh) return false;

        refresh = false;
        return true;
    };

    // the maximum number of bytes that will be written before querying a required
    // (1 means after every byte write a refresh is required)
    int count;
#define REFRESH_RATE 1


    class sequencer seq;
    class crt crt;
    class graphics_controller gc;
    class attribute attr;
    class dac dac;

    vga();

    ~vga() { for (int i = 0; i < 4; i++) delete plane[i]; };


    void get_windowsize(uint16_t *x, uint16_t *y);

    // display the current rgb image into the buffer
    void display_rgb_image(uint8_t *buffer, uint32_t size);

    // get index in terms of pixels.
    // assuming graphic mode, return the index in terms of attribute.
    uint8_t attr_index_graphic_mode(uint32_t n);

    // get index in terms of pixels.
    // assuming text mode, return the index in terms of attribute.
    uint8_t attr_index_text_mode(uint32_t n);


    uint8_t read_byte_from_port(uint16_t addr) override;

    void write_byte_to_port(uint16_t addr, uint8_t v) override;


    uint8_t read_byte_from_mem(uint32_t addr) override;

    void write_byte_to_mem(uint32_t addr, uint8_t v) override;


};

#endif
