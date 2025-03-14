#ifndef NES_MAPPER_H
#define NES_MAPPER_H

#include <stdint.h>
#include <stddef.h>

#include "nes_structure.h"

void nes_rom_to_memory(NES *nes, uint8_t *rom, size_t rom_size);
void nes_rom_free(NES *nes);

void init_nrom(NES *nes);
uint8_t read_nrom(NES *nes, uint16_t address);
void write_nrom(NES *nes, uint16_t address, uint8_t value);

void init_mmc1(NES *nes);
uint8_t read_mmc1(NES *nes, uint16_t address);
void write_mmc1(NES *nes, uint16_t address, uint8_t value);

void init_unrom(NES *nes);
uint8_t read_unrom(NES *nes, uint16_t address);
void write_unrom(NES *nes, uint16_t address, uint8_t value);

void init_cnrom(NES *nes);
uint8_t read_cnrom(NES *nes, uint16_t address);
void write_cnrom(NES *nes, uint16_t address, uint8_t value);

void init_mmc3(NES *nes);
uint8_t read_mmc3(NES *nes, uint16_t address);
void write_mmc3(NES *nes, uint16_t address, uint8_t value);

#endif