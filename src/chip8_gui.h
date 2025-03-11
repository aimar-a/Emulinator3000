#ifndef GUI_H
#define GUI_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "chip8_display.h"
#include "chip8_input.h"
#include "chip8_timers.h"
#include "chip8_opcodes.h"

void showSettingsWindow();
void showInitialWindow(int *startGame);
void loadRomsFromDirectory(const char *path, char roms[][128], int *romCount);

#endif