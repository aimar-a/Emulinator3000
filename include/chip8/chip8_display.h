#ifndef DISPLAY_H
#define DISPLAY_H


#include <SDL2/SDL.h>

extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;

extern int SCREEN_SCALE;


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
