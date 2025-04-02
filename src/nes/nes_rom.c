#include "nes_rom.h"

int nes_load_rom(NES *nes, const char *filename)
{
  FILE *file = fopen(filename, "rb");
  if (!file)
  {
    nes_log_error("ERROR: No se pudo abrir la rom: %s\n", filename);
    return 1;
  }

  // Leer cabecera iNES (16 bytes)
  fread(nes->rom->header, 1, 16, file);

  // Verificar que sea un archivo iNES válido
  if (nes->rom->header[0] != 'N' || nes->rom->header[1] != 'E' || nes->rom->header[2] != 'S' || nes->rom->header[3] != 0x1A)
  {
    nes_log_error("ERROR: No es un archivo .nes válido.\n");
    fclose(file);
    return 1;
  }

  // Obtener datos de la cabecera
  nes_get_rom_info(nes);

  // Reservar memoria para PRG-ROM (16 KB por banco)
  nes->rom->prg_rom = malloc(nes->rom->prg_size * 16384);
  if (!nes->rom->prg_rom)
  {
    nes_log_error("ERROR: No se pudo asignar memoria para PRG-rom.\n");
    fclose(file);
    return 1;
  }
  fread(nes->rom->prg_rom, 1, nes->rom->prg_size * 16384, file);

  // Reservar memoria para CHR-ROM (8 KB por banco, si existe)
  if (nes->rom->chr_size > 0)
  {
    nes->rom->chr_rom = malloc(nes->rom->chr_size * 8192);
    if (!nes->rom->chr_rom)
    {
      nes_log_error("ERROR: No se pudo asignar memoria para CHR-rom.\n");
      free(nes->rom->prg_rom);
      fclose(file);
      return 1;
    }
    fread(nes->rom->chr_rom, 1, nes->rom->chr_size * 8192, file);
  }
  else
  {
    nes->rom->chr_rom = NULL; // Algunas ROMs usan CHR-RAM en lugar de CHR-ROM
  }

  fclose(file);
  nes_log_traceback("INFO: ROM cargada correctamente\n");
  return 0;
}

void nes_get_rom_info(NES *nes)
{
  nes_log_traceback("INFO: Getting ROM info\n");

  // Cantidad de bancos de PRG-ROM (16KB cada uno)
  nes->rom->prg_size = nes->rom->header[4];

  // Cantidad de bancos de CHR-ROM (8KB cada uno)
  nes->rom->chr_size = nes->rom->header[5];

  // Mirroring (bit 0 de header[6])
  nes->rom->mirroring = (nes->rom->header[6] & 0x01) ? MIRROR_VERTICAL : MIRROR_HORIZONTAL;

  // Si la ROM usa batería para PRG-RAM (bit 1 de header[6])
  nes->rom->has_battery = (nes->rom->header[6] & 0x02) ? true : false;

  // Si la ROM incluye un trainer de 512 bytes (bit 2 de header[6])
  nes->rom->has_trainer = (nes->rom->header[6] & 0x04) ? true : false;

  // Si usa VRAM de 4 pantallas (bit 3 de header[6])
  nes->rom->four_screen = (nes->rom->header[6] & 0x08) ? true : false;

  // ID del Mapper (bits 4-7 de header[6] y bits 4-7 de header[7])
  nes->rom->mapper = ((nes->rom->header[7] & 0xF0) | (nes->rom->header[6] >> 4));

  // Tamaño de PRG-RAM (Byte 8, en bloques de 8KB, 0 significa 8KB por defecto)
  nes->rom->prg_ram_size = nes->rom->header[8] ? (nes->rom->header[8]) : 1;

  // Tamaño de CHR-RAM (Si CHR-ROM es 0, significa que usa CHR-RAM)
  if (nes->rom->chr_size == 0)
  {
    nes->rom->chr_ram_size = 1; // 8KB por defecto si no hay CHR-ROM
  }
  else
  {
    nes->rom->chr_ram_size = 0; // No usa CHR-RAM
  }

  // Determinar si es un archivo iNES 2.0
  bool is_ines2 = ((nes->rom->header[7] & 0x0C) == 0x08);

  // Mensaje de salida con la información de la ROM
  nes_log_instant("INFO: PRG-ROM: %d KB\n", nes->rom->prg_size * 16);
  nes_log_instant("INFO: CHR-ROM: %d KB\n", nes->rom->chr_size * 8);
  nes_log_instant("INFO: Mapper: %d\n", nes->rom->mapper);
  nes_log_instant("INFO: Mirroring: %s\n", nes->rom->mirroring == MIRROR_VERTICAL ? "Vertical" : "Horizontal");
  nes_log_instant("INFO: Battery: %s\n", nes->rom->has_battery ? "Yes" : "No");
  nes_log_instant("INFO: Trainer: %s\n", nes->rom->has_trainer ? "Yes" : "No");
  nes_log_instant("INFO: Four-Screen VRAM: %s\n", nes->rom->four_screen ? "Yes" : "No");
  nes_log_instant("INFO: PRG-RAM: %d KB\n", nes->rom->prg_ram_size * 8);
  nes_log_instant("INFO: CHR-RAM: %d KB\n", nes->rom->chr_ram_size ? 8 : 0);
  nes_log_instant("INFO: Format: %s\n", is_ines2 ? "iNES 2.0" : "iNES 1.0");
}
