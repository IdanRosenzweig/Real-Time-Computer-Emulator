#ifndef EMULATOR_MEMORY_LOADER_H
#define EMULATOR_MEMORY_LOADER_H

#include "cpu_i386/cpu_i386.hpp"

bool load_to_memory(cpu_i386 *proc, const char *fname, uint32_t addr, uint32_t offset);


#endif
