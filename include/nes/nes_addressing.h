#ifndef NES_ADDRESSING_H
#define NES_ADDRESSING_H

#include <stdint.h>
#include "nes_structure.h"
#include "nes_memory.h"
#include "nes_logger.h"

uint8_t nes_immediate(NES *nes);
uint16_t nes_absolute(NES *nes);
uint16_t nes_absolute_x(NES *nes);
uint16_t nes_absolute_y(NES *nes);
uint8_t nes_zero_page(NES *nes);
uint8_t nes_zero_page_x(NES *nes);
uint8_t nes_zero_page_y(NES *nes);
uint16_t nes_indirect_x(NES *nes);
uint16_t nes_indirect_y(NES *nes);

#endif