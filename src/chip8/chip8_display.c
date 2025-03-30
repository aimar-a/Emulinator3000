#include "chip8_display.h"

uint8_t *pantalla;
Screen screenSDL;

// valores por defecto (display chip8: 64x32)


// Inicializa SDL y crea la ventana y el renderer
int chip8displayInitPantalla(uint8_t *pantalla_)
{
  chip8_log("INFO: Inicializando pantalla...\n");

  cargarConfiguracion("resources/config/config");// Especificar la ruta de tu archivo de configuración


  // si estamos en modosuperchip8 cambiamos los valores del display
  if (modosuperchip8 == true)
  {
      SCREEN_WIDTH = SCREEN_WIDTH_SUPERCHIP;
      SCREEN_HEIGHT = SCREEN_HEIGHT_SUPERCHIP;
      SCREEN_SCALE = SCREEN_SCALE_SUPERCHIP;
      chip8_log("INFO: Modo SuperChip8 activado. Resolución: %dx%d\n", SCREEN_WIDTH, SCREEN_HEIGHT);
  }

  pantalla = pantalla_;
  chip8displayLimpiarPantalla();
  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    chip8_log("ERROR: No se pudo inicializar SDL: %s\n", SDL_GetError());
    return 0;
  }
  screenSDL.window = SDL_CreateWindow("Pantalla", SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH * SCREEN_SCALE,
    SCREEN_HEIGHT * SCREEN_SCALE, SDL_WINDOW_SHOWN);
  if (!screenSDL.window)
  {
    chip8_log("ERROR: Error al crear la ventana: %s\n", SDL_GetError());
    SDL_Quit();
    return 0;
  }

  screenSDL.renderer = SDL_CreateRenderer(screenSDL.window, -1, SDL_RENDERER_ACCELERATED);
  if (!screenSDL.renderer)
  {
    chip8_log("ERROR: Error al crear el renderer: %s\n", SDL_GetError());
    SDL_DestroyWindow(screenSDL.window);
    SDL_Quit();
    return 0;
  }

  screenSDL.running = 1;
  chip8_log("INFO: Pantalla inicializada correctamente.\n");
  return 1;
}

// Limpia la pantalla
void chip8displayLimpiarPantalla()
{
  chip8_log("INFO: Limpiando pantalla...\n");
  for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++)
  {
    pantalla[i] = 0;
  }
  chip8displayPrintPantalla();
  chip8_log("INFO: Pantalla limpiada.\n");
}

// Destruye la ventana y el renderer
void chip8displayDestroyPantalla()
{
  chip8_log("INFO: Destruyendo pantalla...\n");
  SDL_DestroyRenderer(screenSDL.renderer);
  SDL_DestroyWindow(screenSDL.window);
  SDL_Quit();
  chip8_log("INFO: Pantalla destruida.\n");
}

// Dibuja un sprite en la pantalla
uint8_t chip8displayDrawSprite(int x, int y, uint8_t *sprite, int height)
{
  chip8_log("INFO: Dibujando sprite en posición (%d, %d) con altura %d...\n", x, y, height);
  int n = 8; // por defecto en el modo chip8 el sprite tendra 8 pixeles de ancho
  uint8_t colision = 0;
  for (int i = 0; i < height; i++)
  {
    if (height == 16)
    { // si el parametro de height es 16 es decir esta en modo superchip8 el sprite tendra 16 pixeles de ancho
      n = 16;
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
  chip8_log("INFO: Sprite dibujado. Colisión: %d\n", colision);
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
  chip8_log("INFO: Pantalla renderizada.\n");
}
