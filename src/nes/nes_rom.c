#include "nes_rom.h"
#include "nes_structure.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

NES_ROM *nes_load_rom(NES *nes, const char *filename)
{
  FILE *file = fopen(filename, "rb");
  if (!file)
  {
    printf("Error: No se pudo abrir la ROM.\n");
    return NULL;
  }

  NES_ROM *rom = malloc(sizeof(NES_ROM));
  if (!rom)
  {
    printf("Error: No se pudo asignar memoria para la ROM.\n");
    fclose(file);
    return NULL;
  }

  // Leer cabecera iNES (16 bytes)
  fread(rom->header, 1, 16, file);

  // Verificar que sea un archivo iNES válido
  if (rom->header[0] != 'N' || rom->header[1] != 'E' || rom->header[2] != 'S' || rom->header[3] != 0x1A)
  {
    printf("Error: No es un archivo .nes válido.\n");
    free(rom);
    fclose(file);
    return NULL;
  }

  // Obtener tamaño de PRG-ROM y CHR-ROM
  rom->prg_size = rom->header[4]; // Cantidad de bancos de 16 KB
  rom->chr_size = rom->header[5]; // Cantidad de bancos de 8 KB

  // Reservar memoria para PRG-ROM (16 KB por banco)
  rom->prg_rom = malloc(rom->prg_size * 16384);
  if (!rom->prg_rom)
  {
    printf("Error: No se pudo asignar memoria para PRG-ROM.\n");
    free(rom);
    fclose(file);
    return NULL;
  }
  fread(rom->prg_rom, 1, rom->prg_size * 16384, file);

  // Reservar memoria para CHR-ROM (8 KB por banco, si existe)
  if (rom->chr_size > 0)
  {
    rom->chr_rom = malloc(rom->chr_size * 8192);
    if (!rom->chr_rom)
    {
      printf("Error: No se pudo asignar memoria para CHR-ROM.\n");
      free(rom->prg_rom);
      free(rom);
      fclose(file);
      return NULL;
    }
    fread(rom->chr_rom, 1, rom->chr_size * 8192, file);
  }
  else
  {
    rom->chr_rom = NULL; // Algunas ROMs usan CHR-RAM en lugar de CHR-ROM
  }

  fclose(file);
  printf("ROM cargada correctamente: %d KB PRG, %d KB CHR\n", rom->prg_size * 16, rom->chr_size * 8);
  return rom;
}