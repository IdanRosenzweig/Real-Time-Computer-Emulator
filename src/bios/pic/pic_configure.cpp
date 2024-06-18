#include "../common/common.h"

EXTERNC
void pic_configure(void) {
    disable_intr();

    out_port(0x20, 0x11);
    out_port(0xa0, 0x11);

    out_port(0x21, 0x20);
    out_port(0xa1, 0x28);

    out_port(0x21, 4);
    out_port(0xa1, 2);

    out_port(0x21, 1);
    out_port(0xa1, 1);

    out_port(0x21, 0xfb);
    out_port(0xa1, 0xff);

    enable_intr();
}