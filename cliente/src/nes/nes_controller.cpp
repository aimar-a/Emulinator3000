#include "nes_controller.hpp"

void nes_set_button(uint8_t *controller_state, uint8_t player, uint8_t button, bool pressed)
{
  if (player > 1)
    return;

  if (pressed)
  {
    controller_state[player] |= (1 << button);
  }
  else
  {
    controller_state[player] &= ~(1 << button);
  }
}

int nes_controller_update(uint8_t *controllers)
{
  SDL_Event event;
  while (SDL_PollEvent(&event))
  {
    if (event.type == SDL_QUIT)
    {
      return 1; // Exit signal
    }

    if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP)
    {
      bool pressed = (event.type == SDL_KEYDOWN);

      // Player 1 controls
      switch (event.key.keysym.sym)
      {
      case SDLK_z:
        nes_set_button(controllers, 0, BUTTON_A, pressed);
        break;
      case SDLK_x:
        nes_set_button(controllers, 0, BUTTON_B, pressed);
        break;
      case SDLK_RSHIFT:
        nes_set_button(controllers, 0, BUTTON_SELECT, pressed);
        break;
      case SDLK_RETURN:
        nes_set_button(controllers, 0, BUTTON_START, pressed);
        break;
      case SDLK_UP:
        nes_set_button(controllers, 0, BUTTON_UP, pressed);
        break;
      case SDLK_DOWN:
        nes_set_button(controllers, 0, BUTTON_DOWN, pressed);
        break;
      case SDLK_LEFT:
        nes_set_button(controllers, 0, BUTTON_LEFT, pressed);
        break;
      case SDLK_RIGHT:
        nes_set_button(controllers, 0, BUTTON_RIGHT, pressed);
        break;

      // Player 2 controls
      case SDLK_v:
        nes_set_button(controllers, 1, BUTTON_A, pressed);
        break;
      case SDLK_b:
        nes_set_button(controllers, 1, BUTTON_B, pressed);
        break;
      case SDLK_n:
        nes_set_button(controllers, 1, BUTTON_SELECT, pressed);
        break;
      case SDLK_m:
        nes_set_button(controllers, 1, BUTTON_START, pressed);
        break;
      case SDLK_i:
        nes_set_button(controllers, 1, BUTTON_UP, pressed);
        break;
      case SDLK_k:
        nes_set_button(controllers, 1, BUTTON_DOWN, pressed);
        break;
      case SDLK_j:
        nes_set_button(controllers, 1, BUTTON_LEFT, pressed);
        break;
      case SDLK_l:
        nes_set_button(controllers, 1, BUTTON_RIGHT, pressed);
        break;

      case SDLK_ESCAPE:
        return 1; // Exit on ESC

      default:
        break;
      }
    }
  }
  return 0;
}