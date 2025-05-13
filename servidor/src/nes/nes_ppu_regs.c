#include "nes_ppu_regs.h"

uint8_t ppu_read_register(NES *nes, uint16_t address)
{
  uint8_t value = 0;
  address &= 0x2007; // Handle mirroring

  nes_log_traceback("INFO: PPU Reading register: 0x%04X", address);

  switch (address)
  {
  case 0x2002: // PPUSTATUS
    value = nes->ppu->status;
    nes->ppu->status &= 0x7F;   // Clear VBlank flag
    nes->ppu->write_toggle = 0; // Reset write toggle
    // WARNING: Reading PPUSTATUS clears bits 6-7!
    value &= 0xE0; // Only return bits 5-7 (sprite overflow, sprite 0 hit, VBlank)
    break;

  case 0x2004: // OAMDATA
    nes_log_traceback("INFO: OAMDATA read: 0x%02X\n", nes->ppu->oam[nes->ppu->oamaddr]);
    value = nes->ppu->oam[nes->ppu->oamaddr];
    break;

  case 0x2007: // PPUDATA
    value = nes->ppu->buffer;
    nes->ppu->buffer = ppu_read_ram(nes, nes->ppu->v);

    // Palette reads are immediate, VRAM reads are buffered
    if ((nes->ppu->v & 0x3FFF) >= 0x3F00)
    {
      value = nes->ppu->buffer; // Palette reads bypass buffer
    }

    nes->ppu->v += (nes->ppu->ctrl & 0x04) ? 32 : 1;
    break;

  default:
    nes_log_error("ERROR: PPU Register 0x%04X not readable\n", address);
    value = 0; // Open bus behavior
  }

  nes_log_traceback(", Value: 0x%02X\n", value);
  return value;
}

void ppu_write_register(NES *nes, uint16_t address, uint8_t value)
{
  address &= 0x2007; // Handle mirroring

  nes_log_traceback("INFO: PPU Writing register: 0x%04X, Value: 0x%02X\n", address, value);

  switch (address)
  {
  case 0x2000: // PPUCTRL
    nes->ppu->ctrl = value;
    nes->ppu->t = (nes->ppu->t & 0xF3FF) | ((value & 0x03) << 10);
    break;

  case 0x2001: // PPUMASK
    nes->ppu->mask = value;
    break;

  case 0x2003: // OAMADDR
    nes->ppu->oamaddr = value;
    break;

  case 0x2004: // OAMDATA
    nes_log_traceback("INFO: OAMDATA write: 0x%02X\n", value);
    nes->ppu->oam[nes->ppu->oamaddr] = value;
    nes->ppu->oamaddr++; // Wraps after 256 bytes
    break;

  case 0x2005: // PPUSCROLL
    if (nes->ppu->write_toggle == 0)
    {
      nes->ppu->fineXScroll = value & 0x07;
      nes->ppu->t = (nes->ppu->t & 0xFFE0) | (value >> 3);
      nes->ppu->write_toggle = 1;
    }
    else
    {
      nes->ppu->t = (nes->ppu->t & 0x8C1F) |
                    ((value & 0x07) << 12) |
                    ((value & 0xF8) << 2);
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
    nes_log_error("ERROR: PPU Register 0x%04X not writable\n", address);
  }
}