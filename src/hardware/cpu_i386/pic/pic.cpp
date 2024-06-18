#include <stdint.h>
#include "pic.hpp"

#include <iostream>

using namespace std;;

pic::pic(pic *master) {
    pic_m = master;

    for (int i = 0; i < NO_IRQ_LINES; i++)
        irq[i] = nullptr;

    irr = 0;
    isr = 0;

    init_icw_cnt = INIT_ICW_NOTSTARTED;
}

bool pic::poll_next_irq() {
    if (init_icw_cnt != INIT_ICW_FINISHED)
        return false;

    int i = 0;
    while (i < NO_IRQ_LINES
           && (!has_device(i)           // no device connected at the line
               || chk_imr_on_line(i)   // line is masked as disabled
               || !irq[i]->chk_intr_req())  // device doesn't raise interrupt
            )
        i++;

    if (i == NO_IRQ_LINES)
        return false;

    if (isr && (1 << i) >= isr)
        return false;

    set_irr_on_line(i, true);

    return true;
}

bool pic::chk_intr_req(void) {
    return poll_next_irq();
}

int8_t pic::get_next_intr_num(void) {
    int line = 0;
    while (line < NO_IRQ_LINES && !chk_irr_on_line(line))
        line++;

    if (line == NO_IRQ_LINES) return -1;



    if (!icr.icw4.AEOI)
        set_isr_on_line(line, true);

    set_irr_on_line(line, false);

    if (!icr.icw1.SNGL && !has_master() && chk_slave_on_line(line)) // interrupt was raised by a slave pic
        return SLAVE_INTR;

    if (has_master()
        && (pic_m->icr.icw1.SNGL || pic_m->has_master() || !pic_m->chk_slave_on_line(icr.icw3.ID)))
        DEBUG_ERROR("");

    // interrupt was raised by a device
    uint8_t intr_nums_off = icr.icw4.PM ? icr.icw2.IVA_x86 << 3 : icr.icw1.IVA_l + (icr.icw2.IVA_h << 3);
    return intr_nums_off + line;
}


uint8_t pic::read_byte_from_port(uint16_t addr) {
    switch (addr) {
        case pic_input_codes::MASTER_DATA_PORT: // in case this is the master pic
        case pic_input_codes::SLAVE_DATA_PORT: // in case this is the slave pic
            return imr;

        default:
            cout << "read from unused pic io port\n";
            return 0;
    }
}

void pic::write_byte_to_port(uint16_t addr, uint8_t v) {
    switch (addr) {
        // command port
        case pic_input_codes::MASTER_COMMAND: // in case this is the master pic
        case pic_input_codes::SLAVE_COMMAND: // in case this is the slave pic
            send_command(v);
            break;

            // data port
        case pic_input_codes::MASTER_DATA_PORT: // in case this is the master pic
        case pic_input_codes::SLAVE_DATA_PORT: // in case this is the slave pic
            send_data(v);
            break;

    }
}


void pic::send_command(uint8_t v) {
    if (init_icw_cnt == INIT_ICW_NOTSTARTED) {
        // the first command sent starts the pic's initialization process
        icr.icw1.raw = v;
        init_icw_cnt = 2;
        return;
    }

    // operation command word 2
    struct ocw2 ocw2;

    ocw2.raw = v;
    if (ocw2.EOI) {
        if (ocw2.SL) {
            // reset specific line for the isr
            set_isr_on_line(ocw2.L, false);

        } else {
            // reset the highest priority line in the isr
            // (this command was sent by the interrupt handler when it finished)
            int line = 0;
            while (line < NO_IRQ_LINES && !chk_isr_on_line(line))
                line++;

            if (line < NO_IRQ_LINES) set_isr_on_line(line, false);

        }
    }

    // todo ocw1 and ocw3

}

void pic::send_data(uint8_t v) {
//    if (INIT_ICW_FINISHED < init_icw_cnt) {
    if (init_icw_cnt != INIT_ICW_FINISHED // the initialization process hasn't finished yet
        && init_icw_cnt != INIT_ICW_NOTSTARTED) // but it has started
    {
        switch (init_icw_cnt++) {
            case 2:
                icr.icw2.raw = v;
                if (icr.icw1.SNGL)
                    goto done;
                break;

            case 3:
                icr.icw3.raw = v;
                if (!icr.icw1.IC4)
                    goto done;
                break;

            case 4:
                icr.icw4.raw = v;
            default:
            done:
                init_icw_cnt = INIT_ICW_FINISHED;
                for (int i = 0; i < NO_IRQ_LINES; i++)
                    if (has_device(i)) irq[i]->chk_intr_req();
        }
        return;
    }

    // sending data to the data port casually just sets the imr
    imr = v;
}

