#ifndef EMULATOR_GATE_DESC_H
#define EMULATOR_GATE_DESC_H

#include <stdint.h>


// gate descriptor, used in the IDT
struct gate_desc {
    uint16_t offset_l;
    uint16_t sel;

    uint8_t pc: 5;
    uint8_t : 3;

    uint8_t type: 3;    // 4
    uint8_t D: 1;
    uint8_t S: 1;        // 0
    uint8_t DPL: 2;
    uint8_t P: 1;

    uint16_t offset_h;
};

//struct IntGateDesc {
//    uint16_t offset_l;
//    uint16_t seg_sel;
//
//    uint8_t : 8;
//
//    uint8_t type : 3;	// 6
//    uint8_t D : 1;
//    uint8_t S : 1;		// 0
//    uint8_t DPL : 2;
//    uint8_t P : 1;
//
//    uint16_t offset_h;
//};

//struct TaskGateDesc {
//    uint16_t : 16;
//    uint16_t tss_sel;
//
//    uint8_t : 8;
//
//    uint8_t type : 3;	// 5
//    uint8_t : 1;
//    uint8_t S : 1;		// 0
//    uint8_t DPL : 2;
//    uint8_t P : 1;
//
//    uint16_t : 16;
//};

//struct TrapGateDesc {
//    uint16_t offset_l;
//    uint16_t seg_sel;
//
//    uint8_t : 8;
//
//    uint8_t type : 3;	// 7
//    uint8_t D : 1;
//    uint8_t S : 1;		// 0
//    uint8_t DPL : 2;
//    uint8_t P : 1;
//
//    uint16_t offset_h;
//};

#endif
