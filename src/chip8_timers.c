#include "chip8_timers.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>

uint8_t *delay_timer;
uint8_t *sound_timer;

void timersInit(uint8_t *delay, uint8_t *sound)
{
  delay_timer = delay;
  *delay_timer = 0;
  sound_timer = sound;
  *sound_timer = 0;
  timersInitAudio();
}

// Función de audio callback para generar el sonido (onda cuadrada)
void timersAudioCallback(void *userdata, uint8_t *stream, int len)
{
  if (*sound_timer > 0)
  {
    for (int i = 0; i < len; i++)
    {
      // BEEP (onda cuadrada)
      stream[i] = (i % 100 < 50) ? 255 : 0;
    }
  }
  else
  {
    SDL_memset(stream, 0, len);
  }
}

// Configuración del audio (solo se necesita una vez)
void timersInitAudio()
{
  SDL_Init(SDL_INIT_AUDIO);

  SDL_AudioSpec want, have;
  SDL_memset(&want, 0, sizeof(want));
  want.freq = 44100; // Frecuencia de 44.1 kHz (común para audio)
  want.format = AUDIO_U8;
  want.channels = 1;
  want.samples = 512;
  want.callback = timersAudioCallback;

  if (SDL_OpenAudio(&want, &have) < 0)
  {
    printf("Error al inicializar audio: %s\n", SDL_GetError());
  }
  else
  {
    SDL_PauseAudio(0); // Iniciar el audio
  }
}

void timersSetDelay(uint8_t value)
{
  *delay_timer = value;
}

void timersSetSound(uint8_t value)
{
  *sound_timer = value;
}

void timersDecrement()
{
  if (*delay_timer > 0)
  {
    *delay_timer--;
  }

  if (*sound_timer > 0)
  {
    *sound_timer--;
  }
}

uint8_t timersGetDelay()
{
  return *delay_timer;
}

// BORRAR
uint8_t timersGetSound()
{
  return *sound_timer;
}
