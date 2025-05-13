#ifndef NES_PPU_REGS_H
#define NES_PPU_REGS_H

#include "nes_structure.h"
#include "nes_ppu_memory.h"

uint8_t ppu_read_register(NES *nes, uint16_t address);
void ppu_write_register(NES *nes, uint16_t address, uint8_t value);

#endif