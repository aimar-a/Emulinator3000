#include "nes_addressing.h"

uint16_t nes_immediate(NES *nes)
{
  uint16_t address = nes->PC;
  nes_log("INFO: Immediate address read: 0x%04X at PC: 0x%04X\n", address, nes->PC);
  nes->PC += 1;
  return address;
}

uint16_t nes_absolute(NES *nes)
{
  uint16_t address = nes_read(nes, nes->PC) | (nes_read(nes, nes->PC + 1) << 8);
  nes_log("INFO: Absolute address read: 0x%04X at PC: 0x%04X\n", address, nes->PC);
  nes->PC += 2;
  return address;
}

uint16_t nes_absolute_x(NES *nes)
{
  uint16_t address = nes_absolute(nes);
  nes_log("INFO: Absolute X address calculated: 0x%04X\n", address + nes->X);
  return address + nes->X;
}

uint16_t nes_absolute_y(NES *nes)
{
  uint16_t address = nes_absolute(nes);
  nes_log("INFO: Absolute Y address calculated: 0x%04X\n", address + nes->Y);
  return address + nes->Y;
}

uint8_t nes_zero_page(NES *nes)
{
  uint8_t address = nes_read(nes, nes->PC);
  nes_log("INFO: Zero Page address read: 0x%02X at PC: 0x%04X\n", address, nes->PC);
  nes->PC += 1;
  return address;
}

uint8_t nes_zero_page_x(NES *nes)
{
  uint8_t address = nes_read(nes, nes->PC);
  nes_log("INFO: Zero Page X address calculated: 0x%02X\n", (address + nes->X) & 0xFF);
  nes->PC += 1;
  return (address + nes->X) & 0xFF;
}

uint8_t nes_zero_page_y(NES *nes)
{
  uint8_t address = nes_read(nes, nes->PC);
  nes_log("INFO: Zero Page Y address calculated: 0x%02X\n", (address + nes->Y) & 0xFF);
  nes->PC += 1;
  return (address + nes->Y) & 0xFF;
}

uint16_t nes_indirect(NES *nes)
{
  uint8_t address = nes_read(nes, nes->PC);
  nes->PC += 1;
  uint16_t pointer = nes_read(nes, address) | (nes_read(nes, (address + 1) & 0xFF) << 8);
  nes_log("INFO: Indirect pointer calculated: 0x%04X\n", pointer);
  return pointer;
}

uint16_t nes_indirect_x(NES *nes)
{
  uint8_t address = (nes_read(nes, nes->PC) + nes->X) & 0xFF;
  nes->PC += 1;
  uint16_t pointer = nes_read(nes, address) | (nes_read(nes, (address + 1) & 0xFF) << 8);
  nes_log("INFO: Indirect X pointer calculated: 0x%04X\n", pointer);
  return pointer;
}

uint16_t nes_indirect_y(NES *nes)
{
  uint8_t address = nes_read(nes, nes->PC);
  nes->PC += 1;
  uint16_t base = nes_read(nes, address) | (nes_read(nes, (address + 1) & 0xFF) << 8);
  nes_log("INFO: Indirect Y base address calculated: 0x%04X\n", base + nes->Y);
  return base + nes->Y;
}
