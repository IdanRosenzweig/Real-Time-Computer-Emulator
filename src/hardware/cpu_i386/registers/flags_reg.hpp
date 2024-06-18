#ifndef EMULATOR_EFLAGS_REGISTER_H
#define EMULATOR_EFLAGS_REGISTER_H

#include "../../../common.hpp"

struct flags_reg {
public:
    union {
        uint32_t eflags;
        uint16_t flags;
        struct {
            uint32_t carry: 1;
            uint32_t : 1;           // 1
            uint32_t parity: 1;
            uint32_t : 1;           // 0
            uint32_t AF: 1;
            uint32_t : 1;           // 0
            uint32_t zero: 1;
            uint32_t sign: 1;
            uint32_t TF: 1;
            uint32_t interrupt: 1;
            uint32_t direction: 1;
            uint32_t overflow: 1;
            uint32_t IOPL: 2;
            uint32_t NT: 1;
            uint32_t : 1;           // 0
            uint32_t RF: 1;
            uint32_t VM: 1;
            uint32_t AC: 1;
            uint32_t VIF: 1;
            uint32_t VIP: 1;
            uint32_t ID: 1;
        };
    };

    template<class T>
    uint32_t update_reg_add(T v1, uint32_t v2, bool has_carry = false);

    template<class T>
    uint32_t update_reg_or(T v1, uint32_t v2);

    template<class T>
    uint32_t update_reg_and(T v1, uint32_t v2);

    template<class T>
    uint32_t update_reg_sub(T v1, uint32_t v2);

    template<class T>
    uint32_t update_reg_mul(T v1, uint32_t v2);

    template<class T>
    uint32_t update_reg_imul(T v1, int32_t v2);

    template<class T>
    uint32_t update_reg_shl(T v, uint8_t c);

    template<class T>
    uint32_t update_reg_shr(T v, uint8_t c);

private:
    bool chk_parity(uint8_t v);

};

#endif
