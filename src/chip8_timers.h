#ifndef TIMERS_H
#define TIMERS_H

#include <stdint.h>

void timersInit(uint8_t *delay, uint8_t *sound);
void timersAudioCallback(void *userdata, uint8_t *stream, int len);
void timersInitAudio();
void timersDecrement();
void timersSetDelay(uint8_t value);
void timersSetSound(uint8_t value);
uint8_t timersGetDelay();
uint8_t timersGetSound();

#endif // !TIMERS_H
