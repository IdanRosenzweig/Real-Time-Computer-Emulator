#include "interrupts_system.h"

void interrupts_system::queue_interrupt(struct interrupt intr) {
    que.push(intr);
}

bool interrupts_system::chk_irq(void) {
    if (!regs->flags_reg.interrupt)
        return false;

    if (!pic_m.chk_intr_req())
        return false;

    int8_t n_intr = pic_m.get_next_intr_num();
    if (n_intr == SLAVE_INTR) // interrupt is from the slave pic and not the master pic
        n_intr = pic_s.get_next_intr_num();

//    cout << "new interrupt chk and queued\n";
    queue_interrupt(interrupt{n_intr, true});
    return true;
}