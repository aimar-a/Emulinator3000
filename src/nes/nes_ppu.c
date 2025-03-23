#include "nes_ppu.h"
#include "nes_structure.h"

void ppu_main_loop(PPU *ppu, uint8_t *screen)
{
  // esto aqui deberia de hacer cosas pero no se el que :)
}

uint8_t ppu_read(PPU *ppu, uint16_t address)
{
  if (address < 0x2000)
  {
    return ppu->vram[address % 0x2000];
  }
  else if (address < 0x3F00)
  {
    return ppu->vram[address % 0x2000];
  }
  else
  {
    return ppu->palette[address % 32];
  }
}

void ppu_write(PPU *ppu, uint16_t address, uint8_t value)
{
  if (address < 0x2000)
  {
    ppu->vram[address % 0x2000] = value;
  }
  else if (address < 0x3F00)
  {
    ppu->vram[address % 0x2000] = value;
  }
  else
  {
    ppu->palette[address % 32] = value;
  }
}