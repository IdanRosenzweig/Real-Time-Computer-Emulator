#ifndef EMULATOR_PTE_H
#define EMULATOR_PTE_H

// page table entry
// points to a physical page in memory
struct page_table_entry {
    uint32_t P: 1;
    uint32_t RW: 1;
    uint32_t US: 1;
    uint32_t PWT: 1;
    uint32_t PCD: 1;
    uint32_t A: 1;
    uint32_t D: 1;
    uint32_t PAT: 1;
    uint32_t G: 1;
    uint32_t : 3;
    uint32_t page_base: 20;
};

#endif
