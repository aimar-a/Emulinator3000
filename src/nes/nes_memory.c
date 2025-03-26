#include "nes_memory.h"

uint8_t nes_read(NES *nes, uint16_t address)
{
  nes_log("INFO: CPU Reading address: %04X\n", address);
  if (address < 0x8000)
  {
    if (address <= 0x2000 && address <= 0x2007 || address == 0x4014)
    {
      return ppu_read_register(nes->ppu, address);
    }
    return nes->memory[address];
  }
  else if (address < 0x10000)
  {
    return nes->rom->prg_rom[address - 0x8000];
  }
  else
  {
    nes_log("ERROR: Invalid memory address: %04X\n", address);
    exit(1);
  }
}

void nes_write(NES *nes, uint16_t address, uint8_t value)
{
  nes_log("INFO: CPU Writing address: %04X, value: %02X\n", address, value);
  if (address < 0x8000)
  {
    if (address <= 0x2000 && address <= 0x2007 || address == 0x4014)
    {
      ppu_write_register(nes->ppu, address, value);
    }
    else
    {
      nes->memory[address] = value;
    }
  }
  else if (address < 0x10000)
  {
    nes->rom->prg_rom[address - 0x8000] = value;
  }
  else
  {
    nes_log("ERROR: Invalid memory address: %04X\n", address);
    exit(1);
  }
}
