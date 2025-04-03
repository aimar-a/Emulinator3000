#include "nes_ppu_memory.h"

// Función para leer de la memoria de la PPU
uint8_t ppu_read_ram(NES *nes, uint16_t addr)
{
  nes_log_traceback("INFO: PPU Reading address 0x%04X", addr);
  uint8_t value = 0;
  if (addr < 0x1000)
  {
    nes_log_traceback(" (Pattern Table 0 CHR ROM)");
    value = nes->rom->chr_rom[addr];
  }
  else if (addr < 0x2000)
  {
    nes_log_traceback(" (Pattern Table 1 CHR ROM)");
    value = nes->rom->chr_rom[addr];
  }
  else if (addr < 0x23C0)
  {
    nes_log_traceback(" (Name Table 0 VRAM)");
    value = nes->ppu->vram[addr];
  }
  else if (addr < 0x2400)
  {
    nes_log_traceback(" (Attribute Table 0 VRAM)");
    value = nes->ppu->vram[addr];
  }
  else if (addr < 0x27C0)
  {
    nes_log_traceback(" (Name Table 1 VRAM)");
    value = nes->ppu->vram[addr];
  }
  else if (addr < 0x2800)
  {
    nes_log_traceback(" (Attribute Table 1 VRAM)");
    value = nes->ppu->vram[addr];
  }
  else if (addr < 0x2BC0)
  {
    nes_log_traceback(" (Name Table 2 VRAM)");
    value = nes->ppu->vram[addr];
  }
  else if (addr < 0x2C00)
  {
    nes_log_traceback(" (Attribute Table 2 VRAM)");
    value = nes->ppu->vram[addr];
  }
  else if (addr < 0x2FC0)
  {
    nes_log_traceback(" (Name Table 3 VRAM)");
    value = nes->ppu->vram[addr];
  }
  else if (addr < 0x3000)
  {
    nes_log_traceback(" (Attribute Table 3 VRAM)");
    value = nes->ppu->vram[addr];
  }
  else if (addr < 0x3F00)
  {
    nes_log_traceback(" (Mirrors $2000-$2FFF)");
    nes_log_instant("WARNING: Reading from PPU mirrors is not implemented yet.\n");
    // TODO
  }
  else if (addr < 0x3F10)
  {
    nes_log_traceback(" (Image Palette)");
    value = nes->ppu->palette[addr % 0x3F00];
  }
  else if (addr < 0x3F20)
  {
    nes_log_traceback(" (Sprite Palette)");
    value = nes->ppu->palette[addr % 0x3F00];
  }
  else if (addr < 0x4000)
  {
    nes_log_traceback(" (Mirrors $3F00-$3F1F)");
    nes_log_instant("WARNING: Reading from PPU mirrors is not implemented yet.\n");
    // TODO
  }
  else if (addr < 0x10000)
  {
    nes_log_traceback(" (Mirrors $0000-$3FFF)");
    nes_log_instant("WARNING: Reading from PPU mirrors is not implemented yet.\n");
    // TODO
  }
  else
  {
    nes_log_error("ERROR: PPU Invalid address 0x%04X\n", addr);
    exit(1);
  }

  nes_log_traceback(", Value read: 0x%02X\n", value);
  // nes->ppu->data = value; // TODO: ver si es necesario
  return value;
}

