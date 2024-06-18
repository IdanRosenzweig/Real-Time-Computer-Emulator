#ifndef EMULATOR_GC_H
#define EMULATOR_GC_H

#include <stdint.h>
#include "../../cpu_i386/io/port_mapped_io.h"

#define r(reg)    (&reg.raw)
#define chk_regidx(n)    do{ if(n>sizeof(regs))DEBUG_ERROR("register index out of bound", n); if(!regs[n])DEBUG_ERROR("not implemented"); }while(0);

enum gmode_t {
    MODE_TEXT,          // 0
    MODE_GRAPHIC,       // 1
    MODE_GRAPHIC256     // 2
};

class vga;

class graphics_controller : public pmio {
private:
    class vga *vga;

    // set/reset register
    union {
        uint8_t raw;
        struct {
            uint8_t SRM0: 1;
            uint8_t SRM1: 1;
            uint8_t SRM2: 1;
            uint8_t SRM3: 1;
        };
    } sr;

    // enable set/reset register
    union {
        uint8_t raw;
        struct {
            uint8_t ESRM0: 1;
            uint8_t ESRM1: 1;
            uint8_t ESRM2: 1;
            uint8_t ESRM3: 1;
        };
    } esr;

    // color compare register
    union {
        uint8_t raw;
        struct {
            uint8_t CCM0: 1;
            uint8_t CCM1: 1;
            uint8_t CCM2: 1;
            uint8_t CCM3: 1;
        };
    } ccr;

    // data rotate register
    union {
        uint8_t raw;
        struct {
            uint8_t RC: 3; // rotation count
            uint8_t FS: 2; // operation
        };
    } drr;

    // read map selected register
    union {
        uint8_t raw;
        struct {
            uint8_t MS: 2;
        };
    } rmsr;

    // graphics mode register
    union {
        uint8_t raw;
        struct {
            uint8_t WM: 2; // write mode
            uint8_t : 1;
            uint8_t RM: 1; // read mode

            uint8_t OE: 1; // odd/even bit
            uint8_t SRM: 1;
            uint8_t _256CM: 1;
        };
    } gmr;


    // miscellaneous graphics register
    union {
        uint8_t raw;
        struct {
            uint8_t GM: 1; // controls alphanumeric/graphic mode
            uint8_t OE: 1;

            uint8_t MM: 2;
        };
    } mr;

    inline void get_mem_area(uint32_t* base, uint32_t* size) {
        switch (mr.MM) { // depending on the mmio area
            case 0:
                *base = 0xa0000;
                *size = 0x20000;
                break;
            case 1:
                *base = 0xa0000;
                *size = 0x10000;
                break;
            case 2:
                *base = 0xb0000;
                *size = 0x08000;
                break;
            case 3:
                *base = 0xb8000;
                *size = 0x08000;
                break;
        }
    }

    // sync offset to be able to access the memory planes.
    // if the offset is in the map range, decrement the base addr from the offset and return true.
    // otherwise return false and don't change offset
    bool sync_offset(uint32_t *offset);


    // the various registers and their assigned indexes
    uint8_t *regs[9] = {
            r(sr),
            r(esr),

            r(ccr),
            r(drr),
            r(rmsr),
            r(gmr),
            r(mr),
            NULL,
            NULL};

    // in io operations, this value holds the index to the corresponding register that is being accessed
    union {
        uint8_t raw;
        struct {
            uint8_t INDX: 4;
        };
    } gcar;

public:
    graphics_controller(class vga *v);


    // read and write to memory planes
    uint8_t read(uint32_t addr);

    void write(uint8_t nplane, uint32_t addr, uint8_t v);


    // get the current graphics mode
    gmode_t curr_graphics_mode(void);


    uint8_t read_byte_from_port(uint16_t addr) override;

    void write_byte_to_port(uint16_t addr, uint8_t v) override;

};


#endif
