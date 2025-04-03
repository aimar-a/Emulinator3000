#ifndef NES_PPU_MEMORY_H
#define NES_PPU_MEMORY_H

#include "nes_structure.h"
#include "nes_logger.h"
#include <stdlib.h>

uint8_t ppu_read_ram(NES *nes, uint16_t addr);
void ppu_write_ram(NES *nes, uint16_t addr, uint8_t data);
void ppu_write_oam(NES *nes, uint8_t addr, uint8_t data);
void ppu_oamdma_transfer(NES *nes, uint8_t page);

#endif