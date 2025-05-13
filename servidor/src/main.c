#define SDL_MAIN_HANDLED // Evita que SDL2 defina su propia función main
#include <SDL2/SDL.h>
#include "sqlite3.h"
#include <SDL2/SDL_ttf.h> // Si usas texto en la UI
#include "menu_sdl.h"
#include "menu_cmd.h"
#include "bd.h"
#include "config_file.h"
extern sqlite3 *db;
#include "nes_cpu.h"
#include "server.h"

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

  cargarConfiguracion("resources/config/config");

  // Ejecutar acciones según la configuración
  if (load)
  {
    abrirBaseDeDatos(&db);
  }
  if (deletebbdd)
  {
    eliminarBaseDeDatos();
  }

  if (createbbdd)
  {

    crearBD();

    // Al crear la bbdd aparte de crearla en si añadimos todos los juegos de nes y chip8 a la tabla "Juego"
    // no se hace la mejor gestion de memoria ya que en lugar de usar memoria dinamica (porq no sabemos el tamano exacto)
    // usamos memoria estatica por lo que estaremos usando mas memoria de la necesaria
    // mejorar esto para futuras entregas
    char romsCP8[200][256];
    int count = 0;
    listarROMsRecursivo("resources/chip8-roms", romsCP8, &count);

    // recorrer roms CHIP8 y en cada iteracion insertar en la BD
    for (int i = 0; i < count; i++)
    {
      // printf("%d. %s\n", i + 1, romsCP8[i]);

      char nombre[256];

      // Extraer el nombre
      extraerNombreROM(romsCP8[i], nombre); // esto guarda en nombre el "titulo" del juego

      // printf("%s\n", nombre);

      insertarJuego(nombre, romsCP8[i]); // por defecto al crear la Bd la puntuacion record y el usuario record sera 0;
    }

    // lo mismo pero para insertar los roms de la NES
    char romsNES[200][256];
    int count2 = 0;

    listarROMsRecursivoNES("resources/nes-roms", romsNES, &count2);

    // recorrer roms CHIP8 y en cada iteracion insertar en la BD
    for (int i = 0; i < count2; i++)
    {
      // printf("%d. %s\n", i + 1, romsNES[i]);

      char nombre2[256];

      // Extraer el nombre
      extraerNombreROM(romsNES[i], nombre2); // esto guarda en nombre el "titulo" del juego

      // printf("%s\n", nombre2);

      insertarJuego(nombre2, romsNES[i]); // por defecto al crear la Bd la puntuacion record y el usuario record sera 0;
    }
  }

  if (cleanbbdd)
  {
    limpiarBaseDeDatos();
  }

  server_run();

  // Iniciar el menu de usuario
  // menuUsuario();

  // Iniciar la NES directamente
  // nes_launch();

  // Iniciar el Chip8 directamente
  // showInitialWindow();

  return 0;
}