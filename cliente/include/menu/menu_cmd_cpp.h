#ifndef MENU_CMD_CPP_H
#define MENU_CMD_CPP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "menu_sdl_cpp.h"
//  #include "bd.h"
#include <stdbool.h>
// #include "config_file.h"

#ifdef _WIN32
#include <windows.h> // Para usar "Sleep" en Windows
#else
#include <unistd.h>                   // Para usar "sleep" en Linux/Mac
#define Sleep(ms) usleep((ms) * 1000) // Convertir milisegundos a microsegundos
#endif

#define MAX_STRING_LENGTH 128
#define ROMS_PATH "resources/chip8-roms"

extern char *currentUser; // Declaración de la variable global
void menuUsuario();
void menuCambioContraseña();
void menuInicial();
void menuChip8();
void menuListaROMs();
void menuAdvertenciaNES();
void menuConfiguracion();
void menuEscalaChip8();
void menuEscalaSuperChip8();
void menuEscalaNes();
void listarROMsRecursivo(const char *directory, char roms[][256], int *count);
void extraerNombreROM(const char *entrada, char *nombre);
void listarROMsRecursivoNES(const char *directory, char roms[][256], int *count);
void menuPerfil();
void menuVerTiempoJugado();
void mostrarPartidasJugadas(char *nombreJuego);

#endif // !MENU_H