#ifndef NES_PPU_MEMORY_H
#define NES_PPU_MEMORY_H

#include "nes_structure.h"
#include "nes_logger.h"
#include <stdlib.h>

uint8_t ppu_read_ram(PPU *ppu, uint16_t addr);
void ppu_write_ram(PPU *ppu, uint16_t addr, uint8_t data);

#endif