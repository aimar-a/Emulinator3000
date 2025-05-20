#include "chip8_display.hpp"

static SDL_Window *window;
static SDL_Renderer *renderer;

// Inicializa SDL y crea la ventana y el renderer
int chip8displayInitPantalla(bool modosuperchip8)
{
  printf("INFO: Cargando configuracion...\n");
  cargarConfiguracion("resources/config/config"); // Especificar la ruta de tu archivo de configuración
  printf("INFO: Configuracion cargada.\n");

  // si estamos en modosuperchip8 cambiamos los valores del display
  if (modosuperchip8 == true)
  {
    SCREEN_WIDTH_CHIP8 = SCREEN_WIDTH_SUPERCHIP;
    SCREEN_HEIGHT_CHIP8 = SCREEN_HEIGHT_SUPERCHIP;
    SCREEN_SCALE_CHIP8 = SCREEN_SCALE_SUPERCHIP;
    printf("INFO: Modo SuperChip8 activado. Resolución: %dx%d\n", SCREEN_WIDTH_CHIP8, SCREEN_HEIGHT_CHIP8);
  }

  // chip8displayLimpiarPantalla();
  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    printf("ERROR: No se pudo inicializar SDL: %s\n", SDL_GetError());
    return 0;
  }
  window = SDL_CreateWindow("Pantalla", SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH_CHIP8 * SCREEN_SCALE_CHIP8,
                            SCREEN_HEIGHT_CHIP8 * SCREEN_SCALE_CHIP8, SDL_WINDOW_SHOWN);
  if (!window)
  {
    printf("ERROR: Error al crear la ventana: %s\n", SDL_GetError());
    SDL_Quit();
    return 0;
  }
  printf("INFO: Ventana creada correctamente.\n");

  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (!renderer)
  {
    printf("ERROR: Error al crear el renderer: %s\n", SDL_GetError());
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
  }
  printf("INFO: Renderer creado correctamente.\n");

  return 1;
}

// Destruye la ventana y el renderer
void chip8displayDestroyPantalla()
{
  printf("INFO: Destruyendo pantalla...\n");
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  printf("INFO: Pantalla destruida.\n");
}

// Dibuja la pantalla en la ventana
void chip8displayPrintPantalla(uint8_t *pantalla)
{
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  for (int i = 0; i < SCREEN_WIDTH_CHIP8; i++)
  {
    for (int j = 0; j < SCREEN_HEIGHT_CHIP8; j++)
    {
      if (pantalla[i + j * SCREEN_WIDTH_CHIP8] == 1)
      {
        SDL_Rect pixel = {i * SCREEN_SCALE_CHIP8, j * SCREEN_SCALE_CHIP8, SCREEN_SCALE_CHIP8, SCREEN_SCALE_CHIP8};
        SDL_RenderFillRect(renderer, &pixel);
      }
    }
  }
  SDL_RenderPresent(renderer);
}
