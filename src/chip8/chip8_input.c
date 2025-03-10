#include "chip8_input.h"

#define NUM_KEYS 16

uint8_t *teclado;
bool *esc;
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
  teclado = teclado_;
  esc = esc_;
  memset(teclado, 0, NUM_KEYS);
  esc = false;
}

// Mapea las teclas del teclado de la máquina
void chip8inputCapturarTeclado()
{
  SDL_Event evento;
  while (SDL_PollEvent(&evento))
  {
    if (evento.type == SDL_QUIT)
    {
      exit(0);
    }
    else if (evento.type == SDL_KEYDOWN)
    {
      if (evento.key.keysym.sym == SDLK_ESCAPE)
      {
        *esc = true;
      }
      for (int i = 0; i < NUM_KEYS; i++)
      {
        if (evento.key.keysym.sym == teclado_equivalente[i])
        {
          teclado[i] = 1;
        }
      }
    }
    else if (evento.type == SDL_KEYUP)
    {
      for (int i = 0; i < NUM_KEYS; i++)
      {
        if (evento.key.keysym.sym == teclado_equivalente[i])
        {
          teclado[i] = 0;
        }
      }
    }
  }
}

// Verifica si una tecla está presionada
bool chip8inputEstaPresionada(uint8_t key)
{
  return teclado[key];
}

// Devuelve las teclas presionadas
uint8_t *chip8inputGetTeclado()
{
  return teclado;
}