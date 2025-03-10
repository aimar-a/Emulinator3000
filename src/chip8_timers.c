#include "chip8_timers.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>

uint8_t delay_timer = 0;
uint8_t sound_timer = 0;

// Función de audio callback para generar el sonido (onda cuadrada)
void audioCallback(void *userdata, uint8_t *stream, int len)
{
  if (sound_timer > 0)
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
void iniciarSonido()
{
  SDL_AudioSpec want, have;
  SDL_memset(&want, 0, sizeof(want));
  want.freq = 44100; // Frecuencia de 44.1 kHz (común para audio)
  want.format = AUDIO_U8;
  want.channels = 1;
  want.samples = 512;
  want.callback = audioCallback;

  if (SDL_OpenAudio(&want, &have) < 0)
  {
    printf("Error al inicializar audio: %s\n", SDL_GetError());
  }
  else
  {
    SDL_PauseAudio(0); // Iniciar el audio
  }
}

void set_delay_timer(uint8_t value)
{
  delay_timer = value;
}

void set_sound_timer(uint8_t value)
{
  sound_timer = value;
}

void decrement_timers()
{
  if (delay_timer > 0)
  {
    delay_timer--;
  }

  if (sound_timer > 0)
  {
    sound_timer--;
  }
}

uint8_t get_delay_timer()
{
  return delay_timer;
}

// BORRAR
uint8_t get_sound_timer()
{
  return sound_timer;
}
