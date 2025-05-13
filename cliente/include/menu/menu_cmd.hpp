#ifndef MENU_CMD_HPP
#define MENU_CMD_HPP

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "menu_sdl.hpp"
#include <stdbool.h>
#include "network_util.hpp"

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#define Sleep(ms) usleep((ms) * 1000)
#endif

#define MAX_STRING_LENGTH 128
#define ROMS_PATH "resources/chip8-roms"

extern char *currentUser;

void menuUsuario(socket_t sock);
void menuInicial(socket_t sock);
void menuConfiguracion(socket_t sock);
void menuCambioContraseña(socket_t sock);
void menuChip8(socket_t sock);
void menuListaROMs(socket_t sock);
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