#include "nes_ppu_regs.h"

// Funcion para leer registros de la PPU
uint8_t ppu_read_register(NES *nes, uint16_t address)
{
  uint8_t value = 0;
  nes_log("\nINFO: PPU Reading register: 0x%04X", address);
  switch (address)
  {
  case 0x2002: // PPUSTATUS
    uint8_t status = nes->ppu->status;
    nes->ppu->status &= 0x7F;   // Limpia el flag de VBlank
    nes->ppu->write_toggle = 0; // Resetea el toggle de escritura en $2005/$2006
    value = status;
    break;
  case 0x2004: // OAMDATA (OAM)
    value = nes->ppu->oamdata;
    break;
  case 0x2007: // PPUDATA (VRAM)
    uint8_t data = nes->ppu->buffer;
    nes->ppu->buffer = ppu_read_ram(nes, nes->ppu->v);
    nes->ppu->v += (nes->ppu->ctrl & 0x04) ? 32 : 1;
    value = data;
    break;
  default:
    nes_log("\nERROR: PPU Register 0x%04X not readable\n", address);
    exit(1);
  }
  return value;
}

// Funcion para escribir registros de la PPU
void ppu_write_register(NES *nes, uint16_t address, uint8_t value)
{
  nes_log("INFO: PPU Writing register: 0x%04X, value: 0x%02X\n", address, value);
  switch (address)
  {
  case 0x2000: // PPUCTRL
    nes->ppu->ctrl = value;
    nes->ppu->t = (nes->ppu->t & 0xF3FF) | ((value & 0x03) << 10); // Ajustar nametable base
    break;
  case 0x2001: // PPUMASK
    nes->ppu->mask = value;
    break;
  case 0x2003: // OAMADDR
    nes->ppu->oamaddr = value;
    break;
  case 0x2004: // OAMDATA
    ppu_write_oam(nes, nes->ppu->oamaddr, value);
    nes->ppu->oamaddr++;
    break;
  case 0x2005: // PPUSCROLL
    if (nes->ppu->write_toggle == 0)
    {
      nes->ppu->t = (nes->ppu->t & 0xFFE0) | (value >> 3);
      nes->ppu->x = value & 0x07;
      nes->ppu->write_toggle = 1;
    }
    else
    {
      nes->ppu->t = (nes->ppu->t & 0x8FFF) | ((value & 0x07) << 12);
      nes->ppu->t = (nes->ppu->t & 0xFC1F) | ((value & 0xF8) << 2);
      nes->ppu->write_toggle = 0;
    }
    break;
  case 0x2006: // PPUADDR
    if (nes->ppu->write_toggle == 0)
    {
      nes->ppu->t = (nes->ppu->t & 0x00FF) | ((value & 0x3F) << 8);
      nes->ppu->write_toggle = 1;
    }
    else
    {
      nes->ppu->t = (nes->ppu->t & 0xFF00) | value;
      nes->ppu->v = nes->ppu->t;
      nes->ppu->write_toggle = 0;
    }
    break;
  case 0x2007: // PPUDATA
    ppu_write_ram(nes, nes->ppu->v, value);
    nes->ppu->v += (nes->ppu->ctrl & 0x04) ? 32 : 1;
    break;
  default:
    nes_log("ERROR: PPU Register 0x%04X not writable\n", address);
    exit(1);
  }
}
