#include "nes_memory.h"

uint8_t nes_read(NES *nes, uint16_t address)
{
  nes_log_traceback("INFO: CPU Reading address: 0x%04X", address);
  uint8_t value = 0;
  if (address < 0x0100)
  {
    nes_log_traceback(" (Zero Page)");
    value = nes->memory[address];
  }
  else if (address < 0x0200)
  {
    nes_log_traceback(" (Stack)");
    value = nes->memory[address];
  }
  else if (address < 0x0800)
  {
    nes_log_traceback(" (RAM)");
    value = nes->memory[address];
  }
  else if (address < 0x2000)
  {
    nes_log_traceback(" (Mirrors $0000-$07FF)");
    nes_log_instant("WARNING: Reading from RAM mirrors is not implemented yet.\n");
    // TODO
  }
  else if (address < 0x2008)
  {
    nes_log_traceback(" (PPU Registers)");
    value = ppu_read_register(nes, address);
  }
  else if (address < 0x4000)
  {
    nes_log_traceback(" (PPU Registers Mirrors $2000-$2007)");
    nes_log_instant("WARNING: Reading from PPU registers mirrors is not implemented yet.\n");
    // TODO
  }
  else if (address < 0x4016)
  {
    nes_log_traceback(" (APU and I/O Registers)");
    nes_log_instant("WARNING: Reading from APU and I/O registers is not implemented yet.\n");
    // TODO
  }
  else if (address < 0x4018)
  {
    nes_log_traceback(" (APU and I/O Registers Mirrors $4014-$4017)");
    nes_log_instant("WARNING: Reading from APU and I/O registers mirrors is not implemented yet.\n");
    // TODO
  }
  else if (address < 0x4020)
  {
    nes_log_traceback(" (APU and I/O Registers)");
    nes_log_instant("WARNING: Reading from APU and I/O registers is not implemented yet.\n");
    // TODO
  }
  else if (address < 0x6000)
  {
    nes_log_traceback(" (Expansion ROM)");
    nes_log_instant("WARNING: Reading from Expansion ROM is not implemented yet.\n");
    // TODO
  }
  else if (address < 0x8000)
  {
    nes_log_traceback(" (SRAM)");
    nes_log_instant("WARNING: Reading from PRG RAM is not implemented yet.\n");
    // TODO
  }
  else if (address < 0xC000)
  {
    nes_log_traceback(" (PRG-ROM Lower Bank)");
    if (nes->rom->prg_size * 16 == 16)
    {
      value = nes->rom->prg_rom[(address - 0x8000) % (16 * 1024)];
    }
    else if (nes->rom->prg_size * 16 == 32)
    {
      value = nes->rom->prg_rom[address - 0x8000];
    }
    else
    {
      nes_log_error("ERROR: Invalid PRG size: %d KB\n", nes->rom->prg_size * 16);
      exit(1);
    }
  }
  else if (address < 0x10000)
  {
    nes_log_traceback(" (PRG-ROM Upper Bank)");
    if (nes->rom->prg_size * 16 == 16)
    {
      value = nes->rom->prg_rom[(address - 0x8000) % (16 * 1024)];
    }
    else if (nes->rom->prg_size * 16 == 32)
    {
      value = nes->rom->prg_rom[address - 0x8000];
    }
    else
    {
      nes_log_error("ERROR: Invalid PRG size: %d KB\n", nes->rom->prg_size * 16);
      exit(1);
    }
  }
  else
  {
    nes_log_traceback(" (Invalid address)");
    nes_log_error("ERROR: Invalid memory address: 0x%04X\n", address);
    exit(1);
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

  if (address < 0x0100)
  {
    nes_log_traceback(" (Zero Page)");
    nes->memory[address] = value;
  }
  else if (address < 0x0200)
  {
    nes_log_traceback(" (Stack)");
    nes->memory[address] = value;
  }
  else if (address < 0x0800)
  {
    nes_log_traceback(" (RAM)");
    nes->memory[address] = value;
  }
  else if (address < 0x2000)
  {
    nes_log_traceback(" (Mirrors $0000-$07FF)");
    nes_log_instant("WARNING: Writing to RAM mirrors is not implemented yet.\n");
    // TODO
  }
  else if (address < 0x2008)
  {
    nes_log_traceback(" (PPU Registers)");
    ppu_write_register(nes, address, value);
  }
  else if (address < 0x4000)
  {
    nes_log_traceback(" (PPU Registers Mirrors $2000-$2007)");
    nes_log_instant("WARNING: Writing to PPU registers mirrors is not implemented yet.\n");
    // TODO
  }
  else if (address < 0x4016)
  {
    nes_log_traceback(" (APU and I/O Registers)");
    nes_log_instant("WARNING: Writing to APU and I/O registers is not implemented yet.\n");
    // TODO
  }
  else if (address < 0x4018)
  {
    nes_log_traceback(" (APU and I/O Registers Mirrors $4014-$4017)");
    nes_log_instant("WARNING: Writing to APU and I/O registers mirrors is not implemented yet.\n");
    // TODO
  }
  else if (address < 0x4020)
  {
    nes_log_traceback(" (APU and I/O Registers)");
    nes_log_instant("WARNING: Writing to APU and I/O registers is not implemented yet.\n");
    // TODO
  }
  else if (address < 0x6000)
  {
    nes_log_traceback(" (Expansion ROM)");
    nes_log_instant("WARNING: Writing to Expansion ROM is not implemented yet.\n");
    // TODO
  }
  else if (address < 0x8000)
  {
    nes_log_traceback(" (SRAM)");
    nes_log_instant("WARNING: Writing to PRG RAM is not implemented yet.\n");
    // TODO
  }
  else if (address < 0xC000)
  {
    nes_log_traceback(" (PRG-ROM Lower Bank)");
    if (nes->rom->prg_size * 16 == 16)
    {
      nes->rom->prg_rom[(address - 0x8000) % (16 * 1024)] = value;
    }
    else if (nes->rom->prg_size * 16 == 32)
    {
      nes->rom->prg_rom[address - 0x8000] = value;
    }
    else
    {
      nes_log_error("ERROR: Invalid PRG size: %d KB\n", nes->rom->prg_size * 16);
      exit(1);
    }
  }
  else if (address < 0x10000)
  {
    nes_log_traceback(" (PRG-ROM Upper Bank)");
    if (nes->rom->prg_size * 16 == 16)
    {
      nes->rom->prg_rom[(address - 0x8000) % (16 * 1024)] = value;
    }
    else if (nes->rom->prg_size * 16 == 32)
    {
      nes->rom->prg_rom[address - 0x8000] = value;
    }
    else
    {
      nes_log_error("ERROR: Invalid PRG size: %d KB\n", nes->rom->prg_size * 16);
      exit(1);
    }
  }
  else
  {
    nes_log_traceback(" (Invalid address)");
    nes_log_error("ERROR: Invalid memory address: 0x%04X\n", address);
    exit(1);
  }

  nes_log_traceback(", Value written: 0x%02X\n", value);
}
