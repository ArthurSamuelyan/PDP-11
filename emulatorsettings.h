#ifndef EMULATORSETTINGS_H
#define EMULATORSETTINGS_H

#define RAM_SIZE 16 * 512
#define VRAM_SIZE 16 * 512
#define ROM_SIZE 24 * 512
#define IO_SIZE 8 * 512

#define RAM_START 0
#define VRAM_START RAM_START + RAM_SIZE
#define ROM_START VRAM_START + VRAM_SIZE
#define IO_START ROM_START + ROM_SIZE

#define MEM_SIZE IO_START + IO_SIZE

#define TAIL_SIZE 1 * 512               // The last Kb to store ports addresses
#define TAIL_START MEM_SIZE - TAIL_SIZE


// + screen settings

#endif // EMULATORSETTINGS_H
