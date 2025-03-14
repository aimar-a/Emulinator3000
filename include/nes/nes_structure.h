
#ifndef NES_STRUCTURE_H
#define NES_STRUCTURE_H

#include <stdint.h>
#include <stdbool.h>

typedef struct
{
  uint8_t header[16]; // Cabecera iNES (16 bytes)
  uint8_t *prg_rom;   // PRG-ROM (Código del juego)
  uint8_t *chr_rom;   // CHR-ROM (Gráficos)
  uint8_t prg_size;   // Tamaño de PRG-ROM en bloques de 16 KB
  uint8_t chr_size;   // Tamaño de CHR-ROM en bloques de 8 KB
} NES_ROM;

typedef struct
{
  // CPU
  uint8_t A, X, Y; // Registros
  uint16_t PC;     // Contador de programa
  uint8_t SP;      // Stack Pointer
  uint8_t P;       // Registro de estado

  // Memoria
  uint8_t memory[0x10000]; // 64KB de memoria
  NES_ROM *rom;            // ROM cargada

  // Display
  uint8_t screen[256 * 240]; // 256x240 pixels

  // Input
  bool controller_strobe;      // Strobe
  uint8_t controller_shift[2]; // Shift
  uint8_t controller_state[2]; // Estado de los botones

  // Mappers
  uint8_t mapper_bank;    // Banco de memoria PRG
  uint8_t chr_bank;       // Banco de memoria CHR (para gráficos)
  bool interrupt_enable;  // Habilitar interrupciones (para algunos mappers)
  uint8_t mapper_state;   // Estado del mapper (se usa en MMC1, MMC3, etc.)
  uint8_t current_mapper; // Tipo de mapper actual (como Mapper 0, 1, 3, etc.)
} NES;

#endif