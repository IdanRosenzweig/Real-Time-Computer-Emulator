#ifndef EMULATOR_DIRECTORY_TABLE_H
#define EMULATOR_DIRECTORY_TABLE_H

// page directory entry
// points to a page table
struct page_directory_entry {
    uint32_t P: 1;
    uint32_t RW: 1;
    uint32_t US: 1;
    uint32_t PWT: 1;
    uint32_t PCD: 1;
    uint32_t A: 1;
    uint32_t : 1;
    uint32_t PS: 1;
    uint32_t G: 1;
    uint32_t : 3;
    uint32_t ptbl_base: 20;
};

#endif
