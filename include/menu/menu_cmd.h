#ifndef MENU_CMD_H
#define MENU_CMD_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "chip8_cpu.h"
#include "nes_cpu.h"
#include "menu_sdl.h"
#include "bd.h"
#include <stdbool.h>
#include "chip8_config.h"

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

#endif // !MENU_H