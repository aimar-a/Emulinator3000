#ifndef INPUT_H
#define INPUT_H

#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

void inputInitTeclado(uint8_t *teclado_, bool *esc_);
// Mapea las teclas del teclado de la máquina
void inputCapturarTeclado();

// Verifica si una tecla está presionada
bool inputEstaPresionada(uint8_t key);

// Devuelve las teclas presionadas
uint8_t *inputGetTeclado();

#endif // !INPUT_H
