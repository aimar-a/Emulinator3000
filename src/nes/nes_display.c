#include "nes_display.h"

#define SCREEN_WIDTH 256
#define SCREEN_HEIGHT 240
#define SCREEN_SCALE 4

static SDL_Window *window;
static SDL_Renderer *renderer;
static SDL_Texture *texture;

void nes_display_init()
{
  SDL_Init(SDL_INIT_VIDEO);

  window = SDL_CreateWindow("NES", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH * SCREEN_SCALE, SCREEN_HEIGHT * SCREEN_SCALE, 0);
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);
}

void nes_display_draw(uint8_t *pantalla)
{
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  for (int j = 0; j < SCREEN_HEIGHT; j++)
  {
    for (int i = 0; i < SCREEN_WIDTH; i++)
    {
      uint8_t color = pantalla[i + j * SCREEN_WIDTH];
      SDL_SetRenderDrawColor(renderer, color, color, color, 255);
      SDL_Rect pixel = {i * SCREEN_SCALE, j * SCREEN_SCALE, SCREEN_SCALE, SCREEN_SCALE};
      SDL_RenderFillRect(renderer, &pixel);
    }
  }

  SDL_RenderPresent(renderer);
}

void nes_display_destroy()
{
  SDL_DestroyTexture(texture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}