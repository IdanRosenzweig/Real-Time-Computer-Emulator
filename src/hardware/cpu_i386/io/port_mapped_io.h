#ifndef EMULATOR_PORT_MAPPED_IO_H
#define EMULATOR_PORT_MAPPED_IO_H

#include <stdint.h>
#include "../../../common.hpp"

// port mapped io (PMIO)
// is a method of transferring data to external devices by associating (mapping) port numbers to certain devices.
// in order to communicate with a device, you can read (input) or write (output) to its port numbers.
class pmio {
public:
    virtual ~pmio() {};


    template<int NO_BITS>
    data_t<NO_BITS> read_from_port(uint16_t port) {
        data_t<NO_BITS> v = 0;
        for (int i = 0; i < sizeof(v); i++)
            v += read_byte_from_port(port + i) << (8 * i);
        return v;
    };

    // read a single byte from the port
    virtual uint8_t read_byte_from_port(uint16_t addr) = 0;


    template<int NO_BITS>
    void write_to_port(uint16_t port, data_t<NO_BITS> v) {
        for (int i = 0; i < sizeof(v); i++) {
            write_byte_to_port(port + i, (v >> (8 * i)) & 0xff);
        }
    };

    // write a single byte to the port (at addr)
    virtual void write_byte_to_port(uint16_t addr, uint8_t v) = 0;

};

#include <map>
#include <utility>

class pmio_manager {
protected:
    std::map<uint16_t, std::pair<size_t, pmio *>> regions;

public:
    void assign_port_range(uint16_t port, size_t len, pmio *handler) {
        regions[port] = {len, handler};
    }

    pmio *find_handler_at_port(uint16_t port) {
        if (regions.empty()) return nullptr;

        auto it = regions.upper_bound(port);
        if (it == regions.begin()) return nullptr;

        it--;
        uint16_t curr_port = it->first;
        size_t curr_sz = it->second.first;


        // make sure the searched port is within the range [curr_port, curr_port + curr_sz)
        // (the searched port is already larger/equal than curr_port, because of the binary search)
        if (!(port < curr_port + curr_sz))
            return nullptr;

        return it->second.second;
    }


//    uint32_t in_io32(uint16_t port) {
//        uint32_t v = 0;
//
//        for (int i = 0; i < 4; i++)
//            v += in_io8(port + i) << (8 * i);
//        return v;
//    }
//
//    uint16_t in_io16(uint16_t port) {
//        uint16_t v = 0;
//
//        for (int i = 0; i < 2; i++)
//            v += in_io8(port + i) << (8 * i);
//        return v;
//    }
//
//    uint8_t in_io8(uint16_t port) {
//        pmio *handler = find_handler_at_port(port);
//        if (handler == nullptr) {
//            DEBUG_ERROR("no device mapped to port : 0x%04x", port);
//            return 0;
//        }
//
//        uint8_t v = handler->read_byte_from_port(port);
//        DEBUG_INFO( "in [0x%04x] (0x%04x)", port, v);
//        return v;
//    }


    template<int NO_BITS>
    data_t<NO_BITS> in_io(uint16_t port) {
        pmio *handler = find_handler_at_port(port);
        if (handler == nullptr) {
            DEBUG_ERROR("no device mapped to port : 0x%04x", port);
            return 0;
        }

        data_t<NO_BITS> v = handler->read_from_port<NO_BITS>(port);
        return v;
    }


//    void out_io32(uint16_t port, uint32_t value) {
//        for (int i = 0; i < 4; i++)
//            out_io8(port + i, (value >> (8 * i)) & 0xff);
//    }
//
//    void out_io16(uint16_t port, uint16_t value) {
//        for (int i = 0; i < 2; i++)
//            out_io8(port + i, (value >> (8 * i)) & 0xff);
//    }
//
//    void out_io8(uint16_t port, uint8_t value) {
//        pmio *handler = find_handler_at_port(port);
//        if (handler == nullptr) {
//            DEBUG_ERROR("no device mapped to port: 0x%04x", port);
//            return;
//        }
//
//        handler->write_byte_to_port(port, value);
//        DEBUG_INFO( "out [0x%04x] (0x%04x)", port, value);
//    }

    template<int NO_BITS>
    void out_io(uint16_t port, data_t<NO_BITS> value) {
        pmio *handler = find_handler_at_port(port);
        if (handler == nullptr) {
            DEBUG_ERROR("no device mapped to port: 0x%04x", port);
            return;
        }

        handler->write_to_port<NO_BITS>(port, value);
    }

};

#endif
