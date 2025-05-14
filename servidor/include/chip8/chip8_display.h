#ifndef DISPLAY_H
#define DISPLAY_H

#include "superchip8bool.h"
#include "chip8_structure.h"
#include <stdbool.h>
#include "chip8_logger.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config_file.h"

void chip8displayLimpiarPantalla();
uint8_t chip8displayDrawSprite(int x, int y, uint8_t *sprite, int height);

#endif // !DISPLAY_H
