// nes_controller.c
#include "nes_controller.h"
#include <SDL2/SDL.h>

void nes_set_button(NES *nes, uint8_t player, uint8_t button, bool pressed)
{
  if (player > 1)
    return;

  nes_log_traceback("INFO: Player %d button %d %s\n",
                    player, button, pressed ? "pressed" : "released");

  if (pressed)
  {
    nes->controller_state[player] |= (1 << button);
  }
  else
  {
    nes->controller_state[player] &= ~(1 << button);
  }
}

uint8_t nes_read_controller(NES *nes, uint16_t address)
{
  uint8_t result = 0x40; // Bit 6 always set (open bus behavior)

  if (address == 0x4016 || address == 0x4017)
  {
    int player = (address == 0x4016) ? 0 : 1;

    if (!nes->controller_strobe)
    {
      // Normal read: get next button state
      result |= (nes->controller_shift[player] & 1);
      nes->controller_shift[player] >>= 1;
    }
    else
    {
      // Strobe mode: always return first button
      result |= (nes->controller_state[player] & 1);
    }

    nes_log_traceback("INFO: Read controller %d: 0x%02X\n", player, result);
  }
  else
  {
    nes_log_error("ERROR: Invalid controller read at 0x%04X\n", address);
  }

  return result;
}

void nes_write_controller(NES *nes, uint16_t address, uint8_t value)
{
  if (address == 0x4016)
  {
    bool new_strobe = value & 1;

    if (new_strobe && !nes->controller_strobe)
    {
      // Strobe just activated - reload shift registers
      nes->controller_shift[0] = nes->controller_state[0];
      nes->controller_shift[1] = nes->controller_state[1];
      nes_log_traceback("INFO: Controller strobe activated\n");
    }
    else if (!new_strobe && nes->controller_strobe)
    {
      nes_log_traceback("INFO: Controller strobe deactivated\n");
    }

    nes->controller_strobe = new_strobe;
  }
  else
  {
    nes_log_error("ERROR: Invalid controller write at 0x%04X\n", address);
  }
}

int nes_controller_update(NES *nes)
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
        nes_set_button(nes, 0, BUTTON_A, pressed);
        break;
      case SDLK_x:
        nes_set_button(nes, 0, BUTTON_B, pressed);
        break;
      case SDLK_RSHIFT:
        nes_set_button(nes, 0, BUTTON_SELECT, pressed);
        break;
      case SDLK_RETURN:
        nes_set_button(nes, 0, BUTTON_START, pressed);
        break;
      case SDLK_UP:
        nes_set_button(nes, 0, BUTTON_UP, pressed);
        break;
      case SDLK_DOWN:
        nes_set_button(nes, 0, BUTTON_DOWN, pressed);
        break;
      case SDLK_LEFT:
        nes_set_button(nes, 0, BUTTON_LEFT, pressed);
        break;
      case SDLK_RIGHT:
        nes_set_button(nes, 0, BUTTON_RIGHT, pressed);
        break;

      // Player 2 controls
      case SDLK_v:
        nes_set_button(nes, 1, BUTTON_A, pressed);
        break;
      case SDLK_b:
        nes_set_button(nes, 1, BUTTON_B, pressed);
        break;
      case SDLK_n:
        nes_set_button(nes, 1, BUTTON_SELECT, pressed);
        break;
      case SDLK_m:
        nes_set_button(nes, 1, BUTTON_START, pressed);
        break;
      case SDLK_i:
        nes_set_button(nes, 1, BUTTON_UP, pressed);
        break;
      case SDLK_k:
        nes_set_button(nes, 1, BUTTON_DOWN, pressed);
        break;
      case SDLK_j:
        nes_set_button(nes, 1, BUTTON_LEFT, pressed);
        break;
      case SDLK_l:
        nes_set_button(nes, 1, BUTTON_RIGHT, pressed);
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