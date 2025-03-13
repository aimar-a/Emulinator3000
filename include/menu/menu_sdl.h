#ifndef MENU_SDL_H
#define MENU_SDL_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "chip8_display.h"
#include "chip8_input.h"
#include "chip8_timers.h"
#include "chip8_opcodes.h"

void showSettingsWindow();
void showInitialWindow();
void loadRomsFromDirectory(const char *path, char roms[][128], int *romCount);

#endif