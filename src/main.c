#include "chip8_gui.h"

#define SDL_MAIN_HANDLED // Evita que SDL2 defina su propia función main
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h> // Si usas texto en la UI

/*

PARA EJECUTAR EL CODIGO: (dentro de la consola MinGW32)

make
bin/chip8_emulator.exe

*/

int main(int argc, char *argv[])
{
  showInitialWindow();
  return 0;
}