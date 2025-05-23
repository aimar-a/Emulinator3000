#include "nes_controller.hpp"
#include <iostream>

NesController::NesController()
{
  // Constructor can initialize any required resources
}

bool NesController::update(std::array<uint8_t, 2> &controllers)
{
  SDL_Event event;
  while (SDL_PollEvent(&event))
  {
    switch (event.type)
    {
    case SDL_QUIT:
      return true; // Exit signal

    case SDL_KEYDOWN:
    case SDL_KEYUP:
      handleKeyEvent(event.key, controllers);
      break;

    default:
      break;
    }
  }
  return false;
}

void NesController::setButton(uint8_t &controllerState, Button button, bool pressed)
{
  if (pressed)
  {
    controllerState |= (1 << button);
  }
  else
  {
    controllerState &= ~(1 << button);
  }
}

void NesController::handleKeyEvent(const SDL_KeyboardEvent &key, std::array<uint8_t, 2> &controllers)
{
  const bool pressed = (key.type == SDL_KEYDOWN);

  switch (key.keysym.sym)
  {
  // Player 1 controls
  case SDLK_z:
    setButton(controllers[PLAYER1], A, pressed);
    break;
  case SDLK_x:
    setButton(controllers[PLAYER1], B, pressed);
    break;
  case SDLK_RSHIFT:
    setButton(controllers[PLAYER1], SELECT, pressed);
    break;
  case SDLK_RETURN:
    setButton(controllers[PLAYER1], START, pressed);
    break;
  case SDLK_UP:
    setButton(controllers[PLAYER1], UP, pressed);
    break;
  case SDLK_DOWN:
    setButton(controllers[PLAYER1], DOWN, pressed);
    break;
  case SDLK_LEFT:
    setButton(controllers[PLAYER1], LEFT, pressed);
    break;
  case SDLK_RIGHT:
    setButton(controllers[PLAYER1], RIGHT, pressed);
    break;

  // Player 2 controls
  case SDLK_v:
    setButton(controllers[PLAYER2], A, pressed);
    break;
  case SDLK_b:
    setButton(controllers[PLAYER2], B, pressed);
    break;
  case SDLK_n:
    setButton(controllers[PLAYER2], SELECT, pressed);
    break;
  case SDLK_m:
    setButton(controllers[PLAYER2], START, pressed);
    break;
  case SDLK_i:
    setButton(controllers[PLAYER2], UP, pressed);
    break;
  case SDLK_k:
    setButton(controllers[PLAYER2], DOWN, pressed);
    break;
  case SDLK_j:
    setButton(controllers[PLAYER2], LEFT, pressed);
    break;
  case SDLK_l:
    setButton(controllers[PLAYER2], RIGHT, pressed);
    break;

  case SDLK_ESCAPE:
    // ESC is handled in update() through the return value
    break;

  default:
    break;
  }
}