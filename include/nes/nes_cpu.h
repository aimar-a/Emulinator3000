#ifndef NES_CPU_H
#define NES_CPU_H

#include "nes_structure.h"
#include "nes_opcodes.h"
#include "nes_structure.h"
#include "nes_display.h"
#include "nes_controller.h"
#include "nes_memory.h"
#include "nes_rom.h"
#include "nes_mapper.h"
#include "nes_ppu.h"
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "nes_logger.h"

void nes_launch();
void nes_reset(NES *nes);
void nes_run(NES *nes);
void log_check_ppu_ram(NES *nes);

#endif