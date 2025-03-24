#ifndef NES_PPU_H
#define NES_PPU_H

#include <stdint.h>
#include <stdbool.h>
#include "nes_structure.h"
#include <stdio.h>

typedef struct
{
  uint8_t y;
  uint8_t tile;
  uint8_t attributes;
  uint8_t x;
} Sprite;

void ppu_step(NES *nes);
void render_scanline(NES *nes);
uint8_t get_background_pixel(NES *nes, int x, int y);
uint8_t get_sprite_pixel(NES *nes, int x, int y);
void ppu_write(NES *nes, uint16_t address, uint8_t data);
void registers_operations(NES *nes);

#endif