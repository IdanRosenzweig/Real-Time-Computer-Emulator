#include <thread>
#include <cmath>
#include "floppy_disk_driver.hpp"

#include <iostream>

using namespace std;;

floppy_disk_driver::floppy_disk_driver() {
    commands[floppy_commands::FDD_READ_TRACK] = &floppy_disk_driver::fdd_read_track;
    commands[floppy_commands::FDD_WRITE_DATA] = &floppy_disk_driver::fdd_write_data;
    commands[floppy_commands::FDD_READ_DATA] = &floppy_disk_driver::fdd_read_data;
    commands[floppy_commands::FDD_CONFIGURE] = &floppy_disk_driver::fdd_configure;

    for (int i = 0; i < NO_DRIVERS; i++)
        drivers[i].disk = nullptr;

    conf.EIS = 0;
    conf.EFIFO = 1;
    conf.POLL = 0;

    sra.raw = 0;
    srb.raw = 0;

    command_q.max = 0;

    worker_th = std::thread(&floppy_disk_driver::worker, this);
}

floppy_disk_driver::~floppy_disk_driver() {
    for (int i = 0; i < NO_DRIVERS; i++)
        eject_disk(i);

    worker_th.join();
}


bool floppy_disk_driver::insert_disk(uint8_t slot, const char *fname) {
    if (slot >= NO_DRIVERS || drivers[slot].disk != nullptr)
        return false;

    drivers[slot].disk = fopen(fname, "rb+");
    if (drivers[slot].disk == nullptr) {
        return false;
    }

    drivers[slot].track = 0;
    drivers[slot].head = 0;
    drivers[slot].sector = 1;

    return true;
}

bool floppy_disk_driver::eject_disk(uint8_t slot) {
    if (slot >= NO_DRIVERS || drivers[slot].disk == nullptr)
        return false;

    fclose(drivers[slot].disk);
    drivers[slot].disk = nullptr;

    return true;
}


uint8_t floppy_disk_driver::read_byte_from_drive(uint8_t slot) {
    if (drivers[slot].disk == nullptr)
        DEBUG_ERROR("not ready disk%d", slot);

    uint8_t v;
    if (!fread(&v, 1, 1, drivers[slot].disk))
        v = 0;

    advance_pos(slot);

    return v;
}

void floppy_disk_driver::write_byte_to_drive(uint8_t slot, uint8_t v) {
    if (drivers[slot].disk == nullptr)
        DEBUG_ERROR("not ready disk%d", slot);

    fwrite(&v, 1, 1, drivers[slot].disk);

    advance_pos(slot);
}


void floppy_disk_driver::advance_pos(uint8_t slot) {
    // progress inside the sector itself
    if (drivers[slot].progress < SECTOR_SZ - 1) {
        drivers[slot].progress++;
        return;
    } else
        drivers[slot].progress = 0;

    // progress at the sector level
    if (drivers[slot].sector < NO_SECTORS) {
        drivers[slot].sector++;
        return;
    } else
        drivers[slot].sector = 1;

    // progress at the head level (sides level)
    if (drivers[slot].head < NO_HEADS - 1) {
        drivers[slot].head++;
        return;
    } else
        drivers[slot].head = 0;

    // progress at the track level
    if (drivers[slot].track < NO_TRACKS - 1) {
        drivers[slot].track++;
        return;
    } else
        drivers[slot].track = 0;

}

int32_t floppy_disk_driver::seek(uint8_t slot, uint8_t track, uint8_t head, uint8_t sector) {
    if (drivers[slot].disk == nullptr)
        DEBUG_ERROR("not ready disk%d", slot);

    cout << "seeking, sector:" << (int) sector << ", head: " << (int) head << ", track: " << (int) track<<"\n";
    if (sector < 1 || sector > NO_SECTORS) DEBUG_ERROR("");
    if (track < 0 || track >= NO_TRACKS) DEBUG_ERROR("");
    if (head < 0 || head >= NO_HEADS) DEBUG_ERROR("");

    msr.DRV_BSY |= 1 << slot;

    // those calculations are based on the hierarchy of the disk
    int32_t d_sector = (sector - drivers[slot].sector) * SECTOR_SZ;
    int32_t d_head = (head - drivers[slot].head) * SECTOR_SZ * NO_SECTORS;
    int32_t d_track = (track - drivers[slot].track) * SECTOR_SZ * NO_SECTORS * NO_HEADS;
    int32_t offset = d_sector + d_head + d_track;

    drivers[slot].track = track;
    drivers[slot].sector = sector;
    drivers[slot].head = head;
    drivers[slot].progress = 0;

    fseek(drivers[slot].disk, offset, SEEK_CUR);

    msr.DRV_BSY ^= 1 << slot;

    return offset;
}


uint8_t floppy_disk_driver::read_datareg(void) {
    while (!sra.INT)
        std::this_thread::sleep_for(std::chrono::microseconds(50));

    uint8_t v = curr_command;

    sra.INT = 0;

    return v;
}

