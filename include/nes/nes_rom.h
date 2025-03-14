#ifndef NES_ROM_H
#define NES_ROM_H

#include <stdint.h>
#include "nes_structure.h" // Incluir la definición completa de NES

typedef struct
{
  uint8_t header[16]; // Cabecera iNES (16 bytes)
  uint8_t *prg_rom;   // PRG-ROM (Código del juego)
  uint8_t *chr_rom;   // CHR-ROM (Gráficos)
  uint8_t prg_size;   // Tamaño de PRG-ROM en bloques de 16 KB
  uint8_t chr_size;   // Tamaño de CHR-ROM en bloques de 8 KB
} NES_ROM;

NES_ROM *nes_load_rom(NES *nes, const char *filename);

#endif