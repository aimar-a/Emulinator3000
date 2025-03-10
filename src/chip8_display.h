#ifndef DISPLAY_H
#define DISPLAY_H

#include <SDL2/SDL.h>

#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32

#define SCREEN_SCALE 10

typedef struct
{
  SDL_Window *window;
  SDL_Renderer *renderer;
  int running;
} Screen;

int displayInitPantalla(uint8_t *pantalla_);
void displayLimpiarPantalla();
void displayCerrarPantalla();
uint8_t displayDrawSprite(int x, int y, uint8_t *sprite, int height);
void displayPrintPantalla();

#endif // !DISPLAY_H
