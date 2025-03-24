#include "nes_adressing.h"

uint16_t nes_immediate(NES *nes)
{
  uint16_t address = nes->PC;
  nes->PC++;
  return address;
}

uint16_t nes_absolute(NES *nes)
{
  uint16_t address = nes_read(nes, nes->PC) | (nes_read(nes, nes->PC + 1) << 8);
  nes->PC += 2;
  return address;
}

uint16_t nes_absolute_x(NES *nes)
{
  uint16_t address = nes_read(nes, nes->PC) | (nes_read(nes, nes->PC + 1) << 8);
  nes->PC += 2;
  return address + nes->X;
}

uint16_t nes_absolute_y(NES *nes)
{
  uint16_t address = nes_read(nes, nes->PC) | (nes_read(nes, nes->PC + 1) << 8);
  nes->PC += 2;
  return address + nes->Y;
}

uint16_t nes_indirect_x(NES *nes)
{
  uint16_t address = nes_read(nes, nes->PC) + nes->X;
  nes->PC++;
  return nes_read(nes, address) | (nes_read(nes, address + 1) << 8);
}

uint16_t nes_indirect_y(NES *nes)
{
  uint16_t address = nes_read(nes, nes->PC);
  nes->PC++;
  return (nes_read(nes, address) | (nes_read(nes, address + 1) << 8)) + nes->Y;
}

uint16_t nes_zero_page(NES *nes)
{
  uint16_t address = nes_read(nes, nes->PC);
  nes->PC++;
  return address;
}

uint16_t nes_zero_page_x(NES *nes)
{
  uint16_t address = nes_read(nes, nes->PC) + nes->X;
  nes->PC++;
  return address;
}

uint16_t nes_zero_page_y(NES *nes)
{
  uint16_t address = nes_read(nes, nes->PC) + nes->Y;
  nes->PC++;
  return address;
}