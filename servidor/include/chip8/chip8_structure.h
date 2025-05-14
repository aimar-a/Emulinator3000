#ifndef STRUCTURE_H
#define STRUCTURE_H

#include "superchip8bool.h"
#include <stdint.h>
#include <stdbool.h>

// extern uint8_t pantalla_chip8[64 * 32];
// extern uint8_t pantalla_superchip8[128 * 64];

typedef struct
{
  time_t tiempoInicio, tiempoFin;
  char *rom_path; // ruta de la ROM

  uint8_t V[16]; // Registros de propósito general
  uint16_t pc;   // Contador de programa
  uint16_t I;    // Registro de dirección
  uint8_t sp;    // Puntero de pila
  uint8_t delay_timer;
  uint8_t sound_timer;
  uint16_t stack[16];

  uint8_t *memoria; // declaramos la memoria con un puntero para poder cambiar de tamaño dependiendo de si es chip8 o superchip8

  uint8_t teclado[16]; // Teclado de 16 teclas
  uint8_t *pantalla;

  bool esc;
} Chip8;

void inicializarMemoria(Chip8 *chip8, bool modosuperchip8);

#endif