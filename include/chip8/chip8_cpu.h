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


extern int selectedDelay;
void chip8cpuLaunch(char *rom_path);


#endif // CPU_H