#ifndef TIMERS_H
#define TIMERS_H

#include <stdint.h>
#include "chip8_logger.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>

void chip8timersInit(uint8_t *delay, uint8_t *sound);
void chip8timersAudioCallback(void *userdata, uint8_t *stream, int len);
void chip8timersInitAudio();
void chip8timersDecrement();
void chip8timersSetDelay(uint8_t value);
void chip8timersSetSound(uint8_t value);
uint8_t chip8timersGetDelay();

#endif // !TIMERS_H
