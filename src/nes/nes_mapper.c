#include "nes_mapper.h"

void nes_rom_to_memory(NES *nes)
{
  if (!nes->memory)
  {
    nes_log_error("ERROR: nes->memory no está inicializado\n");
  }
  if (!nes->rom->prg_rom)
  {
    nes_log_error("ERROR: nes->rom->prg_rom no está inicializado\n");
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

  switch (nes->rom->mapper)
  {
  case 0:
    // TODO
    // init_nrom(nes);
    break;
  case 1:
    // TODO
    // init_mmc1(nes);
    break;
  case 2:
    // TODO
    // init_unrom(nes);
    break;
  case 3:
    // TODO
    // init_cnrom(nes);
    break;
  case 4:
    // TODO
    // init_mmc3(nes);
    break;
  default:
    nes_log_error("ERROR: Unsupported mapper: %d\n", nes->rom->mapper);
  }
}
