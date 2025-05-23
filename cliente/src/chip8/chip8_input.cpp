#include "chip8_input.hpp"
#include <iostream>

Chip8Input::Chip8Input()
{
  // Constructor can initialize any required resources
}

bool Chip8Input::captureKeyboard(std::array<uint8_t, 16> &keyboard)
{
  SDL_Event event;
  bool quitRequested = false;

  while (SDL_PollEvent(&event))
  {
    switch (event.type)
    {
    case SDL_QUIT:
      std::cout << "INFO: SDL_QUIT event received, exiting..." << std::endl;
      exit(0);
      break;

    case SDL_KEYDOWN:
      if (event.key.keysym.sym == SDLK_ESCAPE)
      {
        std::cout << "INFO: ESC key pressed" << std::endl;
        quitRequested = true;
      }
      handleKeyDown(event.key, keyboard);
      break;

    case SDL_KEYUP:
      handleKeyUp(event.key, keyboard);
      break;
    }
  }

  return quitRequested;
}

void Chip8Input::handleKeyDown(const SDL_KeyboardEvent &key, std::array<uint8_t, 16> &keyboard)
{
  std::cout << "INFO: Key pressed: " << SDL_GetKeyName(key.keysym.sym) << std::endl;

  for (size_t i = 0; i < NUM_KEYS; ++i)
  {
    if (key.keysym.sym == key_mappings[i])
    {
      keyboard[i] = 1;
      std::cout << "INFO: Mapped key pressed: " << i << std::endl;
    }
  }
}

void Chip8Input::handleKeyUp(const SDL_KeyboardEvent &key, std::array<uint8_t, 16> &keyboard)
{
  std::cout << "INFO: Key released: " << SDL_GetKeyName(key.keysym.sym) << std::endl;

  for (size_t i = 0; i < NUM_KEYS; ++i)
  {
    if (key.keysym.sym == key_mappings[i])
    {
      keyboard[i] = 0;
      std::cout << "INFO: Mapped key released: " << i << std::endl;
    }
  }
}