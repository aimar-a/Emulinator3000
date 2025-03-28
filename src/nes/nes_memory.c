#include "nes_memory.h"

uint8_t nes_read(NES *nes, uint16_t address)
{
  nes_log("INFO: CPU Reading address: 0x%04X", address);
  uint8_t value = 0;
  if (address < 0x8000)
  {
    if ((address >= 0x2000 && address <= 0x2007) || address == 0x4014)
    {
      value = ppu_read_register(nes, address);
    }
    else
    {
      value = nes->memory[address];
    }
  }
  else if (address < 0x10000)
  {
    value = nes->rom->prg_rom[address - 0x8000];
  }
  else
  {
    nes_log("ERROR: Invalid memory address: 0x%04X\n", address);
    exit(1);
  }

  nes_log(", Value read: 0x%02X\n", value);
  return value;
}

uint16_t nes_read_address(NES *nes, uint16_t address)
{
  return nes_read(nes, address) | (nes_read(nes, address + 1) << 8);
}

void nes_write(NES *nes, uint16_t address, uint8_t value)
{
  nes_log("INFO: CPU Writing address: 0x%04X, value: 0x%02X\n", address, value);
  if (address < 0x8000)
  {
    if ((address >= 0x2000 && address <= 0x2007) || address == 0x4014)
    {
      ppu_write_register(nes, address, value);
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
    nes_log("ERROR: Invalid memory address: 0x%04X\n", address);
    exit(1);
  }
}
