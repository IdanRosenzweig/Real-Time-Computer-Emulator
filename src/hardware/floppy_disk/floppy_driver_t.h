#ifndef EMULATOR_FLOPPY_DRIVE_H
#define EMULATOR_FLOPPY_DRIVE_H

// number of sectors per head
#define NO_SECTORS 18
#define SECTOR_SZ 0x200

// number of tracks per head
#define NO_TRACKS 80

// number of heads (sides) per floppy disk
#define NO_HEADS 2

// the total amount of data in a single floppy disk is
// SECTOR_SZ * NO_SECTORS * NO_TRACKS * NO_HEADS_PRE_DISK

// a driver that handles a single floppy
struct floppy_driver_t {
    FILE *disk;

    // the disk hierarchy is as follows (head-:

    // the highest level
    uint8_t track: 7; // 0 indexed
    // middle level
    uint8_t head: 1; // 0 indexed
    // the lowest level
    uint8_t sector: 5; // 1 indexed

    uint16_t progress; // progress inside the sector itself, 0 indexed

    // advancing the arm reader in the floppy disk works in the steps:
    // 1. progress inside the sector itself
    // then if needed
    // 2. progress at the sector level (lowest level)
    // then if needed
    // 3. progress at the head level (middle level)
    // then if needed
    // 4. progress at the track level (highest level)
};

#endif
