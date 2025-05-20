#ifndef TIMERS_H
#define TIMERS_H

#include <stdint.h>
#include "chip8_logger.h"

void chip8timersInit(uint8_t *delay, uint8_t *sound);
void chip8timersDecrement();
void chip8timersSetDelay(uint8_t value);
void chip8timersSetSound(uint8_t value);
uint8_t chip8timersGetDelay();

#endif // !TIMERS_H
