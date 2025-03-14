#ifndef NES_ROM_H
#define NES_ROM_H

#include <stdint.h>
#include "nes_structure.h" // Incluir la definición completa de NES

void nes_load_rom(NES *nes, const char *filename);

#endif