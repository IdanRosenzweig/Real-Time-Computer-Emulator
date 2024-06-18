#include "../common/common.h"

#include "font8x8.h"
#include "rgb.h"

#include "../../hardware/vga/vga_pmio_codes.h"
#include "../../hardware/vga/attr/attr_pmio_codes.h"
#include "../../hardware/vga/seq/seq_pmio_codes.h"
#include "../../hardware/vga/dac/dac_pmio_codes.h"
#include "../../hardware/vga/gc/gc_pmio_codes.h"
#include "../../hardware/vga/crt/crt_pmio_codes.h"

void mor_configure(void) {
    out_port(vga_pmio_output_codes::MOR, 0x2);
}

void attr_configure(void) {
    for (int i = 0; i < 0x10; i++) {
        out_port(attr_pmio_output_codes::SELECT_REG, i);
        out_port(attr_pmio_output_codes::ACCESS_REG, i);
    }
}

void seq_configure(void) {
    out_port(seq_pmio_output_codes::SELECT_REG, 2);
    out_port(seq_pmio_output_codes::ACCESS_REG, 0x3);

    out_port(seq_pmio_output_codes::SELECT_REG, 3);
    out_port(seq_pmio_output_codes::ACCESS_REG, 0x0);

    out_port(seq_pmio_output_codes::SELECT_REG, 4);
    out_port(seq_pmio_output_codes::ACCESS_REG, 0x2);
}

const rgb_t palette_conf[0x10] = {
//       R,    G,    B
        {0x00, 0x00, 0x00},
        {0x00, 0x00, 0x2a},
        {0x00, 0x2a, 0x00},
        {0x00, 0x2a, 0x2a},
        {0x2a, 0x00, 0x00},
        {0x2a, 0x00, 0x2a},
        {0x2a, 0x15, 0x00},
        {0x2a, 0x2a, 0x2a},
        {0x15, 0x15, 0x15},
        {0x15, 0x15, 0x3f},
        {0x15, 0x3f, 0x15},
        {0x15, 0x3f, 0x3f},
        {0x3f, 0x15, 0x15},
        {0x3f, 0x15, 0x3f},
        {0x3f, 0x3f, 0x15},
        {0x3f, 0x3f, 0x3f}
};

void dac_configure(void) {
    out_port(dac_pmio_output_codes::SET_WRITE_INDX, 0);

    for (int i = 0; i < 0x10; i++) {
        out_port(dac_pmio_output_codes::WRITE_COLOR, palette_conf[i].red);
        out_port(dac_pmio_output_codes::WRITE_COLOR, palette_conf[i].green);
        out_port(dac_pmio_output_codes::WRITE_COLOR, palette_conf[i].blue);
    }
}

void gc_configure(void) {
    out_port(gc_pmio_output_codes::SELECT_REG, 5);
    out_port(gc_pmio_output_codes::ACCESS_REG, 0x10);

    out_port(gc_pmio_output_codes::SELECT_REG, 6);
    out_port(gc_pmio_output_codes::ACCESS_REG, 0xe);
}

void load_font(void) {

    // disable interrupts
    disable_intr();

    out_port(0x3c4, 2);
    out_port(0x3c5, 0x4);
    out_port(0x3c4, 4);
    out_port(0x3c5, 0x6);

    out_port(0x3ce, 5);
    out_port(0x3cf, 0x0);
    out_port(0x3ce, 6);
    out_port(0x3cf, 0x0);

    // enable interrupts back
    enable_intr();

//    __asm__("push es\n"
//            "mov ax, 0xa000\n"
//            "mov es, ax");
    __asm__("push es\n");
    set_es(0xa000);

    for (int i = 0; i < 0x80; i++) {
        uint8_t *p = font8x8_basic[i];
        write_es_dword((uint32_t *) (i * 0x10), *(uint32_t *) &p[0]);
        write_es_dword((uint32_t *) (i * 0x10 + 4), *(uint32_t *) &p[4]);
    }

    __asm__("pop es");
}

void crt_configure(void) {
    // width
    // x : 320
    out_port(crt_pmio_output_codes::SELECT_REG1, 0x1);
    out_port(crt_pmio_output_codes::ACCESS_REG1, 0x28);

    // height
    // y : 200
    out_port(crt_pmio_output_codes::SELECT_REG1, 0x12);
    out_port(crt_pmio_output_codes::ACCESS_REG1, 0x19);

    // maximum scanline
    // MSL : 8
    out_port(crt_pmio_output_codes::SELECT_REG1, 0x9);
    out_port(crt_pmio_output_codes::ACCESS_REG1, 0x8-0x1);
}

EXTERNC
void vga_configure(void) {
    disable_intr();
    mor_configure();
    enable_intr();

    load_font();

    disable_intr();

    attr_configure();
    seq_configure();
    dac_configure();
    gc_configure();
    crt_configure();

    enable_intr();
}

