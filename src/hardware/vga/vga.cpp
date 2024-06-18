#include "vga.hpp"
#include "vga_pmio_codes.h"
#include <iostream>
using namespace std;;

vga::vga() : seq(this), crt(), gc(this), attr(this), dac() {
    for (int i = 0; i < 4; i++)
        plane[i] = new uint8_t[1 << 16];

    count = 0;
}

void vga::get_windowsize(uint16_t *x, uint16_t *y) {
    crt.get_windowsize(x, y);
}

void vga::display_rgb_image(uint8_t *buffer, uint32_t size) {
    gmode_t mode = gc.curr_graphics_mode(); // current graphics mode

    for (uint32_t i = 0; i < size; i++) {
        uint8_t attr_idx;
        switch (mode) {
            case MODE_TEXT:
                attr_idx = attr_index_text_mode(i);
                break;

            case MODE_GRAPHIC:
            case MODE_GRAPHIC256:
                attr_idx = attr_index_graphic_mode(i);
                break;
        }

        uint8_t dac_idx;
        switch (mode) {
            case MODE_TEXT:
            case MODE_GRAPHIC:
                dac_idx = attr.dac_index(attr_idx);
                break;

            case MODE_GRAPHIC256:
                dac_idx = attr_idx;
                break;
        }

        // get the final rgb value
        uint32_t rgb = dac.get_rgb(dac_idx);

        // display it on the buffer
        *(buffer++) = rgb & 0xff;
        *(buffer++) = (rgb >> 8) & 0xff;
        *(buffer++) = (rgb >> 16) & 0xff;
    }
}


uint8_t vga::attr_index_graphic_mode(uint32_t n) {
    return read_plane(2, n);
}

uint8_t vga::attr_index_text_mode(uint32_t n) {

    uint16_t pxl_line = n / crt.PIXELS_PER_LINE(); // index of the pixel line
    uint16_t pxl_offset = n % crt.PIXELS_PER_LINE(); // offset into the pixel line


    uint16_t scanline_num = pxl_line / crt.SCANLINE_REPS(); // the scanline number (without repeating)
    uint16_t scanline_reminder = pxl_line % crt.SCANLINE_REPS(); // the reminder for the scanline repetition

    uint16_t char_block_num = pxl_offset / crt.CHAR_BLOCK; // the char block number (in the scanline)
    uint16_t char_block_reminder = pxl_offset % crt.CHAR_BLOCK; // the reminder for the char block num


    uint16_t indx = scanline_num * crt.CHAR_BLOCKS_PER_LINE();
    indx += char_block_num;

    uint8_t chr = read_plane(0, indx * 2);
    uint8_t att = read_plane(1, indx * 2);

    uint8_t *font = seq.get_font(att);
    uint8_t bits = *(font + chr * 0x10 + scanline_reminder);

    return (bits >> char_block_reminder) & 1 ? att & 0x0f
                                             : (att & 0xf0) >> 4;
}


uint8_t vga::read_byte_from_port(uint16_t addr) {
    switch (addr) {
        case 0x3c2:
            return 0;
        case 0x3c3:
            return 0;
        case vga_pmio_input_codes::MOR:
            return mor.raw;

        case 0x3ba:
        case 0x3da:
            return 0;
    }
    return -1;
}

void vga::write_byte_to_port(uint16_t addr, uint8_t v) {
    switch (addr) {
        case vga_pmio_output_codes::MOR:
            mor.raw = v;
            break;

        case 0x3c3:
            break;

        case 0x3ba:
        case 0x3da:
            break;
    }

}


uint8_t vga::read_byte_from_mem(uint32_t addr) {
    if (!mor.ER) return 0; // reading is disabled

    return seq.read(addr);
}

void vga::write_byte_to_mem(uint32_t addr, uint8_t v) {
    if (!mor.ER) return; // writing is disabled

    seq.write(addr, v);

    count = (count + 1) % REFRESH_RATE; // incrementing the write count, module the refresh rate
    if (count == 0) // if the count resets, a refresh is required
        refresh = true;
}



