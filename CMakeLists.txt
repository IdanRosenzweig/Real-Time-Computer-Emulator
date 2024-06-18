cmake_minimum_required(VERSION 3.26)
project(ComputerEmulator)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

add_executable(emulator
        src/main.cpp

        src/emulator/emulator.cpp
        src/emulator/os_terminal_io.cpp
        src/emulator/window.cpp

        src/hardware/floppy_disk/floppy_disk_driver.cpp

        src/hardware/cpu_i386/cpu_i386.cpp
        src/hardware/cpu_i386/registers/registers.cpp
        src/hardware/cpu_i386/registers/flags_reg.cpp
        src/hardware/cpu_i386/ram/ram.cpp
        src/hardware/cpu_i386/pit/pit.cpp
        src/hardware/cpu_i386/pic/pic.cpp
        src/hardware/cpu_i386/paging/paging_system.cpp
        src/hardware/cpu_i386/io/io.cpp
        src/hardware/cpu_i386/interrupts/interrupts_system.cpp
        src/hardware/cpu_i386/a20_gate/a20_gate_controller.cpp

        src/hardware/ps2_controller/ps2_controller.cpp
        src/hardware/ps2_controller/keyboard.cpp
        src/hardware/ps2_controller/mouse.cpp

        src/hardware/vga/vga.cpp
        src/hardware/vga/attr/attr.cpp
        src/hardware/vga/crt/crt.cpp
        src/hardware/vga/dac/dac.cpp
        src/hardware/vga/gc/gc.cpp
        src/hardware/vga/seq/sequencer.cpp

        src/hardware/memory_loader.cpp

        src/hardware/cpu_i386/instr/instr_decoder.cpp
        src/hardware/cpu_i386/instr/instr_executer.cpp
        src/hardware/cpu_i386/instr/instr_handlers.cpp
        src/hardware/cpu_i386/instr/setup.cpp
        src/hardware/cpu_i386/instr/helper_funcs.cpp

        src/machine/machine.cpp

)
target_compile_definitions(emulator PUBLIC RESET_VECTOR_FILE_PATH=${RESET_VECTOR_FILE_PATH})
target_compile_definitions(emulator PUBLIC BIOS_FILE_PATH=${BIOS_FILE_PATH})

# glfw
target_link_libraries(emulator glfw)
find_package(OpenGL REQUIRED)

# opengl
target_link_libraries(emulator OpenGL::GL)