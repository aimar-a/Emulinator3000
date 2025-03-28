#include "nes_ppu_memory.h"

// Función para leer de la memoria de la PPU
uint8_t ppu_read_ram(NES *nes, uint16_t addr)
{
  nes_log("INFO: PPU Reading address 0x%04X\n", addr);
  if (addr < 0x2000)
  {
    nes->ppu->data = nes->rom->chr_rom[addr];
  }
  else if (addr < 0x3F00)
  {
    nes->ppu->data = nes->ppu->vram[addr];
  }
  else if (addr < 0x4000)
  {
    if (addr > 0x3F1F)
    {
      nes_log("ERROR: PPU Invalid address 0x%04X\n", addr);
      exit(1);
    }
    else
      nes->ppu->data = nes->ppu->palette[addr % 0x3F00];
  }
  else
  {
    nes_log("ERROR: PPU Invalid address 0x%04X\n", addr);
    exit(1);
  }
}

// Función para escribir en la memoria de la PPU
void ppu_write_ram(NES *nes, uint16_t addr, uint8_t data)
{
  nes_log("INFO: PPU Writing address 0x%04X, data: 0x%02X\n", addr, data);
  if (addr < 0x2000)
  {
    nes->rom->chr_rom[addr] = data;
  }
  else if (addr < 0x3F00)
  {
    nes->ppu->vram[addr] = data;
  }
  else if (addr < 0x4000)
  {
    if (addr > 0x3F1F)
    {
      nes_log("ERROR: PPU Invalid address 0x%04X\n", addr);
      exit(1);
    }
    else
      nes->ppu->palette[addr % 0x3F00] = data;
  }
  else
  {
    nes_log("ERROR: PPU Invalid address 0x%04X\n", addr);
    exit(1);
  }
}

// Función para leer de la memoria de OAM
void ppu_write_oam(NES *nes, uint8_t addr, uint8_t data)
{
  nes_log("INFO: Writing OAM address 0x%02X, data: 0x%02X\n", addr, data);
  nes->ppu->oam[addr] = data;
}
