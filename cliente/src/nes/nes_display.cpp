#include "nes_display.hpp"

static SDL_Window *window;
static SDL_Renderer *renderer;

void nes_display_init()
{
  if (SDL_Init(SDL_INIT_VIDEO) != 0)
  {
    printf("ERROR: SDL_Init failed: %s\n", SDL_GetError());
    return;
  }
  printf("INFO: SDL_Init successful\n");

  window = SDL_CreateWindow("NES", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH_NES * SCREEN_SCALE_NES, SCREEN_HEIGHT_NES * SCREEN_SCALE_NES, 0);
  if (!window)
  {
    printf("ERROR: SDL_CreateWindow failed: %s\n", SDL_GetError());
    SDL_Quit();
    return;
  }
  printf("INFO: SDL_CreateWindow successful\n");

  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (!renderer)
  {
    printf("ERROR: SDL_CreateRenderer failed: %s\n", SDL_GetError());
    SDL_DestroyWindow(window);
    SDL_Quit();
    return;
  }
  printf("INFO: SDL_CreateRenderer successful\n");
}

void nes_display_draw(uint8_t *screen_buffer)
{
  // NES color palette
  static const uint32_t nes_palette[64] = {
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

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  for (int y = 0; y < SCREEN_HEIGHT_NES; y++)
  {
    for (int x = 0; x < SCREEN_WIDTH_NES; x++)
    {
      // Get the color index from the screen buffer
      uint8_t palette_index = screen_buffer[x + y * SCREEN_WIDTH_NES] & 0x3F;

      // Get actual NES color (from palette RAM in a real emulator)
      uint32_t nes_color = nes_palette[palette_index];

      // Extract RGB components
      uint8_t r = (nes_color >> 16) & 0xFF;
      uint8_t g = (nes_color >> 8) & 0xFF;
      uint8_t b = nes_color & 0xFF;

      SDL_SetRenderDrawColor(renderer, r, g, b, 255);
      SDL_Rect pixel = {
          x * SCREEN_SCALE_NES,
          y * SCREEN_SCALE_NES,
          SCREEN_SCALE_NES,
          SCREEN_SCALE_NES};
      SDL_RenderFillRect(renderer, &pixel);
    }
  }

  SDL_RenderPresent(renderer);
}

void nes_display_destroy()
{
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}
