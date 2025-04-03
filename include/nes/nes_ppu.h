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

// PPUMASK REGISTER
#define COLOR_EMPHASIS_RED 0x20
#define COLOR_EMPHASIS_GREEN 0x40
#define COLOR_EMPHASIS_BLUE 0x80
#define COLOR_EMPHASIS_RGB (COLOR_EMPHASIS_RED | COLOR_EMPHASIS_GREEN | COLOR_EMPHASIS_BLUE)
#define SPRITE_ENABLE 0x10
#define BACKGROUND_ENABLE 0x08
#define SPRITE_LEFT_COLUMN_ENABLE 0x04
#define BACKGROUND_LEFT_COLUMN_ENABLE 0x02
#define GREYSCALE 0x01

// PPUSTATUS REGISTER
#define VBLANK_FLAG 0x80
#define SPRITE_0_HIT_FLAG 0x40
#define SPRITE_OVERFLOW_FLAG 0x20

// PPUCTRL REGISTER
#define NMI_ENABLE 0x80
#define PPU_MASTER_SLAVE 0x40
#define SPRITE_HEIGHT 0x20
#define BACKGROUND_TILE_SELECT 0x10
#define SPRITE_TILE_SELECT 0x08
#define INCREMENT_MODE 0x04
#define NAMETABLE_ADDRESS 0x03

// CONSTANTS
// const int ScanlineCycleLength = 341;
extern const int ScanlineEndCycle;
extern const int VisibleScanlines;
extern const int ScanlineVisibleDots;
extern const int FrameEndScanline;

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
uint8_t readPalette(NES *nes, uint8_t addr);

#endif