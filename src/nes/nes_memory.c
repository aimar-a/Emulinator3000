#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "nes_memory.h"

uint8_t nes_read(NES *nes, uint16_t address)
{
  if (address < 0x2000)
  {
    return nes->memory[address % 0x0800];
  }
  else if (address < 0x4000)
  {
    return nes->memory[0x2000 + (address % 8)];
  }
  else if (address == 0x4015)
  {
    return nes->memory[0x4015];
  }
  else if (address == 0x4016)
  {
    return nes->memory[0x4016];
  }
  else if (address == 0x4017)
  {
    return nes->memory[0x4017];
  }
  else if (address < 0x6000)
  {
    return 0;
  }
  else if (address < 0x8000)
  {
    return nes->memory[address];
  }
  else if (address >= 0x8000)
  {
    return nes->rom->prg_rom[nes->mapper_bank + (address - 0x8000)];
  }
}

void nes_write(NES *nes, uint16_t address, uint8_t value)
{
  if (address < 0x2000)
  {
    nes->memory[address % 0x0800] = value;
  }
  else if (address < 0x4000)
  {
    nes->memory[0x2000 + (address % 8)] = value;
  }
  else if (address == 0x4014)
  {
    nes->memory[0x4014] = value;
  }
  else if (address == 0x4015)
  {
    nes->memory[0x4015] = value;
  }
  else if (address == 0x4016)
  {
    nes->memory[0x4016] = value;
  }
  else if (address == 0x4017)
  {
    nes->memory[0x4017] = value;
  }
  else if (address < 0x6000)
  {
  }
  else if (address < 0x8000)
  {
    nes->memory[address] = value;
  }
  else
  {
    nes->rom->prg_rom[nes->mapper_bank + (address - 0x8000)] = value;
  }
}
