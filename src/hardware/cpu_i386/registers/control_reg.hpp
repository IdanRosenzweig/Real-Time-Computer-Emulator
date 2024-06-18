#ifndef EMULATOR_CONTROL_REGISTER_H
#define EMULATOR_CONTROL_REGISTER_H

#include "../../../common.hpp"

struct control_reg {
public:
    union {
        uint32_t raw;
        struct {
            uint32_t PE: 1;
            uint32_t MP: 1;
            uint32_t EM: 1;
            uint32_t TS: 1;
            uint32_t ET: 1;
            uint32_t NE: 1;
            uint32_t : 10;
            uint32_t WP: 1;
            uint32_t : 1;
            uint32_t AM: 1;
            uint32_t : 10;
            uint32_t NW: 1;
            uint32_t CD: 1;
            uint32_t PG: 1;
        };
    } cr0;

    union {
        uint32_t raw;
    } cr1;

    union {
        uint32_t raw;
    } cr2;

    union {
        uint32_t raw;
        struct {
            uint32_t : 3;
            uint32_t PWT: 1;
            uint32_t PCD: 1;
            uint32_t : 7;
            uint32_t PageDirBase: 20;
        };
    } cr3;

    union {
        uint32_t raw;
        struct {
            uint32_t VME: 1;
            uint32_t PVI: 1;
            uint32_t TSD: 1;
            uint32_t DE: 1;
            uint32_t PSE: 1;
            uint32_t PAE: 1;
            uint32_t MCE: 1;
            uint32_t PGE: 1;
            uint32_t PCE: 1;
            uint32_t OSFXSR: 1;
            uint32_t OSXMMEXCPT: 1;
            uint32_t : 21;
        };
    } cr4;

private:
    uint32_t *cr[5];

public:
    control_reg() {
        cr[0] = &cr0.raw;
        cr[1] = &cr1.raw;
        cr[2] = &cr2.raw;
        cr[3] = &cr3.raw;
        cr[4] = &cr4.raw;

        for (int i = 0; i < 5; i++)
            *cr[i] = 0;
    }

    inline uint32_t get_crn(uint8_t n) {
        if (n >= sizeof(cr)) DEBUG_ERROR("");
        return *cr[n];
    }

    inline void set_crn(uint8_t n, uint32_t v) {
        if (n >= sizeof(cr)) DEBUG_ERROR("");
        *cr[n] = v;
    }

    inline bool is_protected(void) {
        return cr0.PE;
    }

    inline bool is_paging_enabled(void) {
        return cr0.PG;
    }

    inline uint32_t get_page_directory_base(void) {
        return cr3.PageDirBase;
    }
};

#endif
