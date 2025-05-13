#ifndef NES_CONTROLLER_H
#define NES_CONTROLLER_H

#include <stdint.h>
#include <stdbool.h>
#include "nes_structure.h"
#include <SDL2/SDL.h>
#include "nes_logger.h"

// Button bit positions (matches NES controller shift order)
#define BUTTON_A 0
#define BUTTON_B 1
#define BUTTON_SELECT 2
#define BUTTON_START 3
#define BUTTON_UP 4
#define BUTTON_DOWN 5
#define BUTTON_LEFT 6
#define BUTTON_RIGHT 7

void nes_set_button(NES *nes, uint8_t player, uint8_t button, bool pressed);
uint8_t nes_read_controller(NES *nes, uint16_t address);
void nes_write_controller(NES *nes, uint16_t address, uint8_t value);
int nes_controller_update(NES *nes);

#endif