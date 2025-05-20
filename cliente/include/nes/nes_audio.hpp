#ifndef NES_AUDIO_HPP
#define NES_AUDIO_HPP

#include <SDL2/SDL.h>
#include "network_util.hpp"

// Audio settings
#define SAMPLE_RATE 44100
#define BUFFER_SIZE 1024 // en nes_structure.h se define como 1024 manualmente (no bonito)

void nes_audio_init();
void nes_audio_cleanup();
void nes_audio_queue(float *audio_buffer);

#endif