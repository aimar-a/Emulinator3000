#include "chip8_display.h"
#include "superchip8bool.h"
#include "chip8_structure.h"
#include <stdbool.h>

uint8_t *pantalla;
Screen screenSDL;




//valores por defecto (display chip8: 64x32)
int SCREEN_WIDTH = 64;
int SCREEN_HEIGHT = 32;

int SCREEN_SCALE = 26;

// Inicializa SDL y crea la ventana y el renderer
int chip8displayInitPantalla(uint8_t *pantalla_)
{

  //si estamos en modosuperchip8 cambiamos los valores del display
  if(modosuperchip8==true){
    //128x64 pixeles en modo superchip8
    SCREEN_WIDTH = 128;
    SCREEN_HEIGHT = 64;
  
    SCREEN_SCALE = 13;//equivalente a escala del chip8
  }

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
void chip8displayDestroyPantalla()
{
  SDL_DestroyRenderer(screenSDL.renderer);
  SDL_DestroyWindow(screenSDL.window);
  SDL_Quit();
}

// Dibuja un sprite en la pantalla
uint8_t chip8displayDrawSprite(int x, int y, uint8_t *sprite, int height)
{
  int n = 8; //por defecto en el modo chip8 el sprite tendra 8 pixeles de ancho
  uint8_t colision = 0;
  for (int i = 0; i < height; i++)
  {
    if(height==16){ //si el parametro de height es 16 es decir esta en modo superchip8 el sprite tendra 16 pixeles de ancho
      n=16;
    }
    for (int j = 0; j < n; j++)
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
