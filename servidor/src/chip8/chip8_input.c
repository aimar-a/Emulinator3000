#include "chip8_input.h"

#define NUM_KEYS 16

uint8_t *teclado;

// Inicializa el teclado
void chip8inputInitTeclado(uint8_t *teclado_, bool *esc_)
{
  chip8_log("INFO: Inicializando el teclado\n");
  teclado = teclado_;
  memset(teclado, 0, NUM_KEYS);
}

// Verifica si una tecla está presionada
bool chip8inputEstaPresionada(uint8_t key)
{
  chip8_log("INFO: Verificando si la tecla %d está presionada\n", key);
  return teclado[key];
}

// Devuelve las teclas presionadas
uint8_t *chip8inputGetTeclado()
{
  chip8_log("INFO: Devolviendo el estado del teclado\n");
  return teclado;
}