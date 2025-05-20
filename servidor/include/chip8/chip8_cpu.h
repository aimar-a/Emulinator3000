#ifndef CPU_H
#define CPU_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "chip8_display.h"
#include "chip8_input.h"
#include "chip8_timers.h"

#include "chip8_opcodes.h"
#include "chip8_structure.h"
#include <SDL2/SDL.h>
#include "chip8_logger.h"
#include "chip8_display.h"

extern int selectedDelay;
Chip8 *chip8init(char *rom_path);
void chip8step(Chip8 *chip8);
void chip8terminate(Chip8 *chip8, char *currentUser);
void inicializarMemoria(Chip8 *chip8, bool modosuperchip8);

#endif // CPU_H