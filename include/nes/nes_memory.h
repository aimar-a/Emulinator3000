#ifndef NES_MEMORY_H
#define NES_MEMORY_H

#include "nes_structure.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "nes_logger.h"
#include "nes_ppu_regs.h"
#include "nes_apu.h"
#include "nes_controller.h"

uint8_t nes_read(NES *nes, uint16_t address);
uint16_t nes_read_address(NES *nes, uint16_t address);
void nes_write(NES *nes, uint16_t address, uint8_t value);

#endif