#ifndef EMULATOR_IP_REGISTER_H
#define EMULATOR_IP_REGISTER_H

// instruction pointer register
struct ip_reg {
    union {
        uint16_t ip;
        uint32_t eip;
    };
};

#endif
