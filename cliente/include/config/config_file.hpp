#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>

extern int SCREEN_WIDTH_CHIP8;
extern int SCREEN_HEIGHT_CHIP8;
extern int SCREEN_SCALE_CHIP8;

extern int SCREEN_WIDTH_SUPERCHIP;
extern int SCREEN_HEIGHT_SUPERCHIP;
extern int SCREEN_SCALE_SUPERCHIP;

extern int SCREEN_WIDTH_NES;
extern int SCREEN_HEIGHT_NES;
extern int SCREEN_SCALE_NES;

void cargarConfiguracion();
int cambiarEscalaChip8(int nuevaEscala);
int cambiarEscalaSuperChip(int nuevaEscala);
int cambiarEscalaNes(int nuevaEscala);

#endif // CONFIG_HPP