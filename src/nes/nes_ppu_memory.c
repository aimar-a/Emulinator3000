#include "nes_ppu_memory.h"

// Función para leer de la memoria de la PPU
uint8_t ppu_read_ram(NES *nes, uint16_t addr)
{
  nes_log("INFO: PPU Reading address 0x%04X", addr);
  uint8_t value = 0;
  if (addr < 0x1000)
  {
    nes_log(" (Pattern Table 0 CHR ROM)");
    value = nes->rom->chr_rom[addr];
  }
  else if (addr < 0x2000)
  {
    nes_log(" (Pattern Table 1 CHR ROM)");
    value = nes->rom->chr_rom[addr];
  }
  else if (addr < 0x23C0)
  {
    nes_log(" (Name Table 0 VRAM)");
    value = nes->ppu->vram[addr];
  }
  else if (addr < 0x2400)
  {
    nes_log(" (Attribute Table 0 VRAM)");
    value = nes->ppu->vram[addr];
  }
  else if (addr < 0x27C0)
  {
    nes_log(" (Name Table 1 VRAM)");
    value = nes->ppu->vram[addr];
  }
  else if (addr < 0x2800)
  {
    nes_log(" (Attribute Table 1 VRAM)");
    value = nes->ppu->vram[addr];
  }
  else if (addr < 0x2BC0)
  {
    nes_log(" (Name Table 2 VRAM)");
    value = nes->ppu->vram[addr];
  }
  else if (addr < 0x2C00)
  {
    nes_log(" (Attribute Table 2 VRAM)");
    value = nes->ppu->vram[addr];
  }
  else if (addr < 0x2FC0)
  {
    nes_log(" (Name Table 3 VRAM)");
    value = nes->ppu->vram[addr];
  }
  else if (addr < 0x3000)
  {
    nes_log(" (Attribute Table 3 VRAM)");
    value = nes->ppu->vram[addr];
  }
  else if (addr < 0x3F00)
  {
    nes_log(" (Mirrors $2000-$2FFF)");
    nes_log("\nWARNING: Reading from PPU mirrors is not implemented yet.\n");
    // TODO
  }
  else if (addr < 0x3F10)
  {
    nes_log(" (Image Palette)");
    value = nes->ppu->palette[addr % 0x3F00];
  }
  else if (addr < 0x3F20)
  {
    nes_log(" (Sprite Palette)");
    value = nes->ppu->palette[addr % 0x3F00];
  }
  else if (addr < 0x4000)
  {
    nes_log(" (Mirrors $3F00-$3F1F)");
    nes_log("\nWARNING: Reading from PPU mirrors is not implemented yet.\n");
    // TODO
  }
  else if (addr < 0x10000)
  {
    nes_log(" (Mirrors $0000-$3FFF)");
    nes_log("\nWARNING: Reading from PPU mirrors is not implemented yet.\n");
    // TODO
  }
  else
  {
    nes_log("ERROR: PPU Invalid address 0x%04X\n", addr);
    exit(1);
  }

  nes_log(", Value read: 0x%02X\n", value);
  // nes->ppu->data = value; // TODO: ver si es necesario
  return value;
}

// Función para escribir en la memoria de la PPU
void ppu_write_ram(NES *nes, uint16_t addr, uint8_t data)
{
  nes_log("INFO: PPU Writing address 0x%04X", addr);
  if (addr < 0x1000)
  {
    nes_log(" (Pattern Table 0 CHR ROM)");
    nes->rom->chr_rom[addr] = data;
  }
  else if (addr < 0x2000)
  {
    nes_log(" (Pattern Table 1 CHR ROM)");
    nes->rom->chr_rom[addr] = data;
  }
  else if (addr < 0x23C0)
  {
    nes_log(" (Name Table 0 VRAM)");
    nes->ppu->vram[addr] = data;
  }
  else if (addr < 0x2400)
  {
    nes_log(" (Attribute Table 0 VRAM)");
    nes->ppu->vram[addr] = data;
  }
  else if (addr < 0x27C0)
  {
    nes_log(" (Name Table 1 VRAM)");
    nes->ppu->vram[addr] = data;
  }
  else if (addr < 0x2800)
  {
    nes_log(" (Attribute Table 1 VRAM)");
    nes->ppu->vram[addr] = data;
  }
  else if (addr < 0x2BC0)
  {
    nes_log(" (Name Table 2 VRAM)");
    nes->ppu->vram[addr] = data;
  }
  else if (addr < 0x2C00)
  {
    nes_log(" (Attribute Table 2 VRAM)");
    nes->ppu->vram[addr] = data;
  }
  else if (addr < 0x2FC0)
  {
    nes_log(" (Name Table 3 VRAM)");
    nes->ppu->vram[addr] = data;
  }
  else if (addr < 0x3000)
  {
    nes_log(" (Attribute Table 3 VRAM)");
    nes->ppu->vram[addr] = data;
  }
  else if (addr < 0x3F00)
  {
    nes_log(" (Mirrors $2000-$2FFF)");
    nes_log("\nWARNING: Writing to PPU mirrors is not implemented yet.\n");
    // TODO
  }
  else if (addr < 0x3F10)
  {
    nes_log(" (Image Palette)");
    nes->ppu->palette[addr % 0x3F00] = data;
  }
  else if (addr < 0x3F20)
  {
    nes_log(" (Sprite Palette)");
    nes->ppu->palette[addr % 0x3F00] = data;
  }
  else if (addr < 0x4000)
  {
    nes_log(" (Mirrors $3F00-$3F1F)");
    nes_log("\nWARNING: Writing to PPU mirrors is not implemented yet.\n");
    // TODO
  }
  else if (addr < 0x10000)
  {
    nes_log(" (Mirrors $0000-$3FFF)");
    nes_log("\nWARNING: Writing to PPU mirrors is not implemented yet.\n");
    // TODO
  }
  else
  {
    nes_log("ERROR: PPU Invalid address 0x%04X\n", addr);
    exit(1);
  }

  nes_log(", Value written: 0x%02X\n", data);
}

// Función para leer de la memoria de OAM
void ppu_write_oam(NES *nes, uint8_t addr, uint8_t data)
{
  nes_log("INFO: Writing OAM address 0x%02X, data: 0x%02X\n", addr, data);
  if (addr >= 0x100)
  {
    nes_log("ERROR: OAM address out of range: 0x%02X\n", addr);
    exit(1);
  }
  nes->ppu->oam[addr] = data;
}
