#ifndef NES_PPU_H
#define NES_PPU_H

#include <stdint.h>
#include <stdbool.h>
#include "nes_structure.h"
#include <stdio.h>

void ppu_step(NES *nes);
void render_scanline(PPU *ppu, uint8_t *screen, uint16_t scanline);

#endif