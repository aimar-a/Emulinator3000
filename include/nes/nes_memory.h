#ifndef NES_MEMORY_H
#define NES_MEMORY_H

#include "nes_structure.h"
#include <stddef.h>

uint8_t nes_read(NES *nes, uint16_t address);
void nes_write(NES *nes, uint16_t address, uint8_t value);

#endif