#ifndef NES_DISPLAY_H
#define NES_DISPLAY_H

#include <stdint.h>

void nes_display_init();
void nes_display_draw(uint8_t *pantalla);
void nes_display_destroy();

#endif