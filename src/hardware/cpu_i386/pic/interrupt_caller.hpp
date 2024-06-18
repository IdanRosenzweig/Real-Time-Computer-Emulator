#ifndef EMULATOR_INTERRUPT_CALLER_H
#define EMULATOR_INTERRUPT_CALLER_H

#include "../../../common.hpp"

class basic_interrupt_caller {
public:
    virtual bool chk_intr_req(void) = 0;
};

class device_interrupt_caller : public basic_interrupt_caller {
protected:
    bool intr_req;

public:
    device_interrupt_caller(void) : intr_req(false) {}

    bool chk_intr_req(void) override {
        if (intr_req) {
            intr_req = false;
            return true;
        } else
            return false;
    }
};

#endif
