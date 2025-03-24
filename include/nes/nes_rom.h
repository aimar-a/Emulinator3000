#ifndef NES_ROM_H
#define NES_ROM_H

#include <stdint.h>
#include "nes_structure.h"
#include "nes_mapper.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

void nes_load_rom(NES *nes, const char *filename);

#endif