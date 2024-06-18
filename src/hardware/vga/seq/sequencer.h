#ifndef EMULATOR_SEQUENCER_H
#define EMULATOR_SEQUENCER_H

#include <stdint.h>
#include "../../cpu_i386/io/port_mapped_io.h"

#define r(reg)    (&reg.raw)
#define chk_regidx(n)    do{ if(n>sizeof(regs))DEBUG_ERROR("register index out of bound", n); if(!regs[n])DEBUG_ERROR("not implemented"); }while(0);

class vga;

class sequencer : public pmio {
public:
    class vga *vga;


    union {
        uint8_t raw;
        struct {
            uint8_t _89DC: 1;
            uint8_t : 1;
            uint8_t SL: 1;
            uint8_t DC: 1;
            uint8_t S4: 1;
            uint8_t SO: 1;
        };
    } cmr;

    // map mask register
    union {
        uint8_t raw;
        struct {
            uint8_t MAP0E: 1;
            uint8_t MAP1E: 1;
            uint8_t MAP2E: 1;
            uint8_t MAP3E: 1;
        };
    } map_mr;

    // character map select register
    union {
        uint8_t raw;
        struct {
            uint8_t CMB: 2;
            uint8_t CMA: 2;
            uint8_t CMBM: 1;
            uint8_t CMAM: 1;
        };
    } cmsr;

    // memory mode register
    union {
        uint8_t raw;
        struct {
            uint8_t : 1;
            uint8_t EM: 1;
            uint8_t OE: 1;
            uint8_t C4: 1;
        };
    } mem_mr;


    // the various registers and their assigned indexes
    uint8_t *regs[8] = {
            NULL,
            r(cmr),
            r(map_mr),
            r(cmsr),
            r(mem_mr),
            NULL,
            NULL,
            NULL};

    // in io operations, this value holds the index to the corresponding register that is being accessed
    union {
        uint8_t raw;
        struct {
            uint8_t INDX: 3;
        };
    } sar;

public:
    sequencer(class vga *v);

    uint8_t read(uint32_t addr);

    void write(uint32_t addr, uint8_t v);

    uint8_t *get_font(uint8_t att);


    uint8_t read_byte_from_port(uint16_t addr) override;

    void write_byte_to_port(uint16_t addr, uint8_t v) override;
};


#endif
