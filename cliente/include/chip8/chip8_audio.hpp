#ifndef CHIP8_AUDIO_HPP
#define CHIP8_AUDIO_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>
#include <memory>
#include <iostream>
#include <atomic>

class Chip8Audio
{
public:
  Chip8Audio();
  ~Chip8Audio();

  void setSound(uint8_t value);

  // Delete copy constructor and assignment operator
  Chip8Audio(const Chip8Audio &) = delete;
  Chip8Audio &operator=(const Chip8Audio &) = delete;

private:
  std::atomic<uint8_t> soundTimer;
  SDL_AudioSpec audioSpec;
  SDL_AudioDeviceID audioDevice;

  static void audioCallback(void *userdata, Uint8 *stream, int len);
  void generateSquareWave(Uint8 *stream, int len);
  void logError(const std::string &message) const;
  void logInfo(const std::string &message) const;
};

#endif // CHIP8_AUDIO_HPP