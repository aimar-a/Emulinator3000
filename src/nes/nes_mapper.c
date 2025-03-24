#include "nes_mapper.h"

void nes_rom_to_memory(NES *nes)
{
  if (!nes->memory)
  {
    printf("Error: nes->memory no está inicializado\n");
    exit(1);
  }
  if (!nes->rom->prg_rom)
  {
    printf("Error: nes->rom->prg_rom no está inicializado\n");
    exit(1);
  }

  // Cargar banco intercambiable en $8000-$BFFF (Primer banco)
  memcpy(nes->memory + 0x8000, nes->rom->prg_rom, 0x4000);

  // Cargar último banco en $C000-$FFFF (Último banco siempre fijo)
  memcpy(nes->memory + 0xC000, nes->rom->prg_rom + nes->rom->prg_size - 0x4000, 0x4000);

  // Cargar bancos de 8 KB de CHR-ROM en $0000-$1FFF
  // memcpy(nes->ppu.vram, nes->rom->chr_rom, nes->rom->chr_size * 8192);

  return;

  // por ahora no vamos a usar mappers q son una movida
  // vamos a hacer q funcione con el super mario bros q no necesita mappers
  // y luego veremos como implementar los mappers

  switch (nes->current_mapper)
  {
  case 0:
    init_nrom(nes);
    break;
  case 1:
    init_mmc1(nes);
    break;
  case 2:
    init_unrom(nes);
    break;
  case 3:
    init_cnrom(nes);
    break;
  case 4:
    init_mmc3(nes);
    break;
  default:
    printf("Unsupported mapper: %d\n", nes->current_mapper);
    exit(1);
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
