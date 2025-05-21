
#ifndef CONFIG_H
#define CONFIG_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

extern int SCREEN_WIDTH_CHIP8;
extern int SCREEN_HEIGHT_CHIP8;

extern int SCREEN_WIDTH_SUPERCHIP;
extern int SCREEN_HEIGHT_SUPERCHIP;

extern int SCREEN_WIDTH_NES;
extern int SCREEN_HEIGHT_NES;

extern bool cleanbbdd;
extern bool createbbdd;
extern bool deletebbdd;
extern bool loadSampleData;
extern bool loadGames;

extern bool log_chip8_enabled;
extern bool log_nes_enabled;

void cargarConfiguracion();

#endif // CONFIG_H
