
#ifndef CONFIG_H
#define CONFIG_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

extern int SCREEN_WIDTH_CHIP8;
extern int SCREEN_HEIGHT_CHIP8;
extern int SCREEN_SCALE_CHIP8;

extern int SCREEN_WIDTH_SUPERCHIP;
extern int SCREEN_HEIGHT_SUPERCHIP;
extern int SCREEN_SCALE_SUPERCHIP;

extern int SCREEN_WIDTH_NES;
extern int SCREEN_HEIGHT_NES;
extern int SCREEN_SCALE_NES;

extern bool log_chip8_enabled;
extern bool log_nes_enabled;

void cargarConfiguracion(const char *archivoConfig);
int cambiarEscalaChip8(int nuevaEscala);
int cambiarEscalaSuperChip(int nuevaEscala);
int cambiarEscalaNes(int nuevaEscala);

#endif // CONFIG_H