// Función para escribir en la memoria de la PPU
void ppu_write_ram(NES *nes, uint16_t addr, uint8_t data)
{
  nes_log_traceback("INFO: PPU Writing address 0x%04X", addr);
  if (addr < 0x1000)
  {
    nes_log_traceback(" (Pattern Table 0 CHR ROM)");
    nes->rom->chr_rom[addr] = data;
  }
  else if (addr < 0x2000)
  {
    nes_log_traceback(" (Pattern Table 1 CHR ROM)");
    nes->rom->chr_rom[addr] = data;
  }
  else if (addr < 0x23C0)
  {
    nes_log_traceback(" (Name Table 0 VRAM)");
    nes->ppu->vram[addr] = data;
  }
  else if (addr < 0x2400)
  {
    nes_log_traceback(" (Attribute Table 0 VRAM)");
    nes->ppu->vram[addr] = data;
  }
  else if (addr < 0x27C0)
  {
    nes_log_traceback(" (Name Table 1 VRAM)");
    nes->ppu->vram[addr] = data;
  }
  else if (addr < 0x2800)
  {
    nes_log_traceback(" (Attribute Table 1 VRAM)");
    nes->ppu->vram[addr] = data;
  }
  else if (addr < 0x2BC0)
  {
    nes_log_traceback(" (Name Table 2 VRAM)");
    nes->ppu->vram[addr] = data;
  }
  else if (addr < 0x2C00)
  {
    nes_log_traceback(" (Attribute Table 2 VRAM)");
    nes->ppu->vram[addr] = data;
  }
  else if (addr < 0x2FC0)
  {
    nes_log_traceback(" (Name Table 3 VRAM)");
    nes->ppu->vram[addr] = data;
  }
  else if (addr < 0x3000)
  {
    nes_log_traceback(" (Attribute Table 3 VRAM)");
    nes->ppu->vram[addr] = data;
  }
  else if (addr < 0x3F00)
  {
    nes_log_traceback(" (Mirrors $2000-$2FFF)");
    nes_log_instant("WARNING: Writing to PPU mirrors is not implemented yet.\n");
    // TODO
  }
  else if (addr < 0x3F10)
  {
    nes_log_traceback(" (Image Palette)");
    nes->ppu->palette[addr % 0x3F00] = data;
  }
  else if (addr < 0x3F20)
  {
    nes_log_traceback(" (Sprite Palette)");
    nes->ppu->palette[addr % 0x3F00] = data;
  }
  else if (addr < 0x4000)
  {
    nes_log_traceback(" (Mirrors $3F00-$3F1F)");
    nes_log_instant("WARNING: Writing to PPU mirrors is not implemented yet.\n");
    // TODO
  }
  else if (addr < 0x10000)
  {
    nes_log_traceback(" (Mirrors $0000-$3FFF)");
    nes_log_instant("WARNING: Writing to PPU mirrors is not implemented yet.\n");
    // TODO
  }
  else
  {
    nes_log_error("ERROR: PPU Invalid address 0x%04X\n", addr);
    exit(1);
  }

  nes_log_traceback(", Value written: 0x%02X\n", data);
}

// Función para leer de la memoria de OAM
void ppu_write_oam(NES *nes, uint8_t addr, uint8_t data)
{
  nes_log_traceback("INFO: Writing OAM address 0x%02X, data: 0x%02X\n", addr, data);
  if (addr >= 0x100)
  {
    nes_log_error("ERROR: OAM address out of range: 0x%02X\n", addr);
  }
  nes->ppu->oam[addr] = data;
}

void ppu_oamdma_transfer(NES *nes, uint8_t page)
{
  nes_log_traceback("INFO: Starting OAM DMA transfer from page 0x%02X\n", page);

  // Calculate source address (page * 256)
  uint16_t src_addr = (uint16_t)page << 8;
  uint8_t *src = &nes->memory[src_addr];

  // Get PPU OAM pointer (typically 0x2004)
  uint8_t *oam = nes->ppu->oam;

  // Perform the 256-byte transfer
  for (int i = 0; i < 256; i++)
  {
    oam[i] = src[i];
  }

  // Log completion
  nes_log_traceback("INFO: OAM DMA transfer completed (0x%04X-0x%04X -> OAM)\n",
                    src_addr, src_addr + 255);

  // DMA takes 513 or 514 cycles (depending on odd/even CPU cycle)
  int cycles = (nes->cycles % 2 == 1) ? 513 : 514;
  nes->stall_cycles += cycles;

  nes_log_traceback("INFO: CPU stalled for %d cycles\n", cycles);
}