#include "paging_system.h"

#include "../exceptions/exceptions.h"

bool paging_system::search_cached_page(uint32_t vpn, page_table_entry *res) {
    if (!(vpn < cache.size()) || cache[vpn] == nullptr)
        return false;

    DEBUG_ASSERT(res);
    *res = *cache[vpn];

    return true;
}

void paging_system::cache_page(uint32_t vpn, page_table_entry pte) {
    if (!(vpn < cache.size()))
        cache.resize(vpn + 1, nullptr);

    cache[vpn] = new page_table_entry;
    *cache[vpn] = pte;
}

uint32_t paging_system::translate_addr(acsmode_t mode, uint32_t addr) {
    CPU_EXCEPTION(EXP_GP, !regs->is_protected());

    uint8_t cpl = regs->get_sgreg(CS) & 3;

    uint32_t vpn = addr >> 12;
    uint16_t offset = addr & ((1 << 12) - 1);

    page_table_entry pte;
    if (!search_cached_page(vpn, &pte)) {
//        if (true) {

        uint16_t pdir_index = addr >> 22;
        uint16_t ptbl_index = (addr >> 12) & ((1 << 10) - 1);

        uint32_t pdir_base = regs->control_reg.get_page_directory_base() << 12;

        page_directory_entry pde;
        memory->read_data(&pde, pdir_base + pdir_index * sizeof(page_directory_entry), sizeof(page_directory_entry));

        CPU_EXCEPTION_DO(EXP_PF, !pde.P, regs->control_reg.cr2.raw = addr);
        CPU_EXCEPTION_DO(EXP_PF, !pde.RW && mode == MODE_WRITE, regs->control_reg.cr2.raw = addr);
        CPU_EXCEPTION_DO(EXP_PF, !pde.US && cpl > 2, regs->control_reg.cr2.raw = addr);

        uint32_t ptbl_base = pde.ptbl_base << 12;
        memory->read_data(&pte, ptbl_base + ptbl_index * sizeof(page_table_entry), sizeof(page_table_entry));
        cache_page(vpn, pte);

    }

    CPU_EXCEPTION_DO(EXP_PF, !pte.P, regs->control_reg.cr2.raw = addr);
    CPU_EXCEPTION_DO(EXP_PF, !pte.RW && mode == MODE_WRITE, regs->control_reg.cr2.raw = addr);
    CPU_EXCEPTION_DO(EXP_PF, !pte.US && cpl > 2, regs->control_reg.cr2.raw = addr);

    return (pte.page_base << 12) + offset;
}