#ifndef INPUT_H
#define INPUT_H

#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "chip8_logger.h"

void chip8inputInitTeclado(uint8_t *teclado_, bool *esc_);
void chip8inputCapturarTeclado();
bool chip8inputEstaPresionada(uint8_t key);
uint8_t *chip8inputGetTeclado();

#endif // !INPUT_H
