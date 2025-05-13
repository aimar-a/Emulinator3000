#ifndef NES_APU_H
#define NES_APU_H

#include <SDL2/SDL.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include "nes_structure.h"
#include "nes_logger.h"

void nes_apu_init(APU *apu);
void nes_apu_write(APU *apu, uint16_t address, uint8_t value);
uint8_t nes_apu_read(APU *apu, uint16_t address);
void nes_apu_clock(APU *apu);
void nes_apu_generate_samples(APU *apu, size_t sample_count);
void nes_apu_cleanup(APU *apu);

#endif // NES_APU_H
