#include "nes_addressing.h"

uint16_t nes_immediate(NES *nes)
{
  uint16_t address = nes->PC;
  nes_log_traceback("INFO: Immediate address read: 0x%04X at PC: 0x%04X\n", address, nes->PC);
  nes->PC += 1;
  return address;
}

uint16_t nes_absolute(NES *nes)
{
  uint16_t address = nes_read(nes, nes->PC) | (nes_read(nes, nes->PC + 1) << 8);
  nes_log_traceback("INFO: Absolute address read: 0x%04X at PC: 0x%04X\n", address, nes->PC);
  nes->PC += 2;
  return address;
}

uint16_t nes_absolute_x(NES *nes)
{
  uint16_t address = nes_absolute(nes);
  nes_log_traceback("INFO: Absolute X address calculated: 0x%04X\n", address + nes->X);
  return address + nes->X;
}

uint16_t nes_absolute_y(NES *nes)
{
  uint16_t address = nes_absolute(nes);
  nes_log_traceback("INFO: Absolute Y address calculated: 0x%04X\n", address + nes->Y);
  return address + nes->Y;
}

uint8_t nes_zero_page(NES *nes)
{
  uint8_t address = nes_read(nes, nes->PC);
  nes_log_traceback("INFO: Zero Page address read: 0x%02X at PC: 0x%04X\n", address, nes->PC);
  nes->PC += 1;
  return address;
}

uint8_t nes_zero_page_x(NES *nes)
{
  uint8_t address = nes_read(nes, nes->PC);
  nes_log_traceback("INFO: Zero Page X address calculated: 0x%02X\n", (address + nes->X) & 0xFF);
  nes->PC += 1;
  return (address + nes->X) & 0xFF;
}

uint8_t nes_zero_page_y(NES *nes)
{
  uint8_t address = nes_read(nes, nes->PC);
  nes_log_traceback("INFO: Zero Page Y address calculated: 0x%02X\n", (address + nes->Y) & 0xFF);
  nes->PC += 1;
  return (address + nes->Y) & 0xFF;
}

uint16_t nes_indirect(NES *nes)
{
  uint16_t addr = (nes_read(nes, nes->PC) | nes_read(nes, nes->PC + 1) << 8);
  nes->PC += 2;

  // Emular el bug del 6502 al hacer JMP ($xxFF)
  uint8_t ptr_low = nes_read(nes, addr);
  uint8_t ptr_high;
  if ((addr & 0x00FF) == 0x00FF)
  {
    // Bug: la página no avanza, se queda en xx00 en vez de xx+1 00
    ptr_high = nes_read(nes, addr & 0xFF00);
  }
  else
  {
    ptr_high = nes_read(nes, addr + 1);
  }

  uint16_t pointer = (ptr_high << 8) | ptr_low;

  nes_log_traceback("INFO: Indirect pointer calculated: 0x%04X\n", pointer);
  return pointer;
}

uint16_t nes_indirect_x(NES *nes)
{
  uint8_t address = (nes_read(nes, nes->PC) + nes->X) & 0xFF;
  nes->PC += 1;
  uint16_t pointer = nes_read(nes, address) | (nes_read(nes, (address + 1) & 0xFF) << 8);
  nes_log_traceback("INFO: Indirect X pointer calculated: 0x%04X\n", pointer);
  return pointer;
}

uint16_t nes_indirect_y(NES *nes)
{
  uint8_t address = nes_read(nes, nes->PC);
  nes->PC += 1;
  uint16_t base = nes_read(nes, address) | (nes_read(nes, (address + 1) & 0xFF) << 8);
  nes_log_traceback("INFO: Indirect Y base address calculated: 0x%04X\n", base + nes->Y);
  return base + nes->Y;
}
