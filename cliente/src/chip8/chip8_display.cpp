#include "chip8_display.hpp"

Screen screenSDL;

// valores por defecto (display chip8: 64x32)

// Inicializa SDL y crea la ventana y el renderer
int chip8displayInitPantalla(uint8_t *pantalla_, bool modosuperchip8)
{

  cargarConfiguracion("resources/config/config"); // Especificar la ruta de tu archivo de configuración

  // si estamos en modosuperchip8 cambiamos los valores del display
  if (modosuperchip8 == true)
  {
    SCREEN_WIDTH_CHIP8 = SCREEN_WIDTH_SUPERCHIP;
    SCREEN_HEIGHT_CHIP8 = SCREEN_HEIGHT_SUPERCHIP;
    SCREEN_SCALE_CHIP8 = SCREEN_SCALE_SUPERCHIP;
    printf("INFO: Modo SuperChip8 activado. Resolución: %dx%d\n", SCREEN_WIDTH_CHIP8, SCREEN_HEIGHT_CHIP8);
  }

  chip8displayLimpiarPantalla();
  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    printf("ERROR: No se pudo inicializar SDL: %s\n", SDL_GetError());
    return 0;
  }
  screenSDL.window = SDL_CreateWindow("Pantalla", SDL_WINDOWPOS_CENTERED,
                                      SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH_CHIP8 * SCREEN_SCALE_CHIP8,
                                      SCREEN_HEIGHT_CHIP8 * SCREEN_SCALE_CHIP8, SDL_WINDOW_SHOWN);
  if (!screenSDL.window)
  {
    printf("ERROR: Error al crear la ventana: %s\n", SDL_GetError());
    SDL_Quit();
    return 0;
  }

  screenSDL.renderer = SDL_CreateRenderer(screenSDL.window, -1, SDL_RENDERER_ACCELERATED);
  if (!screenSDL.renderer)
  {
    printf("ERROR: Error al crear el renderer: %s\n", SDL_GetError());
    SDL_DestroyWindow(screenSDL.window);
    SDL_Quit();
    return 0;
  }

  screenSDL.running = 1;
  printf("INFO: Pantalla inicializada correctamente.\n");
  return 1;
}

// Destruye la ventana y el renderer
void chip8displayDestroyPantalla()
{
  printf("INFO: Destruyendo pantalla...\n");
  SDL_DestroyRenderer(screenSDL.renderer);
  SDL_DestroyWindow(screenSDL.window);
  SDL_Quit();
  printf("INFO: Pantalla destruida.\n");
}

// Dibuja la pantalla en la ventana
void chip8displayPrintPantalla(uint8_t *pantalla)
{
  SDL_SetRenderDrawColor(screenSDL.renderer, 0, 0, 0, 255);
  SDL_RenderClear(screenSDL.renderer);

  SDL_SetRenderDrawColor(screenSDL.renderer, 255, 255, 255, 255);
  for (int i = 0; i < SCREEN_WIDTH_CHIP8; i++)
  {
    for (int j = 0; j < SCREEN_HEIGHT_CHIP8; j++)
    {
      if (pantalla[i + j * SCREEN_WIDTH_CHIP8] == 1)
      {
        SDL_Rect pixel = {i * SCREEN_SCALE_CHIP8, j * SCREEN_SCALE_CHIP8, SCREEN_SCALE_CHIP8, SCREEN_SCALE_CHIP8};
        SDL_RenderFillRect(screenSDL.renderer, &pixel);
      }
    }
  }
  SDL_RenderPresent(screenSDL.renderer);
  printf("INFO: Pantalla renderizada.\n");
}
