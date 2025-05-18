#ifndef NES_CONTROLLER_HPP
#define NES_CONTROLLER_HPP
#include <SDL2/SDL.h>

#define BUTTON_A 0
#define BUTTON_B 1
#define BUTTON_SELECT 2
#define BUTTON_START 3
#define BUTTON_UP 4
#define BUTTON_DOWN 5
#define BUTTON_LEFT 6
#define BUTTON_RIGHT 7

void nes_set_button(uint8_t *controller_state, uint8_t player, uint8_t button, bool pressed);
int nes_controller_update(uint8_t *controllers);

#endif