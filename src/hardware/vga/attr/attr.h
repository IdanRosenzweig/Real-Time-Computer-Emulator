#ifndef EMULATOR_ATTR_H
#define EMULATOR_ATTR_H

#include <stdint.h>
#include "../../cpu_i386/io/port_mapped_io.h"

#define r(reg)    (&reg.raw)
#define chk_regidx(n)    do{ if(n>sizeof(regs))DEBUG_ERROR("register index out of bound", n); if(!regs[n])DEBUG_ERROR("not implemented"); }while(0);

class vga;

class attribute : public pmio {
private:
    class vga *vga;

    // internal palette registers
    union {
        uint8_t raw;
        struct {
            uint8_t P0: 1;
            uint8_t P1: 1;
            uint8_t P2: 1;
            uint8_t P3: 1;
            uint8_t P4: 1;
            uint8_t P5: 1;
        };
    } ipr[0x10];

    // attribute mode controller register
    union {
        uint8_t raw;
        struct {
            uint8_t GAM: 1;
            uint8_t ME: 1;
            uint8_t ELGCC: 1;
            uint8_t ELSBI: 1;
            uint8_t : 1;
            uint8_t PELPC: 1;
            uint8_t PELW: 1;
            uint8_t P54S: 1;
        };
    } amcr;

    // color plane enable register
    union {
        uint8_t raw;
        struct {
            uint8_t ECP: 4;
            uint8_t VSM: 2;
        };
    } cper;

    // horizontal pixel panning register
    union {
        uint8_t raw;
        struct {
            uint8_t HPELP: 4;
        };
    } hpelpr;

    // color select register
    union {
        uint8_t raw;
        struct {
            uint8_t SC45: 2;
            uint8_t SC67: 2;
        };
    } csr;


    // the various registers and their assigned indexes
    uint8_t *regs[0x15] = {r(ipr[0x0]), r(ipr[0x1]), r(ipr[0x2]), r(ipr[0x3]),
                           r(ipr[0x4]), r(ipr[0x5]), r(ipr[0x6]), r(ipr[0x7]),
                           r(ipr[0x8]), r(ipr[0x9]), r(ipr[0xa]), r(ipr[0xb]),
                           r(ipr[0xc]), r(ipr[0xd]), r(ipr[0xe]), r(ipr[0xf]),
                           r(amcr),
                           NULL,
                           r(cper),
                           r(hpelpr),
                           r(csr)};

    // in io operations, this value holds the index to the corresponding register that is being accessed
    union {
        uint8_t raw;
        struct {
            uint8_t INDX: 5;
            uint8_t IPAS: 1;
        };
    } acar;

public:
    attribute(class vga *v);

    uint8_t dac_index(uint8_t index);


    uint8_t read_byte_from_port(uint16_t addr) override;

    void write_byte_to_port(uint16_t addr, uint8_t v) override;
};


#endif
