#include "chip8_input.h"

#define NUM_KEYS 16

uint8_t *teclado;
SDL_KeyCode teclado_equivalente[NUM_KEYS] = {
    SDLK_x,                 // 0
    SDLK_1, SDLK_2, SDLK_3, // 1, 2, 3
    SDLK_q, SDLK_w, SDLK_e, // 4, 5, 6
    SDLK_a, SDLK_s, SDLK_d, // 7, 8, 9
    SDLK_z, SDLK_c,         // A, B
    SDLK_4, SDLK_r,         // C, D
    SDLK_f, SDLK_v          // E, F
};

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