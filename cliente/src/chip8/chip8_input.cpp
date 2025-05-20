#include "chip8_input.hpp"

#define NUM_KEYS 16

SDL_KeyCode teclado_equivalente[NUM_KEYS] = {
    SDLK_x,                 // 0
    SDLK_1, SDLK_2, SDLK_3, // 1, 2, 3
    SDLK_q, SDLK_w, SDLK_e, // 4, 5, 6
    SDLK_a, SDLK_s, SDLK_d, // 7, 8, 9
    SDLK_z, SDLK_c,         // A, B
    SDLK_4, SDLK_r,         // C, D
    SDLK_f, SDLK_v          // E, F
};

// Mapea las teclas del teclado de la máquina
bool chip8inputCapturarTeclado(uint16_t *teclado)
{
  bool esc = false;

  SDL_Event evento;
  while (SDL_PollEvent(&evento))
  {
    if (evento.type == SDL_QUIT)
    {
      printf("INFO: Evento SDL_QUIT recibido, saliendo...\n");
      exit(0);
    }
    else if (evento.type == SDL_KEYDOWN)
    {
      printf("INFO: Tecla presionada: %s\n", SDL_GetKeyName(evento.key.keysym.sym));
      if (evento.key.keysym.sym == SDLK_ESCAPE)
      {
        printf("INFO: Tecla ESC presionada\n");
        esc = true;
      }
      for (int i = 0; i < NUM_KEYS; i++)
      {
        if (evento.key.keysym.sym == teclado_equivalente[i])
        {
          *teclado |= (1 << i);
          printf("INFO: Tecla mapeada presionada: %d\n", i);
        }
      }
    }
    else if (evento.type == SDL_KEYUP)
    {
      printf("INFO: Tecla liberada: %s\n", SDL_GetKeyName(evento.key.keysym.sym));
      for (int i = 0; i < NUM_KEYS; i++)
      {
        if (evento.key.keysym.sym == teclado_equivalente[i])
        {
          *teclado &= ~(1 << i);
          printf("INFO: Tecla mapeada liberada: %d\n", i);
        }
      }
    }
  }

  return esc;
}
