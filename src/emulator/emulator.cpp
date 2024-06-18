#include "emulator.h"

emulator::emulator(const emulator_settings& set) :
        machine(set.proc_set),
        window(set.ui_set, &machine.ps2Controller, &machine.vga),
        terminal_io() {

    // connect the terminal device
    machine.processor.io.pmio_manager.assign_port_range(0x3f8, 1, &terminal_io); // using serial port 0x3f8

}
