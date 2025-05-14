#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include <SDL2/SDL.h>
// #include "superchip8bool.h"
// #include "chip8_structure.h"
#include <stdbool.h>
// #include "chip8_logger.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config_file.hpp"
#include "network_util.hpp"

typedef struct
{
  SDL_Window *window;
  SDL_Renderer *renderer;
  int running;
} Screen;
void cargarConfiguracion(const char *archivo);
int chip8displayInitPantalla(uint8_t *pantalla_, bool modosuperchip8);
void chip8displayDestroyPantalla();
void chip8displayPrintPantalla(uint8_t *pantalla);

#endif // !DISPLAY_H
