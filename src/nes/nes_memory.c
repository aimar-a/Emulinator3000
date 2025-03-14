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
  else
  {
    return nes->memory[address % 0x8000];
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
    nes->memory[address % 0x8000] = value;
  }
}

void init_nrom(NES *nes)
{

  // Para NROM no hay una configuración compleja, ya que es muy sencillo
  nes->mapper_bank = 0;
  nes->chr_bank = 0;
  nes->interrupt_enable = false;
  nes->mapper_state = 0;
}

uint8_t read_nrom(NES *nes, uint16_t address)
{
  if (address < 0x8000)
  {
    return nes->memory[address]; // Leer de la RAM
  }
  else
  {
    return nes->memory[address]; // Leer de la ROM
  }
}

void write_nrom(NES *nes, uint16_t address, uint8_t value)
{
  // No se permite escribir en la ROM para NROM
  // Sin embargo, puede haber áreas de RAM en el rango 0x0000 a 0x07FF
  if (address < 0x0800)
  {
    nes->memory[address] = value; // Escribir en RAM
  }
}
void init_mmc1(NES *nes)
{
  nes->mapper_state = 0; // Inicia en el estado base
}

uint8_t read_mmc1(NES *nes, uint16_t address)
{
  if (address < 0x8000)
  {
    return nes->memory[address]; // Leer de la RAM
  }
  else
  {
    // Leer de la ROM, dependiendo del banco mapeado
    return nes->memory[nes->mapper_bank + (address - 0x8000)];
  }
}

void write_mmc1(NES *nes, uint16_t address, uint8_t value)
{
  // Manipula el registro de control y actualiza el banco de memoria
  if (address >= 0x8000)
  {
    nes->mapper_state = (nes->mapper_state + 1) % 4;
    if (nes->mapper_state == 0)
    {
      nes->mapper_bank = value & 0x7F;
    }
  }
}

void init_unrom(NES *nes)
{
  nes->mapper_bank = 0;
}

uint8_t read_unrom(NES *nes, uint16_t address)
{
  if (address < 0x8000)
  {
    return nes->memory[address]; // Leer de la RAM
  }
  else
  {
    return nes->memory[nes->mapper_bank + (address - 0x8000)];
  }
}

void write_unrom(NES *nes, uint16_t address, uint8_t value)
{
  // UNROM solo cambia el banco si la escritura está en un rango específico
  if (address >= 0x8000)
  {
    nes->mapper_bank = value & 0x01; // Solo tiene 2 bancos posibles
  }
}

void init_cnrom(NES *nes)
{
  nes->mapper_bank = 0; // Banco de gráficos (CHR) inicial
}

uint8_t read_cnrom(NES *nes, uint16_t address)
{
  if (address < 0x8000)
  {
    return nes->memory[address]; // Leer de la RAM
  }
  else if (address >= 0x8000 && address < 0x10000)
  {
    // Leer de la ROM (PRG)
    return nes->memory[nes->mapper_bank + (address - 0x8000)];
  }
  return 0;
}

void write_cnrom(NES *nes, uint16_t address, uint8_t value)
{
  // CNROM solo cambia los bancos de gráficos
  if (address >= 0x8000)
  {
    nes->mapper_bank = value & 0x03; // Hasta 4 bancos de 8KB
  }
}

void init_mmc3(NES *nes)
{
  nes->mapper_bank = 0; // Banco de PRG inicial
  nes->chr_bank = 0;    // Banco de CHR inicial
  nes->interrupt_enable = false;
}

uint8_t read_mmc3(NES *nes, uint16_t address)
{
  if (address < 0x8000)
  {
    return nes->memory[address]; // Leer de la RAM
  }
  else if (address < 0xA000)
  {
    return nes->memory[nes->mapper_bank + (address - 0x8000)]; // Leer PRG
  }
  else if (address < 0xC000)
  {
    return nes->memory[nes->chr_bank + (address - 0xA000)]; // Leer CHR
  }
  return 0;
}

void write_mmc3(NES *nes, uint16_t address, uint8_t value)
{
  if (address >= 0x8000)
  {
    if (address < 0xA000)
    {
      // Actualiza banco de PRG
      nes->mapper_bank = value & 0x0F;
    }
    else if (address < 0xC000)
    {
      // Actualiza banco de CHR
      nes->chr_bank = value & 0x03;
    }
    else if (address < 0xE000)
    {
      // Interrupción
      nes->interrupt_enable = value & 0x01;
    }
  }
}
