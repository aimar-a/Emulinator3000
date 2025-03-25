#include "nes_addressing.h"

uint8_t nes_immediate(NES *nes)
{
  uint8_t value = nes_read(nes, nes->PC);
  nes->PC += 1;
  return value;
}

uint16_t nes_absolute(NES *nes)
{
  uint16_t address = nes_read(nes, nes->PC) | (nes_read(nes, nes->PC + 1) << 8);
  nes->PC += 2;
  return address;
}

uint16_t nes_absolute_x(NES *nes)
{
  uint16_t address = nes_absolute(nes);
  return address + nes->X;
}

uint16_t nes_absolute_y(NES *nes)
{
  uint16_t address = nes_absolute(nes);
  return address + nes->Y;
}

uint8_t nes_zero_page(NES *nes)
{
  uint8_t address = nes_read(nes, nes->PC);
  nes->PC += 1;
  return address;
}

uint8_t nes_zero_page_x(NES *nes)
{
  uint8_t address = nes_read(nes, nes->PC);
  nes->PC += 1;
  return (address + nes->X) & 0xFF;
}

uint8_t nes_zero_page_y(NES *nes)
{
  uint8_t address = nes_read(nes, nes->PC);
  nes->PC += 1;
  return (address + nes->Y) & 0xFF;
}

uint16_t nes_indirect_x(NES *nes)
{
  uint8_t address = (nes_read(nes, nes->PC) + nes->X) & 0xFF;
  nes->PC += 1;
  uint16_t pointer = nes_read(nes, address) | (nes_read(nes, (address + 1) & 0xFF) << 8);
  return pointer;
}

uint16_t nes_indirect_y(NES *nes)
{
  uint8_t address = nes_read(nes, nes->PC);
  nes->PC += 1;
  uint16_t base = nes_read(nes, address) | (nes_read(nes, (address + 1) & 0xFF) << 8);
  return base + nes->Y;
}
