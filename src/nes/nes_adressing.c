#include "nes_adressing.h"

uint8_t nes_immediate(NES *nes)
{
  uint8_t value = nes_read(nes, nes->PC);
  nes->PC += 1;
  return value;
}

uint8_t nes_absolute(NES *nes)
{
  uint16_t address = nes_read(nes, nes->PC) | (nes_read(nes, nes->PC + 1) << 8);
  nes->PC += 2;
  return nes_read(nes, address);
}

uint8_t nes_absolute_x(NES *nes)
{
  uint16_t address = nes_read(nes, nes->PC) | (nes_read(nes, nes->PC + 1) << 8);
  nes->PC += 2;
  return nes_read(nes, address + nes->X);
}

uint8_t nes_absolute_y(NES *nes)
{
  uint16_t address = nes_read(nes, nes->PC) | (nes_read(nes, nes->PC + 1) << 8);
  nes->PC += 2;
  return nes_read(nes, address + nes->Y);
}

uint8_t nes_zero_page(NES *nes)
{
  uint8_t address = nes_read(nes, nes->PC);
  nes->PC += 1;
  return nes_read(nes, address);
}

uint8_t nes_zero_page_x(NES *nes)
{
  uint8_t address = nes_read(nes, nes->PC);
  nes->PC += 1;
  return nes_read(nes, address + nes->X);
}

uint8_t nes_zero_page_y(NES *nes)
{
  uint8_t address = nes_read(nes, nes->PC);
  nes->PC += 1;
  return nes_read(nes, address + nes->Y);
}

uint8_t nes_indirect_x(NES *nes)
{
  uint8_t address = nes_read(nes, nes->PC) + nes->X;
  nes->PC += 1;
  uint16_t pointer = nes_read(nes, address) | (nes_read(nes, address + 1) << 8);
  return nes_read(nes, pointer);
}

uint8_t nes_indirect_y(NES *nes)
{
  uint8_t address = nes_read(nes, nes->PC);
  nes->PC += 1;
  uint16_t pointer = nes_read(nes, address) | (nes_read(nes, address + 1) << 8);
  return nes_read(nes, pointer + nes->Y);
}