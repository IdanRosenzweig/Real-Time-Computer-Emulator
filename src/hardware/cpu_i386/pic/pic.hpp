#ifndef EMULATOR_PIC_H
#define EMULATOR_PIC_H

#include "../../../common.hpp"
#include "interrupt_caller.hpp"
#include "../io/port_mapped_io.h"

#include "pic_input_codes.h"
#include "pic_output_codes.h"

#define NO_IRQ_LINES 8


// programmable interrupt controller
class pic : public basic_interrupt_caller, public pmio {
private:
    // ptr to master pic (if exists)
    pic *pic_m;

    inline bool has_master() { return pic_m != nullptr; }


    // interrupt lines
    basic_interrupt_caller *irq[NO_IRQ_LINES];

    // return true whether a device is connected to the irq line
    inline bool has_device(int line) {
        return irq[line] != nullptr;
    }


    // interrupt mask register
    // each bit corresponds to a specific irq line,
    // allowing to enable or disable interrupts from that line
    uint8_t imr;

    // returns the mask status of the line
    inline bool chk_imr_on_line(int line) {
        return (imr >> line) & 0b1;
    }


    // interrupt request register
    // each bit corresponds to a specific irq line,
    // and indicates whether there is a request pending on that line
    uint8_t irr;

    // returns the status of the irq line
    inline bool chk_irr_on_line(int line) {
        return (irr >> line) & 0b1;
    }

    // set the status of the irq on the line (pending or not pending)
    inline void set_irr_on_line(int line, bool pending) {
        if (pending) irr |= (1 << line);
        else irr &= ~(1 << line);
    }


    // in service register
    // each bit corresponds to a specific irq line
    // indicates which interrupts are in progress and which has not been ack
    uint8_t isr;

    // returns the status of the isr regarding the line
    inline bool chk_isr_on_line(int line) {
        return (isr >> line) & 0b1;
    }

    // set the status of the isr regarding the line (in progress or not)
    inline void set_isr_on_line(int line, bool status) {
        if (status) isr |= (1 << line);
        else isr &= ~(1 << line);
    }


    // initialization command register
    // contains 4 sub registers (initialization control words)
    struct {

        union {
            uint8_t raw;
            struct {
                uint8_t IC4: 1;

                // indicate whether the pic operates as single
                // or in cascading mode (multiple pics connected)
                uint8_t SNGL: 1;

                uint8_t ADI: 1;
                uint8_t LTIM: 1;
                uint8_t : 1;
                uint8_t IVA_l: 3;    // MCS-80/85
            };
        } icw1;

        // icw2 contains the offset for this pic's interrupt
        // numbers in the interrupt vector table.
        // if this value is x, then the first interrupt generated
        // from this pic would become interrupt number x at the IVT,
        // the second would become interrupt number x+1, third x+2...
        union {
            uint8_t raw;
            struct {
                uint8_t IVA_h: 3;
                uint8_t IVA_x86: 5;
            };
        } icw2;

        // icw3 is used by a master pic to determine for each of its
        // IRQ lines whether it is cascaded to a slave pic
        // (each bit corresponds to one IRQ line)
        union {
            uint8_t raw;
            struct {
                uint8_t S0: 1;
                uint8_t S1: 1;
                uint8_t S2: 1;
                uint8_t S3: 1;
                uint8_t S4: 1;
                uint8_t S5: 1;
                uint8_t S6: 1;
                uint8_t S7: 1;
            };

            struct {
                uint8_t ID: 3;
            };
        } icw3;

        union {
            uint8_t raw;
            struct {
                // specifies operating mode
                uint8_t PM: 1;
                // enable/disable automatic end-of-interrupt mode
                uint8_t AEOI: 1;
                // specifies whether the pic is in slave mode or master mode
                uint8_t MS: 1;
                uint8_t BUF: 1;
                uint8_t SFNM: 1;
            };
        } icw4;

    } icr;

    // the first byte sent to the command port starts the initialization process of the pic.
    // after that, the next NO_ICWS-1 bytes sent to the data port
    // will initialize the rest of the words in the icr.
    // this variable refers to the next word in the icr
    // that would be initialized in the process (the words are 1-indexed)
    int init_icw_cnt;

    // indicates the initialization process hasn't started yet
#define INIT_ICW_NOTSTARTED (-1)
    // indicate the initialization process has finished
#define INIT_ICW_FINISHED (0)

    // after the initialization process has finished, the pic reads
    // operation command words (OCWs)
    // todo ocw1
    struct ocw2 {
        union {
            uint8_t raw;
            struct {
                uint8_t L: 3;
                uint8_t : 2;
                uint8_t EOI: 1;
                uint8_t SL: 1;
                uint8_t R: 1;
            };
        };
    };
    // todo ocw3

public:
    pic(pic *master = NULL);

private:

    // check whether a slave pic is connected to line
    inline bool chk_slave_on_line(int line) {
        return icr.icw3.raw & (1 << line);
    }

public:
    void setup_interrupt_line(uint8_t line, basic_interrupt_caller *dev) {
        if (line < NO_IRQ_LINES) irq[line] = dev;
        else
            DEBUG_ERROR("interrupt line out of bound : %d", line);
    };

    // polls and check if some device (or slave pic) generates interrupt.
    // if so return true and mark the irq line in the irr
    bool poll_next_irq(void);

    // call poll_next_irq()
    bool chk_intr_req(void) override;

    // assuming some irq line is marked
    // if the irq line is connected to a device, return the interrupt number (at the IVT) that is generated by the line
    // if the ieq line is onnceted to a slave pic, return SLAVE_INTR
#define SLAVE_INTR (-1)

    int8_t get_next_intr_num(void);

    uint8_t read_byte_from_port(uint16_t addr) override;

    void write_byte_to_port(uint16_t addr, uint8_t v) override;

private:

    // handle data received in the command port
    void send_command(uint8_t v);

    // handle data received in the data port
    void send_data(uint8_t v);
};

#endif
