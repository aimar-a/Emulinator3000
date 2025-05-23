#ifndef NES_CONTROLLER_HPP
#define NES_CONTROLLER_HPP

#include <SDL2/SDL.h>
#include <array>
#include <cstdint>

class NesController
{
public:
  enum Button
  {
    A = 0,
    B = 1,
    SELECT = 2,
    START = 3,
    UP = 4,
    DOWN = 5,
    LEFT = 6,
    RIGHT = 7
  };

  enum Player
  {
    PLAYER1 = 0,
    PLAYER2 = 1
  };

  NesController();

  // Update controller state and check for quit events
  bool update(std::array<uint8_t, 2> &controllers);

private:
  void setButton(uint8_t &controllerState, Button button, bool pressed);
  void handleKeyEvent(const SDL_KeyboardEvent &key, std::array<uint8_t, 2> &controllers);
};

#endif // NES_CONTROLLER_HPP