#include "flags_reg.hpp"

template uint32_t flags_reg::update_reg_add(uint32_t v1, uint32_t v2, bool has_carry);

template uint32_t flags_reg::update_reg_add(uint16_t v1, uint32_t v2, bool has_carry);

template uint32_t flags_reg::update_reg_add(uint8_t v1, uint32_t v2, bool has_carry);

template<class T>
uint32_t flags_reg::update_reg_add(T v1, uint32_t v2, bool has_carry) {
    uint8_t size = sizeof(T) * 8; // number of bits

    v2 = (T) v2;

    uint64_t result = (uint64_t) v1 + v2 + has_carry;

    bool s1 = v1 >> (size - 1);
    bool s2 = v2 >> (size - 1);
    bool sr = (result >> (size - 1)) & 1;

    carry = (result >> size);
    parity = (chk_parity(result & 0xff));
    zero = (!result);
    sign = (sr);
    overflow = (!(s1 ^ s2) && s1 ^ sr);

    return eflags;
}

template uint32_t flags_reg::update_reg_or(uint32_t v1, uint32_t v2);

template uint32_t flags_reg::update_reg_or(uint16_t v1, uint32_t v2);

template uint32_t flags_reg::update_reg_or(uint8_t v1, uint32_t v2);

template<class T>
uint32_t flags_reg::update_reg_or(T v1, uint32_t v2) {
    T result;
    uint8_t size;

    v2 = (T) v2;
    result = v1 | v2;
    size = sizeof(T) * 8;

    carry = (0);
    parity = (chk_parity(result & 0xff));
    zero = (!result);
    sign = ((result >> (size - 1)) & 1);
    overflow = (0);

    return eflags;
}

template uint32_t flags_reg::update_reg_and(uint32_t v1, uint32_t v2);

template uint32_t flags_reg::update_reg_and(uint16_t v1, uint32_t v2);

template uint32_t flags_reg::update_reg_and(uint8_t v1, uint32_t v2);

template<class T>
uint32_t flags_reg::update_reg_and(T v1, uint32_t v2) {
    T result;
    uint8_t size;

    v2 = (T) v2;
    result = v1 & v2;
    size = sizeof(T) * 8;

    carry = (0);
    parity = (chk_parity(result & 0xff));
    zero = (!result);
    sign = ((result >> (size - 1)) & 1);
    overflow = (0);

    return eflags;
}

template uint32_t flags_reg::update_reg_sub(uint32_t v1, uint32_t v2);

template uint32_t flags_reg::update_reg_sub(uint16_t v1, uint32_t v2);

template uint32_t flags_reg::update_reg_sub(uint8_t v1, uint32_t v2);

template<class T>
uint32_t flags_reg::update_reg_sub(T v1, uint32_t v2) {
    bool s1, s2, sr;
    uint64_t result;
    uint8_t size;

    v2 = (T) v2;
    result = (uint64_t) v1 - v2;
    size = sizeof(T) * 8;

    s1 = v1 >> (size - 1);
    s2 = v2 >> (size - 1);
    sr = (result >> (size - 1)) & 1;

    carry = (result >> size);
    parity = (chk_parity(result & 0xff));
    zero = (!result);
    sign = (sr);
    overflow = (s1 ^ s2 && s1 ^ sr);

    return eflags;
}

template uint32_t flags_reg::update_reg_mul(uint32_t v1, uint32_t v2);

template uint32_t flags_reg::update_reg_mul(uint16_t v1, uint32_t v2);

template uint32_t flags_reg::update_reg_mul(uint8_t v1, uint32_t v2);

template<class T>
uint32_t flags_reg::update_reg_mul(T v1, uint32_t v2) {
    uint64_t result;
    uint8_t size;

    v2 = (T) v2;
    result = (uint64_t) v1 * v2;
    size = sizeof(T) * 8;

    carry = (result >> size);
    overflow = (result >> size);

    return eflags;
}

template uint32_t flags_reg::update_reg_imul(int32_t v1, int32_t v2);

template uint32_t flags_reg::update_reg_imul(int16_t v1, int32_t v2);

template uint32_t flags_reg::update_reg_imul(int8_t v1, int32_t v2);

template<class T>
uint32_t flags_reg::update_reg_imul(T v1, int32_t v2) {
    int64_t result;
    uint8_t size;

    v2 = (T) v2;
    result = (int64_t) v1 * v2;
    size = sizeof(T) * 8;

    carry = ((result >> size) != -1);
    overflow = ((result >> size) != -1);

    return eflags;
}

template uint32_t flags_reg::update_reg_shl(uint32_t v, uint8_t c);

template uint32_t flags_reg::update_reg_shl(uint16_t v, uint8_t c);

template uint32_t flags_reg::update_reg_shl(uint8_t v, uint8_t c);

template<class T>
uint32_t flags_reg::update_reg_shl(T v, uint8_t c) {
    T result;
    uint8_t size;

    result = v << c;
    size = sizeof(T) * 8;

    carry = ((v >> (size - c)) & 1);
    parity = (chk_parity(result & 0xff));
    zero = (!result);
    sign = ((result >> (size - 1)) & 1);
    if (c == 1)
        overflow = (((v >> (size - 1)) & 1) ^ ((v >> (size - 2)) & 1));

    return eflags;
}

template uint32_t flags_reg::update_reg_shr(uint32_t v, uint8_t c);

template uint32_t flags_reg::update_reg_shr(uint16_t v, uint8_t c);

template uint32_t flags_reg::update_reg_shr(uint8_t v, uint8_t c);

template<class T>
uint32_t flags_reg::update_reg_shr(T v, uint8_t c) {
    T result;
    uint8_t size;

    result = v >> c;
    size = sizeof(T) * 8;

    carry = ((v >> (c - 1)) & 1);
    parity = (chk_parity(result & 0xff));
    zero = (!result);
    sign = ((result >> (size - 1)) & 1);
    if (c == 1)
        overflow = ((v >> (size - 1)) & 1);

    return eflags;
}

bool flags_reg::chk_parity(uint8_t v) {
    bool p = true;

    for (int i = 0; i < 8; i++)
        p ^= (v >> i) & 1;

    return p;
}
