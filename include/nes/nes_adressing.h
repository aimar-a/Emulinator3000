#ifndef NES_ADRESSING_H
#define NES_ADRESSING_H

#include <stdint.h>
#include "nes_structure.h"
#include "nes_memory.h"

uint8_t nes_absolute(NES *nes);
uint8_t nes_absolute_x(NES *nes);
uint8_t nes_absolute_y(NES *nes);
uint8_t nes_immediate(NES *nes);
uint8_t nes_indirect_x(NES *nes);
uint8_t nes_indirect_y(NES *nes);
uint8_t nes_zero_page(NES *nes);
uint8_t nes_zero_page_x(NES *nes);
uint8_t nes_zero_page_y(NES *nes);

#endif