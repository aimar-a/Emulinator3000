#ifndef NES_ROM_H
#define NES_ROM_H

#include <stdint.h>
#include "nes_structure.h"
#include "nes_mapper.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "nes_logger.h"

#define MIRROR_VERTICAL 0
#define MIRROR_HORIZONTAL 1

int nes_load_rom(NES *nes, const char *filename);
void nes_get_rom_info(NES *nes);

#endif