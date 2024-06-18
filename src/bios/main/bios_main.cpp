#include "../common/common.h"


const char* msg = "bios main called\n";
const char* msg2 = "line number ";

EXTERNC
int bios_main(void) {
    write_string_cpp(msg);

    for (int i = 0; i < 3; i++) {
        write_string_cpp(msg2);
        write_char('0' + i);
        write_char('\n');
    }

//    scroll_lines(4);
//    write_string(msg);

    return 0;
}
