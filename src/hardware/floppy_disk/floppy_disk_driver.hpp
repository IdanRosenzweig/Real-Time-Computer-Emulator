#ifndef EMULATOR_FLOPPY_DISK_DRIVER_H
#define EMULATOR_FLOPPY_DISK_DRIVER_H

#include <thread>

#include "../../common.hpp"
#include "../cpu_i386/io/port_mapped_io.h"
#include "../cpu_i386/pic/interrupt_caller.hpp"

#include "floppy_commands.h"
#include "floppy_driver_t.h"
#include "commands_queue.h"
#include "floppy_input_codes.h"
#include "floppy_output_codes.h"

#define NO_DRIVERS 4

// floppy disk driver
class floppy_disk_driver : public device_interrupt_caller, public pmio {
private:
    typedef void (floppy_disk_driver::*fddfunc_t)(void);

    fddfunc_t commands[MAX_COMMAND_NUM] = {nullptr};

    // current configuration
    union {
        uint8_t raw;
        struct {
            uint8_t FIFOTHR: 4;
            uint8_t POLL: 1;
            uint8_t EFIFO: 1;
            uint8_t EIS: 1;
        };
    } conf;


    // status register a
    union {
        uint8_t raw;
        struct {
            uint8_t DIR: 1;
            uint8_t nWP: 1;
            uint8_t nINDX: 1;
            uint8_t HDSEL: 1;
            uint8_t nTRK0: 1;
            uint8_t STEP: 1;
            uint8_t nDRV2: 1;
            uint8_t INT: 1;
        };
    } sra;

    // status register b
    union {
        uint8_t raw;
        struct {
            uint8_t MOT0: 1;
            uint8_t MOT1: 1;
            uint8_t WE: 1;
            uint8_t RD: 1;
            uint8_t WR: 1;
            uint8_t SEL0: 1;
        };
    } srb;

    // main status register
    union {
        uint8_t raw;
        struct {
            uint8_t DRV_BSY: 4; // holds which drivers are busy
            uint8_t CMD_BSY: 1; // handling command (command busy)
            uint8_t NON_DMA: 1;
            uint8_t DIO: 1;
            uint8_t RQM: 1;
        };
    } msr;


    // tap driver register
    union {
        uint8_t raw;
    } tdr;


    // datagram select register
    union {
        uint8_t raw;
    } dsr;


    // configuration control register
    union {
        uint8_t raw;
    } ccr;


    // digital input register
    union {
        uint8_t raw;
    } dir;

    // digital output register
    union {
        uint8_t raw;
        struct {
            uint8_t SEL0: 1;
            uint8_t SEL1: 1;
            uint8_t nRESET: 1;
            uint8_t nDMA: 1;
            uint8_t MOT: 4;
        };
    } dor;


    // 3 error status registers
    union {
        uint8_t raw;
        struct {
            uint8_t DS0: 1;
            uint8_t DS1: 1;
            uint8_t H: 1;
            uint8_t : 1;
            uint8_t EC: 1;
            uint8_t SE: 1;
            uint8_t IC0: 1;
            uint8_t IC1: 1;
        };
    } st0;

    union {
        uint8_t raw;
        struct {
            uint8_t MA: 1;
            uint8_t NW: 1;
            uint8_t ND: 1;
            uint8_t : 1;
            uint8_t OR: 1;
            uint8_t DE: 1;
            uint8_t : 1;
            uint8_t EN: 1;
        };
    } st1;

    union {
        uint8_t raw;
        struct {
            uint8_t MD: 1;
            uint8_t BC: 1;
            uint8_t SN: 1;
            uint8_t SH: 1;
            uint8_t WC: 1;
            uint8_t DD: 1;
            uint8_t CM: 1;
        };
    } st2;

//    union {
//        uint8_t raw;
//        struct {
//            uint8_t DS0: 1;
//            uint8_t DS1: 1;
//            uint8_t HD: 1;
//            uint8_t : 1;
//            uint8_t T0: 1;
//            uint8_t : 1;
//            uint8_t WP: 1;
//        };
//    } str3;

    uint8_t curr_command;
    struct commands_queue command_q;

    struct floppy_driver_t drivers[NO_DRIVERS];

public:
    floppy_disk_driver();

    ~floppy_disk_driver();

    // insert and remove floppy disks
    bool insert_disk(uint8_t slot, const char *fname);

    bool eject_disk(uint8_t slot);


    // read and write byte to a driver
    uint8_t read_byte_from_drive(uint8_t slot);

    void write_byte_to_drive(uint8_t slot, uint8_t v);


    // advance the associated position of the drive in one byte
    void advance_pos(uint8_t slot);

    // change the associated position of the driver to specific position
    int32_t seek(uint8_t slot, uint8_t track, uint8_t head, uint8_t sector);


    uint8_t read_datareg(void);

    void write_datareg(uint8_t v);


    // input byte and output byte (pmio handlers)
    uint8_t read_byte_from_port(uint16_t addr) override;

    void write_byte_to_port(uint16_t addr, uint8_t v) override;


    //bool chk_intreq(void) { if(sra.INT){ sra.INT = false; return true; } return false; };
private:
    // various floppy_disk commands
    void fdd_read_track(void);

    void fdd_write_data(void);

    void fdd_read_data(void);

    void fdd_configure(void);


    // a thread that handles the commands that are pushed to the queue
    std::thread worker_th;

    void worker(void);

};

#endif
