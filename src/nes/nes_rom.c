#include "nes_rom.h"

void nes_load_rom(NES *nes, const char *filename)
{
  FILE *file = fopen(filename, "rb");
  if (!file)
  {
    printf("Error: No se pudo abrir la rom->\n");
  }

  // Leer cabecera iNES (16 bytes)
  fread(nes->rom->header, 1, 16, file);

  // Verificar que sea un archivo iNES válido
  if (nes->rom->header[0] != 'N' || nes->rom->header[1] != 'E' || nes->rom->header[2] != 'S' || nes->rom->header[3] != 0x1A)
  {
    printf("Error: No es un archivo .nes válido.\n");
    fclose(file);
  }

  // Obtener tamaño de PRG-ROM y CHR-ROM
  nes->rom->prg_size = nes->rom->header[4]; // Cantidad de bancos de 16 KB
  nes->rom->chr_size = nes->rom->header[5]; // Cantidad de bancos de 8 KB
  nes->current_mapper = ((nes->rom->header[7] & 0xF0) | (nes->rom->header[6] >> 4));

  // Reservar memoria para PRG-ROM (16 KB por banco)
  nes->rom->prg_rom = malloc(nes->rom->prg_size * 16384);
  if (!nes->rom->prg_rom)
  {
    printf("Error: No se pudo asignar memoria para PRG-rom->\n");
    fclose(file);
  }
  fread(nes->rom->prg_rom, 1, nes->rom->prg_size * 16384, file);

  // Reservar memoria para CHR-ROM (8 KB por banco, si existe)
  if (nes->rom->chr_size > 0)
  {
    nes->rom->chr_rom = malloc(nes->rom->chr_size * 8192);
    if (!nes->rom->chr_rom)
    {
      printf("Error: No se pudo asignar memoria para CHR-rom->\n");
      free(nes->rom->prg_rom);
      fclose(file);
    }
    fread(nes->rom->chr_rom, 1, nes->rom->chr_size * 8192, file);
  }
  else
  {
    nes->rom->chr_rom = NULL; // Algunas ROMs usan CHR-RAM en lugar de CHR-ROM
  }

  fclose(file);
  printf("ROM cargada correctamente: %d KB PRG, %d KB CHR\n", nes->rom->prg_size * 16, nes->rom->chr_size * 8);
}