void floppy_disk_driver::write_datareg(uint8_t v) {
    while (sra.INT)
        std::this_thread::sleep_for(std::chrono::microseconds(50));

    curr_command = v;

    sra.INT = 1;
}


uint8_t floppy_disk_driver::read_byte_from_port(uint16_t addr) {
    switch (addr) {
        case floppy_input_codes::STATUS_REGISTER_A:
            return sra.raw;
        case floppy_input_codes::STATUS_REGISTER_B:
            return srb.raw;
        case floppy_input_codes::TAPE_DRIVE_REGISTER:
            return tdr.raw;
        case floppy_input_codes::MAIN_STATUS_REGISTER:
            return msr.raw;
        case floppy_input_codes::DATA_FIFO:
            return read_datareg();
        case floppy_input_codes::DIGITAL_INPUT_REGISTER: // todo change to CONFIGURATION_CONTROL_REGISTER?
            return ccr.raw;
        default:
            return 0;
    }
}

void floppy_disk_driver::write_byte_to_port(uint16_t addr, uint8_t v) {
    switch (addr) {
        case floppy_output_codes::DIGITAL_OUTPUT_REGISTER:
            dor.raw = v;
            break;
        case floppy_output_codes::TAPE_DRIVE_REGISTER:
            tdr.raw = v;
            break;
        case floppy_output_codes::DATARATE_SELECT_REGISTER:
            dsr.raw = v;
            break;
        case floppy_output_codes::DATA_FIFO:
            command_q.enqueue(v);
            break;
        case floppy_output_codes::CONFIGURATION_CONTROL_REGISTER: // todo change to DIGITAL_INPUT_REGISTER?
            dir.raw = v;
            break;
    }
}


void floppy_disk_driver::fdd_read_track(void) {

    msr.RQM = 1;
    msr.DIO = 0;

    uint8_t cmd[8];
    for (int i = 0; i < 8; i++)
        cmd[i] = command_q.dequeue();

    uint8_t slot = cmd[0] & 3;
    if (conf.EIS)
        seek(slot, cmd[1], cmd[2], 1);

    size_t no_bytes = std::pow(2, cmd[4]) * 128 * NO_SECTORS;
    for (int i = 0; i < no_bytes; i++) {
        write_datareg(read_byte_from_drive(slot));
        intr_req = true;
    }

    write_datareg(st0.raw);
    write_datareg(st1.raw);
    write_datareg(st2.raw);
    write_datareg(drivers[slot].track);
    write_datareg(drivers[slot].head);
    write_datareg(drivers[slot].sector);
    write_datareg(cmd[4]);

    msr.RQM = 0;
}

void floppy_disk_driver::fdd_write_data(void) {
    uint8_t cmd[8];
    uint8_t slot;

    msr.RQM = 1;
    msr.DIO = 1;

    for (int i = 0; i < 8; i++)
        cmd[i] = command_q.dequeue();

    slot = cmd[0] & 3;
    if (conf.EIS)
        seek(slot, cmd[1], cmd[2], cmd[3]);

    size_t no_bytes = std::pow(2, cmd[4]) * 128;
    for (int i = 0; i < no_bytes; i++) {
        write_byte_to_drive(slot, command_q.dequeue());
        intr_req = true;
    }

    write_datareg(st0.raw);
    write_datareg(st1.raw);
    write_datareg(st2.raw);
    write_datareg(drivers[slot].track);
    write_datareg(drivers[slot].head);
    write_datareg(drivers[slot].sector);
    write_datareg(cmd[4]);

    msr.RQM = 0;
}

void floppy_disk_driver::fdd_read_data(void) {
    uint8_t cmd[8];
    uint8_t slot;

    msr.RQM = 1;
    msr.DIO = 0;

    for (int i = 0; i < 8; i++)
        cmd[i] = command_q.dequeue();

    slot = cmd[0] & 3;
    if (conf.EIS)
        seek(slot, cmd[1], cmd[2], cmd[3]);

    size_t no_bytes = std::pow(2, cmd[4]) * 128;
    for (int i = 0; i < no_bytes; i++) {
        write_datareg(read_byte_from_drive(slot));
        intr_req = true;
    }

    write_datareg(st0.raw);
    write_datareg(st1.raw);
    write_datareg(st2.raw);
    write_datareg(drivers[slot].track);
    write_datareg(drivers[slot].head);
    write_datareg(drivers[slot].sector);
    write_datareg(cmd[4]);

    msr.RQM = 0;
}

void floppy_disk_driver::fdd_configure(void) {
    uint8_t cmd[3];

    for (int i = 0; i < 3; i++)
        cmd[i] = command_q.dequeue();
    conf.raw = cmd[1];
}


void floppy_disk_driver::worker(void) {
    while (true) {
        while (command_q.queue.empty())
            std::this_thread::sleep_for(std::chrono::milliseconds(10));

        uint8_t command = command_q.dequeue();
        if (commands[command] == nullptr) {
            curr_command = 0x80;
            continue;
        }

        msr.CMD_BSY = 1;
        (this->*commands[command])();
        msr.CMD_BSY = 0;
    }
}