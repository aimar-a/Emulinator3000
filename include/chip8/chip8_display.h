#ifndef DISPLAY_H
#define DISPLAY_H

#include <SDL2/SDL.h>
#include "superchip8bool.h"
#include "chip8_structure.h"
#include <stdbool.h>
#include "chip8_logger.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "chip8_config.h"

typedef struct
{
  SDL_Window *window;
  SDL_Renderer *renderer;
  int running;
} Screen;
void cargarConfiguracion(const char *archivo);
int chip8displayInitPantalla(uint8_t *pantalla_);
void chip8displayLimpiarPantalla();
void chip8displayDestroyPantalla();
uint8_t chip8displayDrawSprite(int x, int y, uint8_t *sprite, int height);
void chip8displayPrintPantalla();

#endif // !DISPLAY_H
