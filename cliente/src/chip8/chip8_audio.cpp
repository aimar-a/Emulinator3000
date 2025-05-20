#include "chip8_audio.hpp"

uint8_t *sound_timer;

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
    printf("ERROR: No se pudo abrir el audio: %s\n", SDL_GetError());
    exit(1);
  }
  else
  {
    SDL_PauseAudio(0); // Iniciar el audio
    printf("INFO: Audio initialized successfully.\n");
  }
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

void chip8timersSetSound(uint8_t value)
{
  *sound_timer = value;
}

void chip8audioDestroyAudio()
{
  SDL_CloseAudio();
  SDL_Quit();
}