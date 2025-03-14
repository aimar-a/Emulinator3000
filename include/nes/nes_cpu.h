#ifndef NES_CPU_H
#define NES_CPU_H

#include "nes_structure.h"

void nes_launch();
void nes_reset(NES *nes);
void nes_load_rom(NES *nes, const char *filename);
void nes_run(NES *nes);

#endif