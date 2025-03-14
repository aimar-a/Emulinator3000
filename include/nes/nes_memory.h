#ifndef NES_MEMORY_H
#define NES_MEMORY_H

#include "nes_structure.h"

uint8_t nes_read(NES *nes, uint16_t address);
void nes_write(NES *nes, uint16_t address, uint8_t value);

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