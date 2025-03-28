#ifndef NES_PPU_H
#define NES_PPU_H

#include <stdint.h>
#include <stdbool.h>
#include "nes_structure.h"
#include <stdio.h>
#include "nes_logger.h"
#include <stdlib.h>
#include "nes_display.h"
#include "nes_ppu_memory.h"

typedef struct
{
  uint8_t y;
  uint8_t tile;
  uint8_t attributes;
  uint8_t x;
} Sprite;

void ppu_step(NES *nes);
void ppu_step_optimized(NES *nes);
void render_scanline(NES *nes);
uint8_t get_background_pixel(NES *nes, int x, int y);
uint8_t get_sprite_pixel(NES *nes, int x, int y);

#endif