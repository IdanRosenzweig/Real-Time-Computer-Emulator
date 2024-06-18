#ifndef EMULATOR_PAGING_SYSTEM_H
#define EMULATOR_PAGING_SYSTEM_H

#include <stdint.h>

#include "page_directory.h"
#include "page_table.h"

#include "access_mode.h"

#include "../ram/ram.hpp"
#include "../registers/registers.hpp"

#include <vector>

class paging_system {
private:
    registers *regs;
    ram *memory;

    std::vector<page_table_entry *> cache;

public:
    paging_system(registers *regs, ram *mem) : regs(regs), memory(mem) {
    }

    bool search_cached_page(uint32_t vpn, page_table_entry *res);

    void cache_page(uint32_t vpn, page_table_entry pte);

    uint32_t translate_addr(acsmode_t mode, uint32_t addr);
};


#endif
