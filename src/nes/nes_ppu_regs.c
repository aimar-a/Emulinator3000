#include "nes_ppu_regs.h"

// Funcion para leer registros de la PPU
uint8_t ppu_read_register(PPU *ppu, uint16_t address)
{
  uint8_t value = 0;
  nes_log("\nINFO: PPU Reading register: 0x%04X", address);
  switch (address)
  {
  case 0x2002: // PPUSTATUS
    uint8_t status = ppu->status;
    ppu->status &= 0x7F;   // Limpia el flag de VBlank
    ppu->write_toggle = 0; // Resetea el toggle de escritura en $2005/$2006
    value = status;
    break;
  case 0x2004: // OAMDATA (OAM)
    value = ppu->oamdata;
    break;
  case 0x2007: // PPUDATA (VRAM)
    uint8_t data = ppu->buffer;
    ppu->buffer = ppu_read_ram(ppu, ppu->v);
    ppu->v += (ppu->ctrl & 0x04) ? 32 : 1;
    value = data;
    break;
  default:
    nes_log("\nERROR: PPU Register 0x%04X not readable\n", address);
    exit(1);
  }
  return value;
}

// Funcion para escribir registros de la PPU
void ppu_write_register(PPU *ppu, uint16_t address, uint8_t value)
{
  nes_log("INFO: PPU Writing register: 0x%04X, value: 0x%02X\n", address, value);
  switch (address)
  {
  case 0x2000: // PPUCTRL
    ppu->ctrl = value;
    ppu->t = (ppu->t & 0xF3FF) | ((value & 0x03) << 10); // Ajustar nametable base
    break;
  case 0x2001: // PPUMASK
    ppu->mask = value;
    break;
  case 0x2003: // OAMADDR
    ppu->oamaddr = value;
    break;
  case 0x2004: // OAMDATA
    ppu_write_oam(ppu, ppu->oamaddr, value);
    ppu->oamaddr++;
    break;
  case 0x2005: // PPUSCROLL
    if (ppu->write_toggle == 0)
    {
      ppu->t = (ppu->t & 0xFFE0) | (value >> 3);
      ppu->x = value & 0x07;
      ppu->write_toggle = 1;
    }
    else
    {
      ppu->t = (ppu->t & 0x8FFF) | ((value & 0x07) << 12);
      ppu->t = (ppu->t & 0xFC1F) | ((value & 0xF8) << 2);
      ppu->write_toggle = 0;
    }
    break;
  case 0x2006: // PPUADDR
    if (ppu->write_toggle == 0)
    {
      ppu->t = (ppu->t & 0x00FF) | ((value & 0x3F) << 8);
      ppu->write_toggle = 1;
    }
    else
    {
      ppu->t = (ppu->t & 0xFF00) | value;
      ppu->v = ppu->t;
      ppu->write_toggle = 0;
    }
    break;
  case 0x2007: // PPUDATA
    ppu_write_ram(ppu, ppu->v, value);
    ppu->v += (ppu->ctrl & 0x04) ? 32 : 1;
    break;
  default:
    nes_log("ERROR: PPU Register 0x%04X not writable\n", address);
    exit(1);
  }
}
