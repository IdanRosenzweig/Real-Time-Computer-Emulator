#ifndef EMULATOR_IVT_ENTRY_H
#define EMULATOR_IVT_ENTRY_H

// interrupt vector table

// the absolute address of the IVT
#define IVT_ADDR 0x0

// entry in the interrupt vector table
struct ivt_entry {
    union {
        uint32_t raw;
        struct {
            uint16_t offset;
            uint16_t segment;
        };
    };
};


#endif
