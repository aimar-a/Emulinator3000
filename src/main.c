#define SDL_MAIN_HANDLED // Evita que SDL2 defina su propia función main
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h> // Si usas texto en la UI
#include "menu_sdl.h"
#include "menu_cmd.h"
#include "bd.h"

#include "nes_cpu.h"

/*

PARA EJECUTAR EL CODIGO: (dentro de la consola MinGW32)

make
bin/chip8_emulator.exe

*/

// Dejo esto por aqui q es de un tio q ha hecho la super chip8, hay juegos en el repo para descargar y probar (@Davidrs05)
// https://github.com/zaymat/super-chip8/tree/master

int main(int argc, char *argv[])
{
  // creamos la BD con sus estructuras en el caso en el que no existan
  crearBD();

  // Iniciar la nes directamente
  // nes_launch();

  // Iniciar el menu CMD
  menuInicial();

  // Iniciar el menu SDL
  // showInitialWindow();

  return 0;
}