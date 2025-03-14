#ifndef NES_STRUCTURE_H
#define NES_STRUCTURE_H

#include <stdint.h>
#include <stdbool.h>

typedef struct
{
  // CPU
  uint8_t A, X, Y; // Registros
  uint16_t PC;     // Contador de programa
  uint8_t SP;      // Stack Pointer
  uint8_t P;       // Registro de estado

  // Memoria
  uint8_t memory[0x10000]; // 64KB de memoria

  // Display
  uint8_t screen[256 * 240]; // 256x240 pixels

  // Input
  bool controller_strobe;      // Strobe
  uint8_t controller_shift[2]; // Shift
  uint8_t controller_state[2]; // Estado de los botones

} NES;

#endif