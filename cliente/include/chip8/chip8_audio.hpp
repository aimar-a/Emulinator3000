#ifndef CHIP8_AUDIO_HPP
#define CHIP8_AUDIO_HPP

#include <stdint.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>

void chip8timersInitAudio();
void chip8timersAudioCallback(void *userdata, uint8_t *stream, int len);
void chip8timersSetSound(uint8_t value);
void chip8audioDestroyAudio();

#endif