#include "nes_memory.h"

uint8_t nes_read(NES *nes, uint16_t address)
{
  nes_log_traceback("INFO: CPU Reading address: 0x%04X", address);
  uint8_t value = 0;

  if (address < 0x2000)
  {
    nes_log_traceback(" (RAM)");
    value = nes->memory[address % 0x0800]; // Handles mirrors
  }
  else if (address < 0x4000)
  {
    nes_log_traceback(" (PPU Registers)");
    value = ppu_read_register(nes, 0x2000 + (address % 8));
  }
  else if (address == 0x4014)
  {
    nes_log_traceback(" (OAMDMA)");
    nes_log_instant("WARNING: OAMDMA reads typically return open bus");
    value = 0; // Open bus behavior would go here
  }
  else if (address == 0x4016 || address == 0x4017)
  {
    nes_log_traceback(" (Controller I/O)");
    value = nes_read_controller(nes, address);
  }
  else if (address >= 0x4000 && address < 0x4014)
  {
    nes_log_traceback(" (APU Registers)");
    value = nes_apu_read(nes->apu, address);
  }
  else if (address == 0x4015)
  {
    nes_log_traceback(" (APU Status)");
    value = nes_apu_read(nes->apu, address);
  }
  else if (address >= 0x6000 && address < 0x8000)
  {
    nes_log_traceback(" (SRAM)");
    value = nes->sram[address - 0x6000];
  }
  else if (address >= 0x8000)
  {
    nes_log_traceback(" (PRG-ROM)");
    uint32_t prg_size = nes->rom->prg_size * 0x4000;
    uint32_t prg_offset = address - 0x8000;

    if (prg_size == 0x4000)
    {
      value = nes->rom->prg_rom[prg_offset % 0x4000];
    }
    else if (prg_size == 0x8000)
    {
      value = nes->rom->prg_rom[prg_offset];
    }
    else
    {
      nes_log_error("ERROR: Invalid PRG size: %d bytes\n", prg_size);
    }
  }
  else
  {
    nes_log_traceback(" (Unmapped)");
    nes_log_error("ERROR: Unmapped read at 0x%04X\n", address);
  }

  nes_log_traceback(", Value read: 0x%02X\n", value);
  return value;
}

uint16_t nes_read_address(NES *nes, uint16_t address)
{
  return nes_read(nes, address) | (nes_read(nes, address + 1) << 8);
}

void nes_write(NES *nes, uint16_t address, uint8_t value)
{
  nes_log_traceback("INFO: CPU Writing address: 0x%04X", address);

  if (address < 0x2000)
  {
    nes_log_traceback(" (RAM)");
    nes->memory[address % 0x0800] = value;
  }
  else if (address < 0x4000)
  {
    nes_log_traceback(" (PPU Registers)");
    ppu_write_register(nes, 0x2000 + (address % 8), value);
  }
  else if (address == 0x4014)
  {
    nes_log_traceback(" (OAMDMA)");
    ppu_oamdma_transfer(nes, value);
  }
  else if (address == 0x4016 || address == 0x4017)
  {
    nes_log_traceback(" (Controller I/O)");
    nes_write_controller(nes, address, value);
  }
  else if (address >= 0x4000 && address < 0x4014)
  {
    nes_log_traceback(" (APU Registers)");
    nes_apu_write(nes->apu, address, value);
  }
  else if (address == 0x4015)
  {
    nes_log_traceback(" (APU Status)");
    nes_apu_write(nes->apu, address, value);
  }
  else if (address == 0x4017)
  {
    nes_log_traceback(" (APU Frame Counter)");
    nes_apu_write(nes->apu, address, value);
  }
  else if (address >= 0x6000 && address < 0x8000)
  {
    nes_log_traceback(" (SRAM)");
    nes->sram[address - 0x6000] = value;
  }
  else if (address >= 0x8000)
  {
    nes_log_traceback(" (PRG-ROM)");
    // Normally read-only, log a warning
    nes_log_instant("WARNING: Attempted write to PRG-ROM at 0x%04X\n", address);
    // Some mappers use this for bank switching - you'd handle that here
  }
  else
  {
    nes_log_traceback(" (Unmapped)");
    nes_log_error("ERROR: Unmapped write at 0x%04X\n", address);
  }

  nes_log_traceback(", Value written: 0x%02X\n", value);
}
