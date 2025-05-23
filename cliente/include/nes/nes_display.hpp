#ifndef NES_DISPLAY_HPP
#define NES_DISPLAY_HPP

#include <SDL2/SDL.h>
#include <memory>
#include <array>
#include "config_file.hpp"

class NesDisplay
{
public:
  NesDisplay();
  ~NesDisplay();

  bool initialize();
  void render(const uint8_t *screenBuffer);

  // Delete copy constructor and assignment operator
  NesDisplay(const NesDisplay &) = delete;
  NesDisplay &operator=(const NesDisplay &) = delete;

private:
  std::unique_ptr<SDL_Window, void (*)(SDL_Window *)> window;
  std::unique_ptr<SDL_Renderer, void (*)(SDL_Renderer *)> renderer;

  static constexpr std::array<uint32_t, 64> nesPalette = {
      // Row 0 (Grays/dark colors)
      0x666666, 0x002A88, 0x1412A7, 0x3B00A4, 0x5C007E, 0x6E0040, 0x6C0600, 0x561D00,
      0x333800, 0x0B4800, 0x005200, 0x004F08, 0x00404D, 0x000000, 0x000000, 0x000000,

      // Row 1 (Medium colors)
      0xADADAD, 0x155FD9, 0x4240FF, 0x7527FE, 0xA01ACC, 0xB71E7B, 0xB53120, 0x994E00,
      0x6B6D00, 0x388700, 0x0C9300, 0x008F32, 0x007C8D, 0x000000, 0x000000, 0x000000,

      // Row 2 (Light colors)
      0xFFFFFF, 0x64B0FF, 0x9290FF, 0xC676FF, 0xF36AFF, 0xFE6ECC, 0xFE8170, 0xEA9E22,
      0xBCBE00, 0x88D800, 0x5CE430, 0x45E082, 0x48CDDE, 0x4F4F4F, 0x000000, 0x000000,

      // Row 3 (Duplicate light colors with slight variations)
      0xFFFFFF, 0x64B0FF, 0x9290FF, 0xC676FF, 0xF36AFF, 0xFE6ECC, 0xFE8170, 0xEA9E22,
      0xBCBE00, 0x88D800, 0x5CE430, 0x45E082, 0x48CDDE, 0x4F4F4F, 0x000000, 0x000000};

  void logError(const std::string &message) const;
  void logInfo(const std::string &message) const;
};

#endif // NES_DISPLAY_HPP