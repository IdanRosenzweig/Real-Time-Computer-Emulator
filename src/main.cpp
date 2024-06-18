#include <thread>
#include <cstring>

#include "common.hpp"

#include "hardware/cpu_i386/instr/x86_instruction.h"
#include "hardware/cpu_i386/instr/instr_decoder.h"
#include "hardware/cpu_i386/instr/instr_executer.h"
#include "hardware/cpu_i386/instr/setup.h"

#include "machine/machine.hpp"
#include "hardware/memory_loader.h"

#include "emulator/emulator.h"

#include <iostream>
using namespace std;

#ifndef RESET_VECTOR_FILE_PATH
#error provide reset vector file path
#endif

#ifndef BIOS_FILE_PATH
#error provide bios file path
#endif

int main(int argc, char **args) {
    setbuf(stdout, nullptr);
    setbuf(stderr, nullptr);

    if (argc != 2) {
        cout << "usage: ./emulator <file path to os floppy disk file>" << endl;
        return 1;
    }

    emulator_settings set = {

        .image_name = args[argc - 1],
        .load_addr = 0x0,
        .load_size = SIZE_MAX,

        // processor related settings
        .proc_set = {
#define MEMORY_SIZE (256 * MB)
            .mem_size = MEMORY_SIZE,

            // the reset vector is the first instructions that are executed once the cpu powers on
            // the reset vector code will be loaded at 0xffff:0x0 which is physical address of 0xffff0
#define INIT_IP_REG 0xfff0
#define INIT_CS_REG 0xf000
#define RESET_VECTOR_ADDR ((INIT_CS_REG << 4) + INIT_IP_REG)
            .init_ip_reg = INIT_IP_REG,
            .init_cs_reg = INIT_CS_REG
        },

        // ui related settings
        .ui_set = {
            .full = false,
            .vm = false,
        },
    };

    class emulator emulator(set);

    // loading the reset vector machine code to memory
    if (!load_to_memory(&emulator.machine.processor, RESET_VECTOR_FILE_PATH,
                        RESET_VECTOR_ADDR, 0)) {
        cerr << "can't load the reset vector code" << endl;
        return -1;
    }

    // loading BIOS code to memory
#define BIOS_ADDR 0xf0000
    if (!load_to_memory(&emulator.machine.processor, BIOS_FILE_PATH,
                        BIOS_ADDR, 0)) {
        cerr << "can't load the bios" << endl;
        return -1;
    }

    // inserting a floppy disk that contains the OS
    if (!emulator.machine.fdd->insert_disk(0, set.image_name)) {
        cerr << "can't load os image" << endl;
        return 1;
    }


    setup_instr_handlers(); // setup all opcodes handlers

    while (emulator.window.is_ui_alive()) {
        // clock cycle

        try {
            if (emulator.machine.processor.interrupts_system.chk_irq()) {
                emulator.machine.processor.halt = false;
            }

            if (emulator.machine.processor.halt) {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                continue;
            }

            // handle interrupt
            emulator.machine.processor.hundle_interrupt();

            // process next intsturction
            x86_instr_t instr;
            memset(&instr, 0, sizeof(instr));

            decode(&emulator.machine.processor, &instr); // decode
            execute(&emulator.machine.processor, &instr); // execute
        } catch (exception_t n) {
            cerr << "interrupt exception thrown\n";
            emulator.machine.processor.interrupts_system.queue_interrupt(interrupt{static_cast<uint8_t>(n), true});
            DEBUG_ERROR("Exception %d", n);
        } catch (...) {
            cerr << "exception thrown\n";
            emulator.machine.processor.dump_regs();
            emulator.window.stop_ui();
        }
    }
}
