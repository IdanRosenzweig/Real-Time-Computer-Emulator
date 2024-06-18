#include <stdio.h>
#include "machine.hpp"

machine::machine(processor_settings proc_set) :
        processor(proc_set),
//        floppy_disk(),
//        kb(&processor.memory),
        ps2Controller(&processor.memory),
        vga() {


    // pic master
    processor.io.pmio_manager.assign_port_range(0x020, 2, &processor.interrupts_system.pic_m); // 0x20, 0x21


    // pic slave
    processor.io.pmio_manager.assign_port_range(0x0a0, 2, &processor.interrupts_system.pic_s); // 0xa0, 0xa1
    processor.interrupts_system.pic_m.setup_interrupt_line(2, &processor.interrupts_system.pic_s);


    // pit
    processor.io.pmio_manager.assign_port_range(0x040, 4, &processor.pit);
    processor.interrupts_system.pic_m.setup_interrupt_line(0, &processor.pit);


    // floppy disk drivers
    fdd = new class floppy_disk_driver();
    processor.io.pmio_manager.assign_port_range(0x3f0, 8, fdd);
    processor.interrupts_system.pic_m.setup_interrupt_line(6, fdd);


    // a20 gate controller
    processor.io.pmio_manager.assign_port_range(0x092, 1, &processor.memory.a20_controller);


    // assign interrupt line for keyboard
    processor.interrupts_system.pic_m.setup_interrupt_line(1, &ps2Controller.keyboard);
    // assign interrupt line for mouse
    processor.interrupts_system.pic_s.setup_interrupt_line(4, &ps2Controller.mouse);

    // ps/2 controller (handles keyboard and mouse)
    processor.io.pmio_manager.assign_port_range(0x060, 1, &ps2Controller);        // 0x60
    processor.io.pmio_manager.assign_port_range(0x064, 1, &ps2Controller);        // 0x64


    // vga
    processor.io.pmio_manager.assign_port_range(0x3b4, 2, &vga.crt);    // 0x3b4, 0x3b5
    processor.io.pmio_manager.assign_port_range(0x3ba, 1, &vga);        // 0x3ba
    processor.io.pmio_manager.assign_port_range(0x3c0, 2, &vga.attr);    // 0x3c0, 0x3c1
    processor.io.pmio_manager.assign_port_range(0x3c2, 2, &vga);        // 0x3c2, 0x3c3
    processor.io.pmio_manager.assign_port_range(0x3c4, 2, &vga.seq);    // 0x3c4, 0x3c5
    processor.io.pmio_manager.assign_port_range(0x3c6, 4, &vga.dac);    // 0x3c6, 0x3c7, 0x3c8, 0x3c9
    processor.io.pmio_manager.assign_port_range(0x3cc, 1, &vga);        // 0x3cc
    processor.io.pmio_manager.assign_port_range(0x3ce, 2, &vga.gc);    // 0x3ce, 0x3cf
    processor.io.pmio_manager.assign_port_range(0x3d4, 2, &vga.crt);    // 0x3d4, 0x3d5
    processor.io.pmio_manager.assign_port_range(0x3da, 1, &vga);        // 0x3da

    processor.io.mmio_manager.assign_mem_region(0xa0000, 0x20000, &vga);

}
