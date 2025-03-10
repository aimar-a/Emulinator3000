#ifndef INPUT_H
#define INPUT_H

#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

// Mapea las teclas del teclado de la máquina
void capturarTeclado();

// Verifica si una tecla está presionada
bool estaPresionada(uint8_t key);

// Devuelve las teclas presionadas
uint8_t *getTeclado();

#endif // !INPUT_H
