#include "../common/common.h"
#include "../../hardware/cpu_i386/ivt/ivt_entry.h"

void set_ivt_entry(int n, uint32_t offset, uint16_t cs) {
    struct ivt_entry *ivt = (struct ivt_entry *) IVT_ADDR;

    write_es_word(&(ivt[n].offset), offset);
    write_es_word(&(ivt[n].segment), cs);
}

EXTERNC
void setup_ivt(void) {
    // assuming the load address of BIOS will be 0xf000:0x0 (physical address)
    set_ivt_entry(0x10, (uint32_t) bsv_video, 0xf000);
    set_ivt_entry(0x13, (uint32_t) bsv_disk, 0xf000);
    set_ivt_entry(0x16, (uint32_t) bsv_keyboard, 0xf000);
    set_ivt_entry(0x26, (uint32_t) bsv_irq_disk, 0xf000);
}