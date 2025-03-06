#include "chip8_input.h"

#define NUM_KEYS 16

uint8_t teclado[NUM_KEYS] = {0};
SDL_KeyCode teclado_equivalente[NUM_KEYS] = {
    SDLK_1, SDLK_2, SDLK_3, SDLK_4,
    SDLK_q, SDLK_w, SDLK_e, SDLK_r,
    SDLK_a, SDLK_s, SDLK_d, SDLK_f,
    SDLK_z, SDLK_x, SDLK_c, SDLK_v};

// Mapea las teclas del teclado de la máquina
void capturarTeclado()
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
bool estaPresionada(uint8_t key)
{
  return teclado[key];
}

// Devuelve las teclas presionadas
uint8_t *getTeclado()
{
  return teclado;
}