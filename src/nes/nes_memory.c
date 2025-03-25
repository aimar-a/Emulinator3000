#include "nes_memory.h"

uint8_t nes_read(NES *nes, uint16_t address)
{
  // printf("Reading address: %04X\n", address);
  if (address < 0x8000)
  {
    if (address == 0x2000)
    {
      printf("ERROR: PPUCTRL is not readable\n");
      exit(1);
      return nes->ppu->ctrl;
    }
    else if (address == 0x2001)
    {
      printf("ERROR: PPUMASK is not readable\n");
      exit(1);
      return nes->ppu->mask;
    }
    else if (address == 0x2002)
    {
      uint8_t status = nes->ppu->status;
      nes->ppu->status &= ~0x80; // Borrar flag de VBlank
      return status;
    }
    else if (address == 0x2003)
    {
      printf("ERROR: OAMADDR is not readable\n");
      exit(1);
      return nes->ppu->oamaddr;
    }
    else if (address == 0x2004)
    {
      return nes->ppu->oamdata;
    }
    else if (address == 0x2005)
    {
      printf("ERROR: PPUSCROLL is not readable\n");
      exit(1);
      return nes->ppu->scroll;
    }
    else if (address == 0x2006)
    {
      printf("ERROR: PPUADDR is not readable\n");
      exit(1);
      return nes->ppu->addr;
    }
    else if (address == 0x2007)
    {
      return nes->ppu->data;
    }
    else if (address == 0x4014)
    {
      printf("ERROR: OAMDMA is not readable\n");
      exit(1);
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
      // Write to PPUCTRL: Set NMI_output to bit 7.
      nes->ppu->ctrl = value;
    }
    else if (address == 0x2001)
    {
      nes->ppu->mask = value;
    }
    else if (address == 0x2002)
    {
      printf("ERROR: PPUSTATUS is not writable\n");
      exit(1);
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
