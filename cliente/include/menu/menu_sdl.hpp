#ifndef MENU_SDL_HPP
#define MENU_SDL_HPP

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include "chip8_display.h"
// #include "chip8_input.h"
// #include "chip8_timers.h"
// #include "chip8_opcodes.h"

int showSettingsWindow();
bool isRomSelected(char romOptions[][128], char selectedRom[128], int romCount);
void showInitialWindow();

#endif