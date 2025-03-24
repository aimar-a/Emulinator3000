#include "nes_memory.h"

uint8_t nes_read(NES *nes, uint16_t address)
{
  // printf("Reading address: %04X\n", address);
  if (address < 0x8000)
  {
    if (address == 0x2000)
    {
      return nes->ppu->ctrl;
    }
    else if (address == 0x2001)
    {
      return nes->ppu->mask;
    }
    else if (address == 0x2002)
    {
      return nes->ppu->status;
    }
    else if (address == 0x2003)
    {
      return nes->ppu->oamaddr;
    }
    else if (address == 0x2004)
    {
      return nes->ppu->oamdata;
    }
    else if (address == 0x2005)
    {
      return nes->ppu->scroll;
    }
    else if (address == 0x2006)
    {
      return nes->ppu->addr;
    }
    else if (address == 0x2007)
    {
      return nes->ppu->data;
    }
    else if (address == 0x4014)
    {
      return nes->ppu->dma;
    }

    return nes->memory[address];
  }
  else
  {
    return nes->rom->prg_rom[address - 0x8000];
  }
}

void nes_write(NES *nes, uint16_t address, uint8_t value)
{
  // printf("Writing address: %04X, value: %02X\n", address, value);
  if (address < 0x8000)
  {
    if (address == 0x2000)
    {
      nes->ppu->ctrl = value;
    }
    else if (address == 0x2001)
    {
      nes->ppu->mask = value;
    }
    else if (address == 0x2002)
    {
      nes->ppu->status = value;
    }
    else if (address == 0x2003)
    {
      nes->ppu->oamaddr = value;
    }
    else if (address == 0x2004)
    {
      nes->ppu->oamdata = value;
    }
    else if (address == 0x2005)
    {
      nes->ppu->scroll = value;
    }
    else if (address == 0x2006)
    {
      nes->ppu->addr = value;
    }
    else if (address == 0x2007)
    {
      nes->ppu->data = value;
    }
    else if (address == 0x4014)
    {
      nes->ppu->dma = value;
    }
    nes->memory[address] = value;
  }
  else
  {
    nes->rom->prg_rom[address - 0x8000] = value;
  }
}
