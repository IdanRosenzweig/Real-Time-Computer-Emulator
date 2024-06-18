#ifndef EMULATOR_COMMON_H
#define EMULATOR_COMMON_H

#include <cstdint>
#include <type_traits>
#include <iostream>

#define KB  1024
#define MB  (KB * 1024)
#define GB  (MB * 1024)

template<int NO_BITS>
using data_t = typename std::conditional_t<
        NO_BITS == 8, uint8_t,
        typename std::conditional_t<NO_BITS == 16, uint16_t, typename std::conditional_t<NO_BITS == 32, uint32_t,
                uint64_t> > >;


#define DEBUG_ASSERT(cond)          { if(!(cond)) {printf("assertion failed (%s).\nfile: (%s), line: (%d), function: (%s)\n", #cond, __FILE__, __LINE__, __FUNCTION__); exit(-1);} }
#define DEBUG_ERROR(fmt, ...)       { printf("error.\nfile: (%s), line: (%d), function: (%s)\n", __FILE__, __LINE__, __FUNCTION__); printf(fmt, ##__VA_ARGS__); exit(-1); }

#endif
