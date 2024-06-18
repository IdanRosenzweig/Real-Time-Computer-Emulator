#include "memory_loader.h"

#include <iostream>
using namespace std;

bool load_to_memory(cpu_i386 *proc, const char *fname, uint32_t addr, uint32_t offset) {
    FILE *fp = fopen(fname, "rb");
    if (fp == nullptr) {
        cerr << "can't open file to load to memory" << endl;
        return false;
    }

    fseek(fp, 0, SEEK_END);
    size_t size = ftell(fp);

    uint8_t *buf = new uint8_t[size]; // bytes buffer

    fseek(fp, offset, SEEK_SET);
    fread(buf, 1, size, fp);
    fclose(fp);

    proc->memory.write_data(addr, buf, size);

    delete[] buf;

    return true;
}
