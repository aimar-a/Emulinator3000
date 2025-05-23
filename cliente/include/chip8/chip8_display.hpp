#ifndef CHIP8_DISPLAY_HPP
#define CHIP8_DISPLAY_HPP

#include <SDL2/SDL.h>
#include <memory>
#include <string>
#include <iostream>
#include "config_file.hpp"
#include "network_util.hpp"

class Chip8Display
{
public:
  Chip8Display();
  ~Chip8Display();

  bool initialize(bool superChipMode);
  void destroy();
  void render(const uint8_t *screenBuffer);

private:
  std::unique_ptr<SDL_Window, void (*)(SDL_Window *)> window;
  std::unique_ptr<SDL_Renderer, void (*)(SDL_Renderer *)> renderer;

  void cleanup();
  void logError(const std::string &message) const;
  void logInfo(const std::string &message) const;
};

#endif // CHIP8_DISPLAY_HPP