#include "../common/common.h"

EXTERNC
void fdd_configure_cpp(void) {
    out_port(0x3f5, 0x13);

    out_port(0x3f5,  0x00);
    out_port(0x3f5,  0x40);
    out_port(0x3f5,  0x00);

    disable_intr();

    unsigned char in = in_port(0x21);
    in &= 0xbf;
    out_port(0x21, in);

    enable_intr();

}