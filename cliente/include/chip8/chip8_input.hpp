#ifndef CHIP8_INPUT_HPP
#define CHIP8_INPUT_HPP

#include <SDL2/SDL.h>
#include <array>
#include <cstdint>

class Chip8Input
{
public:
  Chip8Input();
  bool captureKeyboard(std::array<uint8_t, 16> &keyboard);

private:
  static constexpr size_t NUM_KEYS = 16;
  const std::array<SDL_Keycode, NUM_KEYS> key_mappings = {
      SDLK_x,                 // 0
      SDLK_1, SDLK_2, SDLK_3, // 1, 2, 3
      SDLK_q, SDLK_w, SDLK_e, // 4, 5, 6
      SDLK_a, SDLK_s, SDLK_d, // 7, 8, 9
      SDLK_z, SDLK_c,         // A(10), B(11)
      SDLK_4, SDLK_r,         // C(12), D(13)
      SDLK_f, SDLK_v          // E(14), F(15)
  };

  void handleKeyDown(const SDL_KeyboardEvent &key, std::array<uint8_t, 16> &keyboard);
  void handleKeyUp(const SDL_KeyboardEvent &key, std::array<uint8_t, 16> &keyboard);
};

#endif // CHIP8_INPUT_HPP