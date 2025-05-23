#include "nes_audio.hpp"
#include <stdexcept>
#include <iostream>

NesAudio::NesAudio() : audioDevice(0), initialized(false)
{
  if (SDL_Init(SDL_INIT_AUDIO) < 0)
  {
    logError("SDL could not initialize audio: " + std::string(SDL_GetError()));
    throw std::runtime_error("SDL audio initialization failed");
  }

  SDL_AudioSpec desiredSpec{};
  desiredSpec.freq = SAMPLE_RATE;
  desiredSpec.format = AUDIO_F32SYS;
  desiredSpec.channels = 1;
  desiredSpec.samples = static_cast<Uint16>(BUFFER_SIZE);
  desiredSpec.callback = nullptr;

  audioDevice = SDL_OpenAudioDevice(nullptr, 0, &desiredSpec, nullptr, 0);
  if (audioDevice == 0)
  {
    logError("SDL could not open audio device: " + std::string(SDL_GetError()));
    throw std::runtime_error("Audio device opening failed");
  }

  SDL_PauseAudioDevice(audioDevice, 0); // Start playing audio
  initialized = true;
  logInfo("NES Audio initialized successfully");
}

NesAudio::~NesAudio()
{
  if (initialized)
  {
    SDL_CloseAudioDevice(audioDevice);
    SDL_QuitSubSystem(SDL_INIT_AUDIO);
    logInfo("NES Audio resources cleaned up");
  }
}

void NesAudio::queueAudio(const std::array<float, BUFFER_SIZE> &audioBuffer)
{
  if (!initialized)
    return;

  if (SDL_QueueAudio(audioDevice, audioBuffer.data(), BUFFER_SIZE * sizeof(float)) != 0)
  {
    logError("Failed to queue audio: " + std::string(SDL_GetError()));
  }
}

void NesAudio::logError(const std::string &message) const
{
  std::cerr << "ERROR [NES Audio]: " << message << std::endl;
}

void NesAudio::logInfo(const std::string &message) const
{
  std::cout << "INFO [NES Audio]: " << message << std::endl;
}