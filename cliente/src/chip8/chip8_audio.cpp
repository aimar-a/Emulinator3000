#include "chip8_audio.hpp"
#include <stdexcept>
#include <array>

Chip8Audio::Chip8Audio() : soundTimer(0), audioDevice(0)
{
  if (SDL_Init(SDL_INIT_AUDIO) < 0)
  {
    logError("Failed to initialize SDL audio: " + std::string(SDL_GetError()));
    throw std::runtime_error("SDL audio initialization failed");
  }

  SDL_AudioSpec want{};
  want.freq = 44100; // 44.1 kHz sample rate
  want.format = AUDIO_U8;
  want.channels = 1;
  want.samples = 512;
  want.callback = &Chip8Audio::audioCallback;
  want.userdata = this;

  audioDevice = SDL_OpenAudioDevice(nullptr, 0, &want, &audioSpec, 0);
  if (audioDevice == 0)
  {
    logError("Failed to open audio device: " + std::string(SDL_GetError()));
    throw std::runtime_error("Audio device opening failed");
  }

  SDL_PauseAudioDevice(audioDevice, 0); // Start audio playback
  logInfo("Audio initialized successfully");
}

Chip8Audio::~Chip8Audio()
{
  if (audioDevice != 0)
  {
    SDL_CloseAudioDevice(audioDevice);
  }
  SDL_QuitSubSystem(SDL_INIT_AUDIO);
  logInfo("Audio resources cleaned up");
}

void Chip8Audio::setSound(uint8_t value)
{
  soundTimer.store(value, std::memory_order_relaxed);
}

void Chip8Audio::audioCallback(void *userdata, Uint8 *stream, int len)
{
  auto *audio = static_cast<Chip8Audio *>(userdata);
  if (audio->soundTimer.load(std::memory_order_relaxed) > 0)
  {
    audio->generateSquareWave(stream, len);
  }
  else
  {
    SDL_memset(stream, 0, len);
  }
}

void Chip8Audio::generateSquareWave(Uint8 *stream, int len)
{
  constexpr int wavePeriod = 100; // Controls the beep frequency
  for (int i = 0; i < len; ++i)
  {
    // Generate square wave (50% duty cycle)
    stream[i] = (i % wavePeriod < wavePeriod / 2) ? 255 : 0;
  }
}

void Chip8Audio::logError(const std::string &message) const
{
  std::cerr << "ERROR [Audio]: " << message << std::endl;
}

void Chip8Audio::logInfo(const std::string &message) const
{
  std::cout << "INFO [Audio]: " << message << std::endl;
}