#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "nes_memory.h"

uint8_t nes_read(NES *nes, uint16_t address)
{
  if (address < 0x10000)
  {
    return nes->memory[address];
  }
  else
  {
    printf("NO IMPLEMENTADO, Read from ROM: %04X\n", address);
  }
}

void nes_write(NES *nes, uint16_t address, uint8_t value)
{
  if (address < 0x10000)
  {
    nes->memory[address] = value;
  }
  else
  {
    printf("NO IMPLEMENTADO, Write to ROM: %04X\n", address);
  }
}
