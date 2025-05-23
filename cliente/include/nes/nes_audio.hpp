#ifndef NES_AUDIO_HPP
#define NES_AUDIO_HPP

#include <SDL2/SDL.h>
#include <memory>
#include <array>
#include <cstdint>

class NesAudio
{
public:
  static constexpr int SAMPLE_RATE = 44100;
  static constexpr int BUFFER_SIZE = 1024;

  NesAudio();
  ~NesAudio();

  // Delete copy constructor and assignment operator
  NesAudio(const NesAudio &) = delete;
  NesAudio &operator=(const NesAudio &) = delete;

  void queueAudio(const std::array<float, BUFFER_SIZE> &audioBuffer);

private:
  SDL_AudioDeviceID audioDevice;
  bool initialized;

  void logError(const std::string &message) const;
  void logInfo(const std::string &message) const;
};

#endif // NES_AUDIO_HPP