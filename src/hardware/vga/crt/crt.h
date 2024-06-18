#ifndef EMULATOR_CRT_H
#define EMULATOR_CRT_H

#include <stdint.h>
#include "../../cpu_i386/io/port_mapped_io.h"

#define r(reg)    (&reg.raw)

class vga;

class crt : public pmio {
public:

    // horizontal values are in units of "character block". each block is 8 pixels
    const uint32_t CHAR_BLOCK = 8;

    // horizontal total register
    union {
        uint8_t raw;
        uint8_t HT;
    } htr;

    // horizontal display end register
    // this represents the number of active char blocks in a single line (active: values are read from graphic memory)
    union {
        uint8_t raw;
        uint8_t HDEE;
    } hdeer;

    // start horizontal blacking register
    union {
        uint8_t raw;
        uint8_t SHB;
    } shbr;

    // end horizontal blacking register
    union {
        uint8_t raw;
        struct {
            uint8_t EB: 5;
            uint8_t DESC: 2;
        };
    } ehbr;


    // maximum scan line register
    union {
        uint8_t raw;
        struct {
            uint8_t MSL: 5;
            uint8_t SVB9: 1;
            uint8_t LC9: 1;
            uint8_t LC: 1;
        };
    } mslr;


    union {
        uint8_t raw;
        struct {
            uint8_t RSCB: 5;
            uint8_t CO: 1;
        };
    } csr;

    // vertical retrace end register
    union {
        uint8_t raw;
        struct {
            uint8_t RSCE: 5;
            uint8_t CSC: 2;
        };
    } cer;

    union {
        uint8_t raw;
        uint8_t HBSA;
    } sahr;


    union {
        uint8_t raw;
        uint8_t LBSA;
    } salr;

    union {
        uint8_t raw;
        uint8_t HBCL;
    } clhr;

    union {
        uint8_t raw;
        uint8_t LBCL;
    } cllr;


    // vertical display end register
    // this represents the number of active scanlines (active: are values read from graphic memory)
    union {
        uint8_t raw;
        uint8_t VDEE;
    } vdeer;

    // logical width
    union {
        uint8_t raw;
        uint8_t LLWS;
    } ofsr;

    // mode control register
    union {
        uint8_t raw;
        struct {
            uint8_t CMS0: 1;
            uint8_t SRSC: 1;
            uint8_t HRSX: 1;
            uint8_t C2: 1;
            uint8_t : 1;
            uint8_t AW: 1;
            uint8_t WBM: 1;
            uint8_t HR: 1;
        };
    } crtmcr;

    // the various registers and their assigned indexes
    uint8_t *regs[0x19] = {
            r(htr),
            r(hdeer),
            r(shbr),
            r(ehbr),

            NULL,
            NULL,
            NULL,
            NULL,
            NULL,

            r(mslr),

            r(csr),
            r(cer),

            r(sahr),

            r(salr),

            r(clhr),
            r(cllr),
            NULL,
            NULL,
            r(vdeer),
            r(ofsr),
            NULL,
            NULL,
            NULL,
            r(crtmcr),
            NULL};

    // in io operations, this value holds the index to the corresponding register that is being accessed
    union {
        uint8_t raw;
        struct {
            uint8_t INDX: 5;
        };
    } crtcar;

    // the number of char block in each line
    uint32_t CHAR_BLOCKS_PER_LINE() { return hdeer.HDEE; }

    // the number of pixels per line
    uint32_t PIXELS_PER_LINE() { return CHAR_BLOCKS_PER_LINE() * CHAR_BLOCK; }

    // the number of pixel lines
    uint32_t NO_SCANLINES() { return vdeer.VDEE * SCANLINE_REPS(); }

    // the number of times each scanline is repeated
    size_t SCANLINE_REPS() { return 1 + mslr.MSL; }

public:
    crt();

    // get window size in units of pixels
    void get_windowsize(uint16_t *x, uint16_t *y);


    uint8_t read_byte_from_port(uint16_t addr) override;

    void write_byte_to_port(uint16_t addr, uint8_t v) override;

};


#endif
