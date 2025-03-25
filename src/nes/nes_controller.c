#include "nes_controller.h"

void nes_controller_update(NES *nes)
{
  SDL_Event event;
  while (SDL_PollEvent(&event))
  {
    if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP)
    {
      bool state = event.type == SDL_KEYDOWN;
      switch (event.key.keysym.sym)
      {
      case SDLK_ESCAPE:
        nes_log("INFO: Escape key pressed, exiting.");
        exit(0);
        break;
      case SDLK_w:
        nes_log("INFO: W key %s", state ? "pressed" : "released");
        nes_set_button(nes, 0, 4, state);
        break;
      case SDLK_a:
        nes_log("INFO: A key %s", state ? "pressed" : "released");
        nes_set_button(nes, 0, 6, state);
        break;
      case SDLK_s:
        nes_log("INFO: S key %s", state ? "pressed" : "released");
        nes_set_button(nes, 0, 5, state);
        break;
      case SDLK_d:
        nes_log("INFO: D key %s", state ? "pressed" : "released");
        nes_set_button(nes, 0, 7, state);
        break;
      case SDLK_j:
        nes_log("INFO: J key %s", state ? "pressed" : "released");
        nes_set_button(nes, 0, 0, state);
        break;
      case SDLK_k:
        nes_log("INFO: K key %s", state ? "pressed" : "released");
        nes_set_button(nes, 0, 1, state);
        break;
      case SDLK_RETURN:
        nes_log("INFO: RETURN key %s", state ? "pressed" : "released");
        nes_set_button(nes, 0, 2, state);
        break;
      case SDLK_RSHIFT:
        nes_log("INFO: RSHIFT key %s", state ? "pressed" : "released");
        nes_set_button(nes, 0, 3, state);
        break;
      case SDLK_UP:
        nes_log("INFO: UP key %s", state ? "pressed" : "released");
        nes_set_button(nes, 1, 4, state);
        break;
      case SDLK_LEFT:
        nes_log("INFO: LEFT key %s", state ? "pressed" : "released");
        nes_set_button(nes, 1, 6, state);
        break;
      case SDLK_DOWN:
        nes_log("INFO: DOWN key %s", state ? "pressed" : "released");
        nes_set_button(nes, 1, 5, state);
        break;
      case SDLK_RIGHT:
        nes_log("INFO: RIGHT key %s", state ? "pressed" : "released");
        nes_set_button(nes, 1, 7, state);
        break;
      case SDLK_KP_1:
        nes_log("INFO: KP_1 key %s", state ? "pressed" : "released");
        nes_set_button(nes, 1, 0, state);
        break;
      case SDLK_KP_2:
        nes_log("INFO: KP_2 key %s", state ? "pressed" : "released");
        nes_set_button(nes, 1, 1, state);
        break;
      case SDLK_KP_ENTER:
        nes_log("INFO: KP_ENTER key %s", state ? "pressed" : "released");
        nes_set_button(nes, 1, 2, state);
        break;
      case SDLK_KP_0:
        nes_log("INFO: KP_0 key %s", state ? "pressed" : "released");
        nes_set_button(nes, 1, 3, state);
        break;
      default:
        nes_log("WARNING: Unhandled key event: %d", event.key.keysym.sym);
        break;
      }
    }
  }
}

void nes_write_controller(NES *nes, uint16_t address, uint8_t value)
{
  if (address == 0x4016)
  {
    if (value & 1)
    {
      nes_log("INFO: Controller strobe activated.");
      nes->controller_strobe = 1; // Activar el strobe (reset del registro)
    }
    else
    {
      nes_log("INFO: Controller strobe deactivated.");
      nes->controller_strobe = 0;
      nes->controller_shift[0] = nes->controller_state[0]; // Copiar estado de los botones del jugador 1
      nes->controller_shift[1] = nes->controller_state[1]; // Copiar estado de los botones del jugador 2
    }
  }
  else
  {
    nes_log("ERROR: Invalid write address: 0x%04X", address);
  }
}

uint8_t nes_read_controller(NES *nes, uint16_t address)
{
  uint8_t result = 0;

  if (address == 0x4016 || address == 0x4017)
  {
    int player = (address == 0x4016) ? 0 : 1; // 0 = P1, 1 = P2

    result = nes->controller_shift[player] & 1; // Obtener el bit menos significativo
    nes->controller_shift[player] >>= 1;        // Desplazar a la derecha

    result |= 0x40; // Bit 6 siempre es 1 en la NES

    nes_log("INFO: Read controller state for player %d: 0x%02X", player, result);
  }
  else
  {
    nes_log("ERROR: Invalid read address: 0x%04X", address);
  }

  return result;
}

void nes_set_button(NES *nes, uint8_t player, uint8_t button, bool pressed)
{
  nes_log("INFO: Player %d button %d %s", player, button, pressed ? "pressed" : "released");
  if (pressed)
    nes->controller_state[player] |= (1 << button);
  else
    nes->controller_state[player] &= ~(1 << button);
}
