#include "chip8_timers.h"

uint8_t *delay_timer;
uint8_t *sound_timer;

void chip8timersInit(uint8_t *delay, uint8_t *sound)
{
  delay_timer = delay;
  *delay_timer = 0;
  sound_timer = sound;
  *sound_timer = 0;
  chip8_log("INFO: Timers initialized with delay and sound set to 0.\n");
  chip8timersInitAudio();
}

// Función de audio callback para generar el sonido (onda cuadrada)
void chip8timersAudioCallback(void *userdata, uint8_t *stream, int len)
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
void chip8timersInitAudio()
{
  SDL_Init(SDL_INIT_AUDIO);

  SDL_AudioSpec want, have;
  SDL_memset(&want, 0, sizeof(want));
  want.freq = 44100; // Frecuencia de 44.1 kHz (común para audio)
  want.format = AUDIO_U8;
  want.channels = 1;
  want.samples = 512;
  want.callback = chip8timersAudioCallback;

  if (SDL_OpenAudio(&want, &have) < 0)
  {
    chip8_log("ERROR: No se pudo abrir el audio: %s\n", SDL_GetError());
    exit(1);
  }
  else
  {
    chip8_log("INFO: Audio initialized successfully.\n");
    SDL_PauseAudio(0); // Iniciar el audio
  }
}

void chip8timersSetDelay(uint8_t value)
{
  *delay_timer = value;
  chip8_log("INFO: Delay timer set to %d.\n", value);
}

void chip8timersSetSound(uint8_t value)
{
  *sound_timer = value;
  chip8_log("INFO: Sound timer set to %d.\n", value);
}

void chip8timersDecrement()
{
  if (*delay_timer > 0)
  {
    (*delay_timer)--;
    chip8_log("INFO: Delay timer decremented to %d.\n", *delay_timer);
  }

  if (*sound_timer > 0)
  {
    (*sound_timer)--;
    chip8_log("INFO: Sound timer decremented to %d.\n", *sound_timer);
  }
}

uint8_t chip8timersGetDelay()
{
  return *delay_timer;
}