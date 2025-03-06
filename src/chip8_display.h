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

// Inicializa la pantalla
int iniciarPantalla(Screen *p);

// Limpia la pantalla
void limpiarPantalla(Screen *p);

// Destruye la ventana y el renderer
void cerrarPantalla(Screen *p);

// Dibuja un sprite en la pantalla
uint8_t drawSprite(int x, int y, uint8_t *sprite, int height);

// Dibuja la pantalla en la ventana
void printPantalla(Screen *p);

#endif // !DISPLAY
