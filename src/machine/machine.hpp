#ifndef EMULATOR_MACHINE_H
#define EMULATOR_MACHINE_H

#include "../common.hpp"

#include "../hardware/cpu_i386/cpu_i386.hpp"

#include "../hardware/cpu_i386/a20_gate/a20_gate_controller.hpp"

#include "../hardware/cpu_i386/pic/pic.hpp"
#include "../hardware/cpu_i386/pit/pit.hpp"

#include "../hardware/vga/vga.hpp"
#include "../hardware/floppy_disk/floppy_disk_driver.hpp"
#include "../hardware/ps2_controller/ps2_controller.h"

class machine {
public:
    // the main cpu
    class cpu_i386 processor;

    // floppy disk driver
    class floppy_disk_driver *fdd;

    // keyboard & mouse
    ps2_controller ps2Controller;

    // vga and screen
    class vga vga;

    machine(processor_settings proc_set);
};

#endif
