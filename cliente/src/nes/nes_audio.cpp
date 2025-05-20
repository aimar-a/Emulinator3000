#include "nes_audio.hpp"

SDL_AudioDeviceID audio_device;

void nes_audio_init()
{
  // Initialize SDL audio
  if (SDL_Init(SDL_INIT_AUDIO) < 0)
  {
    printf("ERROR: SDL could not initialize audio! SDL_Error: %s\n", SDL_GetError());
    return;
  }

  // Set up audio format
  SDL_AudioSpec desired_spec;
  desired_spec.freq = SAMPLE_RATE;
  desired_spec.format = AUDIO_F32SYS;
  desired_spec.channels = 1;
  desired_spec.samples = BUFFER_SIZE;
  desired_spec.callback = NULL;

  // Open audio device
  audio_device = SDL_OpenAudioDevice(NULL, 0, &desired_spec, NULL, 0);
  if (audio_device == 0)
  {
    printf("ERROR: SDL could not open audio device! SDL_Error: %s\n", SDL_GetError());
    return;
  }

  // Start playing audio
  SDL_PauseAudio(0);
}

// Clean up the APU
void nes_audio_cleanup()
{
  SDL_CloseAudioDevice(audio_device);
}

void nes_audio_queue(float *audio_buffer)
{
  // Queue audio samples
  SDL_QueueAudio(audio_device, audio_buffer, BUFFER_SIZE * sizeof(float));
}