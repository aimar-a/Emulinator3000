#ifndef DISPLAY_H
#define DISPLAY_H

#include <SDL2/SDL.h>

//superchip8: las defino como int para que en caso de activar superchip8 se puedan cambiar sus valores
#define SCREEN_WIDTH 64 
#define SCREEN_HEIGHT 32

#define SCREEN_SCALE 26

typedef struct
{
  SDL_Window *window;
  SDL_Renderer *renderer;
  int running;
} Screen;

int chip8displayInitPantalla(uint8_t *pantalla_);
void chip8displayLimpiarPantalla();
void chip8displayDestroyPantalla();
uint8_t chip8displayDrawSprite(int x, int y, uint8_t *sprite, int height);
void chip8displayPrintPantalla();

#endif // !DISPLAY_H
