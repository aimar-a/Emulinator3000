#include "chip8_display.h"

uint8_t *pantalla;
Screen screenSDL;

// Inicializa SDL y crea la ventana y el renderer
int chip8displayInitPantalla(uint8_t *pantalla_)
{
  pantalla = pantalla_;
  chip8displayLimpiarPantalla();
  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    printf("Error al inicializar SDL: %s\n", SDL_GetError());
    return 0;
  }

  screenSDL.window =
      SDL_CreateWindow("Pantalla 64x32", SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH * SCREEN_SCALE,
                       SCREEN_HEIGHT * SCREEN_SCALE, SDL_WINDOW_SHOWN);
  if (!screenSDL.window)
  {
    printf("Error al crear la ventana: %s\n", SDL_GetError());
    SDL_Quit();
    return 0;
  }

  screenSDL.renderer = SDL_CreateRenderer(screenSDL.window, -1, SDL_RENDERER_ACCELERATED);
  if (!screenSDL.renderer)
  {
    printf("Error al crear el renderer: %s\n", SDL_GetError());
    SDL_DestroyWindow(screenSDL.window);
    SDL_Quit();
    return 0;
  }

  screenSDL.running = 1;
  return 1;
}

// Limpia la pantalla
void chip8displayLimpiarPantalla()
{
  for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++)
  {
    pantalla[i] = 0;
  }
  chip8displayPrintPantalla();
}

// Destruye la ventana y el renderer
void chip8displayCerrarPantalla()
{
  SDL_DestroyRenderer(screenSDL.renderer);
  SDL_DestroyWindow(screenSDL.window);
  SDL_Quit();
}

// Dibuja un sprite en la pantalla
uint8_t chip8displayDrawSprite(int x, int y, uint8_t *sprite, int height)
{
  uint8_t colision = 0;
  for (int i = 0; i < height; i++)
  {
    for (int j = 0; j < 8; j++)
    {
      if ((sprite[i] & (0x80 >> j)) != 0)
      {
        int pos = (x + j) % SCREEN_WIDTH + ((y + i) % SCREEN_HEIGHT) * SCREEN_WIDTH;
        if (pantalla[pos] == 1)
        {
          colision = 1;
        }
        pantalla[pos] ^= 1;
      }
    }
  }
  return colision;
}

// Dibuja la pantalla en la ventana
void chip8displayPrintPantalla()
{
  SDL_SetRenderDrawColor(screenSDL.renderer, 0, 0, 0, 255);
  SDL_RenderClear(screenSDL.renderer);

  SDL_SetRenderDrawColor(screenSDL.renderer, 255, 255, 255, 255);
  for (int i = 0; i < SCREEN_WIDTH; i++)
  {
    for (int j = 0; j < SCREEN_HEIGHT; j++)
    {
      if (pantalla[i + j * SCREEN_WIDTH] == 1)
      {
        SDL_Rect pixel = {i * SCREEN_SCALE, j * SCREEN_SCALE, SCREEN_SCALE, SCREEN_SCALE};
        SDL_RenderFillRect(screenSDL.renderer, &pixel);
      }
    }
  }
  SDL_RenderPresent(screenSDL.renderer);
}
