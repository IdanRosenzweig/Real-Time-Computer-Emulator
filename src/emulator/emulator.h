#ifndef EMULATOR_EMULATOR_H
#define EMULATOR_EMULATOR_H

#include "window.hpp"
#include "os_terminal_io.hpp"
#include "../machine/machine.hpp"

struct emulator_settings {
    const char *image_name;
    uint32_t load_addr;
    size_t load_size;

    processor_settings proc_set;
    window_setting ui_set;
};

class emulator {
public:
    // the hardware machine
    class machine machine;

    // the emulator class has 3 uses:
    // 1. connects a screen to the vga output of the machine (displays the screen in a small emulator)
    // 2. connects a keyboard to the machine (when emulator is in focus)
    // 3. connects a mouse to the machine (when emulator is in focus)
    class window window;

    // OS-terminal stdin/stdout
    // connected (as a device) to the io of the machine
    os_terminal_io terminal_io;

    emulator(const emulator_settings& set);

};


#endif
