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

extern char * currentUser;  // Declaración de la variable global
void menuUsuario();
void menuCambioContraseña();
void menuInicial();
void menuChip8();
void menuListaROMs();

#endif // !MENU_H