#include "nes_controller.h"

int nes_controller_update(NES *nes)
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
        nes_log_traceback("INFO: Escape key pressed, exiting.\n");
        return 1;
        break;
      case SDLK_w:
        nes_log_traceback("INFO: W key %s\n", state ? "pressed" : "released");
        nes_set_button(nes, 0, 4, state);
        break;
      case SDLK_a:
        nes_log_traceback("INFO: A key %s\n", state ? "pressed" : "released");
        nes_set_button(nes, 0, 6, state);
        break;
      case SDLK_s:
        nes_log_traceback("INFO: S key %s\n", state ? "pressed" : "released");
        nes_set_button(nes, 0, 5, state);
        break;
      case SDLK_d:
        nes_log_traceback("INFO: D key %s\n", state ? "pressed" : "released");
        nes_set_button(nes, 0, 7, state);
        break;
      case SDLK_j:
        nes_log_traceback("INFO: J key %s\n", state ? "pressed" : "released");
        nes_set_button(nes, 0, 0, state);
        break;
      case SDLK_k:
        nes_log_traceback("INFO: K key %s\n", state ? "pressed" : "released");
        nes_set_button(nes, 0, 1, state);
        break;
      case SDLK_RETURN:
        nes_log_traceback("INFO: RETURN key %s\n", state ? "pressed" : "released");
        nes_set_button(nes, 0, 2, state);
        break;
      case SDLK_RSHIFT:
        nes_log_traceback("INFO: RSHIFT key %s\n", state ? "pressed" : "released");
        nes_set_button(nes, 0, 3, state);
        break;
      case SDLK_UP:
        nes_log_traceback("INFO: UP key %s\n", state ? "pressed" : "released");
        nes_set_button(nes, 1, 4, state);
        break;
      case SDLK_LEFT:
        nes_log_traceback("INFO: LEFT key %s\n", state ? "pressed" : "released");
        nes_set_button(nes, 1, 6, state);
        break;
      case SDLK_DOWN:
        nes_log_traceback("INFO: DOWN key %s\n", state ? "pressed" : "released");
        nes_set_button(nes, 1, 5, state);
        break;
      case SDLK_RIGHT:
        nes_log_traceback("INFO: RIGHT key %s\n", state ? "pressed" : "released");
        nes_set_button(nes, 1, 7, state);
        break;
      case SDLK_KP_1:
        nes_log_traceback("INFO: KP_1 key %s\n", state ? "pressed" : "released");
        nes_set_button(nes, 1, 0, state);
        break;
      case SDLK_KP_2:
        nes_log_traceback("INFO: KP_2 key %s\n", state ? "pressed" : "released");
        nes_set_button(nes, 1, 1, state);
        break;
      case SDLK_KP_ENTER:
        nes_log_traceback("INFO: KP_ENTER key %s\n", state ? "pressed" : "released");
        nes_set_button(nes, 1, 2, state);
        break;
      case SDLK_KP_0:
        nes_log_traceback("INFO: KP_0 key %s\n", state ? "pressed" : "released");
        nes_set_button(nes, 1, 3, state);
        break;
      default:
        nes_log_instant("WARNING: Unhandled key event: %d\n", event.key.keysym.sym);
        break;
      }
    }
  }
  return 0;
}

void nes_write_controller(NES *nes, uint16_t address, uint8_t value)
{
  if (address == 0x4016)
  {
    if (value & 1)
    {
      nes_log_traceback("INFO: Controller strobe activated.\n");
      nes->controller_strobe = 1; // Activar el strobe (reset del registro)
    }
    else
    {
      nes_log_traceback("INFO: Controller strobe deactivated.\n");
      nes->controller_strobe = 0;
      nes->controller_shift[0] = nes->controller_state[0]; // Copiar estado de los botones del jugador 1
      nes->controller_shift[1] = nes->controller_state[1]; // Copiar estado de los botones del jugador 2
    }
  }
  else
  {
    nes_log_error("ERROR: Invalid write address: 0x%04X\n", address);
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

    nes_log_traceback("INFO: Read controller state for player %d: 0x%02X\n", player, result);
  }
  else
  {
    nes_log_error("ERROR: Invalid read address: 0x%04X\n", address);
  }

  return result;
}

void nes_set_button(NES *nes, uint8_t player, uint8_t button, bool pressed)
{
  nes_log_traceback("INFO: Player %d button %d %s\n", player, button, pressed ? "pressed" : "released");
  if (pressed)
    nes->controller_state[player] |= (1 << button);
  else
    nes->controller_state[player] &= ~(1 << button);
}
