#include "nes_display.hpp"
#include <stdexcept>
#include <iostream>

NesDisplay::NesDisplay()
    : window(nullptr, SDL_DestroyWindow),
      renderer(nullptr, SDL_DestroyRenderer)
{
}

NesDisplay::~NesDisplay()
{
  // Resources are automatically cleaned up by unique_ptr
}

bool NesDisplay::initialize()
{
  if (SDL_Init(SDL_INIT_VIDEO) != 0)
  {
    logError("SDL_Init failed: " + std::string(SDL_GetError()));
    return false;
  }
  logInfo("SDL initialized successfully");

  window.reset(SDL_CreateWindow(
      "NES Emulator",
      SDL_WINDOWPOS_UNDEFINED,
      SDL_WINDOWPOS_UNDEFINED,
      SCREEN_WIDTH_NES * SCREEN_SCALE_NES,
      SCREEN_HEIGHT_NES * SCREEN_SCALE_NES,
      SDL_WINDOW_SHOWN));

  if (!window)
  {
    logError("SDL_CreateWindow failed: " + std::string(SDL_GetError()));
    SDL_Quit();
    return false;
  }
  logInfo("Window created successfully");

  renderer.reset(SDL_CreateRenderer(window.get(), -1, SDL_RENDERER_ACCELERATED));
  if (!renderer)
  {
    logError("SDL_CreateRenderer failed: " + std::string(SDL_GetError()));
    window.reset();
    SDL_Quit();
    return false;
  }
  logInfo("Renderer created successfully");

  return true;
}

void NesDisplay::render(const uint8_t *screenBuffer)
{
  // Clear screen
  SDL_SetRenderDrawColor(renderer.get(), 0, 0, 0, 255);
  SDL_RenderClear(renderer.get());

  // Draw pixels
  for (int y = 0; y < SCREEN_HEIGHT_NES; ++y)
  {
    for (int x = 0; x < SCREEN_WIDTH_NES; ++x)
    {
      const uint8_t paletteIndex = screenBuffer[x + y * SCREEN_WIDTH_NES] & 0x3F;
      const uint32_t nesColor = nesPalette[paletteIndex];

      SDL_SetRenderDrawColor(
          renderer.get(),
          (nesColor >> 16) & 0xFF, // R
          (nesColor >> 8) & 0xFF,  // G
          nesColor & 0xFF,         // B
          255                      // A
      );

      const SDL_Rect pixel = {
          x * SCREEN_SCALE_NES,
          y * SCREEN_SCALE_NES,
          SCREEN_SCALE_NES,
          SCREEN_SCALE_NES};
      SDL_RenderFillRect(renderer.get(), &pixel);
    }
  }

  // Update screen
  SDL_RenderPresent(renderer.get());
}

void NesDisplay::logError(const std::string &message) const
{
  std::cerr << "ERROR [NES Display]: " << message << std::endl;
}

void NesDisplay::logInfo(const std::string &message) const
{
  std::cout << "INFO [NES Display]: " << message << std::endl;
}