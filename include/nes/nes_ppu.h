#ifndef NES_PPU_H
#define NES_PPU_H

#include <stdint.h>
#include <stdbool.h>
#include "nes_structure.h"
#include <stdio.h>

void ppu_main_loop(PPU *ppu, uint8_t *screen);
uint8_t ppu_read(PPU *ppu, uint16_t address);
void ppu_write(PPU *ppu, uint16_t address, uint8_t value);

#endif