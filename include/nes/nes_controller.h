#ifndef NES_CONTROLLER_H
#define NES_CONTROLLER_H

#include <stdint.h>
#include <stdbool.h>
#include "nes_structure.h"
#include <SDL2/SDL.h>
#include "nes_logger.h"

int nes_controller_update(NES *nes);
void nes_write_controller(NES *nes, uint16_t address, uint8_t value);
uint8_t nes_read_controller(NES *nes, uint16_t address);
void nes_set_button(NES *nes, uint8_t controller, uint8_t button, bool state);

#endif