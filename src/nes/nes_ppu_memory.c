#include "nes_ppu_memory.h"

// Función para leer de la memoria de la PPU
uint8_t ppu_read_ram(PPU *ppu, uint16_t addr)
{
  nes_log("INFO: Reading PPU address 0x%04X\n", addr);
  if (addr < 0x2000)
  {
    ppu->data = ppu->vram[addr % 0x2000];
  }
  else if (addr < 0x3F00)
  {
    ppu->data = ppu->palette[addr % 32];
  }
  else if (addr < 0x4000)
  {
    ppu->data = ppu->palette[addr % 32];
  }
  else
  {
    nes_log("ERROR: Invalid PPU address 0x%04X\n", addr);
    exit(1);
  }
}

// Función para escribir en la memoria de la PPU
void ppu_write_ram(PPU *ppu, uint16_t addr, uint8_t data)
{
  nes_log("INFO: Writing PPU address 0x%04X\n", addr);
  if (addr < 0x2000)
  {
    ppu->vram[addr % 0x2000] = data;
  }
  else if (addr < 0x3F00)
  {
    ppu->palette[addr % 32] = data;
  }
  else if (addr < 0x4000)
  {
    ppu->palette[addr % 32] = data;
  }
  else
  {
    nes_log("ERROR: Invalid PPU address 0x%04X\n", addr);
    exit(1);
  }
}
